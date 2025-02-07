#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "chess_logic.h"
#include "types.h"
#include "legal_moves.h"
#include "chess_logic.h"
#include "interface.h"

piece empty_piece()
{
    piece piece;
    piece.name = ' ';
    piece.color = ' ';
    return piece;
}

coords empty_coords()
{
    coords coords;
    coords.x = -1;
    coords.y = -1;
    return coords;
}

move empty_move()
{
    move move;
    move.init_co = empty_coords();
    move.dest_co = empty_coords();
    return move;
}

bool is_empty(piece piece)
{
    return piece.name == ' ' && piece.color == ' ';
}

bool is_empty_coords(coords coords)
{
    return coords.x == -1 && coords.y == -1;
}

bool is_empty_move(move move)
{
    return is_empty_coords(move.init_co) && is_empty_coords(move.dest_co);
}

bool are_same_piece(piece piece1, piece piece2)
{
    return piece1.name == piece2.name && piece1.color == piece2.color;
}

position_list *empty_list()
{
    return NULL;
}

position_list *save_position(board_state *board_s, position_list *pos_l)
{
    board_state *board_s_copy = malloc(sizeof(board_state));
    if (board_s_copy == NULL)
    {
        return NULL;
    }
    *board_s_copy = *board_s;
    position_list *new_list = malloc(sizeof(position_list));
    if (new_list == NULL)
    {
        return NULL;
    }
    new_list->board_s = board_s_copy;
    new_list->tail = pos_l;
    return new_list;
}

bool are_same_pos(board_state *board_s1, board_state *board_s2)
{
    piece(*board1)[8] = board_s1->board;
    piece(*board2)[8] = board_s2->board;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!are_same_piece(board1[i][j], board2[i][j]))
            {
                return false;
            }
        }
    }
    // clang-format off
    return board_s1->black_kingside_castlable == board_s2->black_kingside_castlable && board_s1->black_queenside_castlable == board_s2->black_queenside_castlable
        && board_s1->white_kingside_castlable == board_s2->white_kingside_castlable && board_s1->white_queenside_castlable == board_s2->white_queenside_castlable
        && board_s1->black_pawn_passant == board_s2->black_pawn_passant && board_s1->white_pawn_passant == board_s2->white_pawn_passant;
    // clang-format on
}

bool threefold_repetition(board_state *board_s, position_list *pos_l, int number_of_repetitions)
{
    if (pos_l == NULL)
    {
        return false;
    }
    else
    {
        // print_board(pos_l->board_s, empty_piece(), empty_coords());
        if (are_same_pos(board_s, pos_l->board_s))
        {
            number_of_repetitions++;
        }
        if (number_of_repetitions > 2)
        {
            return true;
        }
        return threefold_repetition(board_s, pos_l->tail, number_of_repetitions);
    }
}

piece get_piece(piece board[8][8], coords coords)
{
    piece piece;
    piece.name = board[coords.x][coords.y].name;
    piece.color = board[coords.x][coords.y].color;
    return piece;
}

bool can_move_heuristic(board_state *board_s, piece piece, coords init_co, coords new_co, bool check_would_stop)
{
    int i = new_co.x;
    int j = new_co.y;
    // clang-format off
    return !(piece.name == ' ')
        && !(piece.name == 'P' && (abs(init_co.x - i) > 2 || abs(init_co.y - j) > 1))
        && !(piece.name == 'N' && abs(init_co.x - i) + abs(init_co.y - j) != 3)
        && !(piece.name == 'B' && abs(init_co.x - i) != abs(init_co.y - j))
        && !(piece.name == 'R' && init_co.x != i && init_co.y != j)
        && !(piece.name == 'Q' && (abs(init_co.x - i) != abs(init_co.y - j) && init_co.x != i && init_co.y != j))
        && !(piece.name == 'K' &&( abs(init_co.x - i) > 1 || abs(init_co.y - j) > 1))
        && can_move(board_s, piece, init_co, new_co, check_would_stop);
    // clang-format on
}

bool is_attacked(board_state *board_s, coords co, char color, bool check_would_stop)
{
    piece(*board)[8] = board_s->board;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            piece piece = board[i][j];
            if (piece.color != color)
            {
                coords piece_coords;
                piece_coords.x = i;
                piece_coords.y = j;
                if (can_move_heuristic(board_s, piece, piece_coords, co, check_would_stop))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

board_state *move_pawn_handling(board_state *board_s, piece move_piece, piece dest_piece, coords init_coords, coords new_coords)
{
    // printf("move_pawn_handling: color: %c, init_coords: (%d, %d), new_coords: (%d, %d)\n", move_piece.color, init_coords.x, init_coords.y, new_coords.x, new_coords.y);
    // printf("dest_piece: %c, %c\n", dest_piece.name, dest_piece.color);
    if ((move_piece.color == 'w' && new_coords.x == 7) || (move_piece.color == 'b' && new_coords.x == 0))
    {
        char promotion = prompt_promotion(board_s, move_piece, init_coords, new_coords);
        board_s->board[new_coords.x][new_coords.y].name = promotion;
    }
    if (move_piece.color == 'w' && new_coords.x - init_coords.x == 2)
    {
        printf("2 pas, color: %c, init_coords: (%d, %d), new_coords: (%d, %d)\n", move_piece.color, init_coords.x, init_coords.y, new_coords.x, new_coords.y);
        board_s->white_pawn_passant = new_coords.y;
    }
    else if (move_piece.color == 'b' && init_coords.x - new_coords.x == 2)
    {
        printf("2 pas, color: %c, init_coords: (%d, %d), new_coords: (%d, %d)\n", move_piece.color, init_coords.x, init_coords.y, new_coords.x, new_coords.y);
        board_s->black_pawn_passant = new_coords.y;
    }
    if (move_piece.color == 'w' && is_empty(dest_piece) && new_coords.y != init_coords.y)
    {
        printf("En passant\n");
        board_s->board[new_coords.x - 1][new_coords.y] = empty_piece();
    }
    else if (move_piece.color == 'b' && is_empty(dest_piece) && new_coords.y != init_coords.y)
    {
        printf("En passant\n");
        board_s->board[new_coords.x + 1][new_coords.y] = empty_piece();
    }
    return board_s;
}

board_state *move_king_handling(board_state *board_s, piece piece, coords init_coords, coords new_coords)
{
    if (piece.color == 'w')
    {
        board_s->white_kingside_castlable = false;
        board_s->white_queenside_castlable = false;
    }
    else
    {
        board_s->black_kingside_castlable = false;
        board_s->black_queenside_castlable = false;
    }
    if (new_coords.y == 6 && init_coords.y == 4)
    {
        board_s->board[new_coords.x][5].name = 'R';
        board_s->board[new_coords.x][5].color = piece.color;
        board_s->board[new_coords.x][7] = empty_piece();
    }
    else if (new_coords.y == 2 && init_coords.y == 4)
    {
        board_s->board[new_coords.x][3].name = 'R';
        board_s->board[new_coords.x][3].color = piece.color;
        board_s->board[new_coords.x][0] = empty_piece();
    }
    return board_s;
}

board_state *move_rook_handling(board_state *board_s, piece piece, coords init_coords, coords new_coords)
{
    if (piece.color == 'w' && init_coords.x == 0 && init_coords.y == 0)
    {
        board_s->white_queenside_castlable = false;
    }
    else if (piece.color == 'w' && init_coords.x == 0 && init_coords.y == 7)
    {
        board_s->white_kingside_castlable = false;
    }
    else if (piece.color == 'b' && init_coords.x == 7 && init_coords.y == 0)
    {
        board_s->black_queenside_castlable = false;
    }
    else if (piece.color == 'b' && init_coords.x == 7 && init_coords.y == 7)
    {
        board_s->black_kingside_castlable = false;
    }
    return board_s;
}

board_state *move_piece(board_state *board_s, coords init_coords, coords new_coords)
{
    piece move_piece = get_piece(board_s->board, init_coords);
    piece dest_piece = get_piece(board_s->board, new_coords);
    board_s->white_pawn_passant = -1;
    board_s->black_pawn_passant = -1;
    board_s->board[new_coords.x][new_coords.y].name = move_piece.name;
    board_s->board[new_coords.x][new_coords.y].color = move_piece.color;
    board_s->board[init_coords.x][init_coords.y] = empty_piece();
    // fifty move rule
    if (dest_piece.name == ' ' && move_piece.name != 'P')
    {
        board_s->fifty_move_rule++;
    }
    else
    {
        board_s->fifty_move_rule = 0;
    }
    // printf("move_piece: %c (%d, %d) -> (%d, %d)\n", move_piece.name, init_coords.x, init_coords.y, new_coords.x, new_coords.y);
    if (move_piece.name == 'P')
    {
        board_s = move_pawn_handling(board_s, move_piece, dest_piece, init_coords, new_coords);
    }
    else if (move_piece.name == 'K')
    {
        board_s = move_king_handling(board_s, move_piece, init_coords, new_coords);
    }
    else if (move_piece.name == 'R')
    {
        board_s = move_rook_handling(board_s, move_piece, init_coords, new_coords);
    }
    return board_s;
}

board_state *move_piece_forced(board_state *board_s, coords init_coords, coords new_coords)
{
    piece move_piece = get_piece(board_s->board, init_coords);
    //  castling
    if (new_coords.y == 6 && init_coords.y == 4)
    {
        board_s->board[new_coords.x][5].name = 'R';
        board_s->board[new_coords.x][7] = empty_piece();
    }
    else if (new_coords.y == 2 && init_coords.y == 4)
    {
        board_s->board[new_coords.x][3].name = 'R';
        board_s->board[new_coords.x][0] = empty_piece();
    }
    // en passant
    if (move_piece.name == 'P' && is_empty(board_s->board[new_coords.x][new_coords.y]) && new_coords.y != init_coords.y)
    {
        if (move_piece.color == 'w')
        {
            board_s->board[new_coords.x + 1][new_coords.y] = empty_piece();
        }
        else
        {
            board_s->board[new_coords.x - 1][new_coords.y] = empty_piece();
        }
    }
    // normal move
    board_s->board[new_coords.x][new_coords.y].name = move_piece.name;
    board_s->board[new_coords.x][new_coords.y].color = move_piece.color;
    board_s->board[init_coords.x][init_coords.y] = empty_piece();
    return board_s;
}

coords find_king(board_state *board_s, char color)
{
    piece(*board)[8] = board_s->board;
    coords king_coords = empty_coords();
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            piece piece = board[i][j];
            if (piece.name == 'K' && piece.color == color)
            {
                king_coords.x = i;
                king_coords.y = j;
                return king_coords;
            }
        }
    }
    return king_coords;
}

bool is_check(board_state *board_s, char color)
{
    coords king_coords = find_king(board_s, color);
    return is_attacked(board_s, king_coords, color, false);
}

bool is_checkmate(board_state *board_s, char color)
{
    piece(*board)[8] = board_s->board;
    if (!is_check(board_s, color))
    {
        return false;
    }
    coords init_coords;
    coords new_coords;
    piece piece;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            piece = board[i][j];
            if (piece.color == color)
            {
                init_coords.x = i;
                init_coords.y = j;
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        new_coords.x = k;
                        new_coords.y = l;
                        if (can_move_heuristic(board_s, piece, init_coords, new_coords, true))
                        {
                            printf("can move: %c (%d, %d) -> (%d, %d)\n", piece.name, init_coords.x, init_coords.y, new_coords.x, new_coords.y);
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool is_stalemate(board_state *board_s, char color)
{
    piece(*board)[8] = board_s->board;
    if (is_check(board_s, color))
    {
        return false;
    }
    coords init_coords;
    coords new_coords;
    piece piece;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            piece = board[i][j];
            if (piece.color == color)
            {
                init_coords.x = i;
                init_coords.y = j;
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        new_coords.x = k;
                        new_coords.y = l;
                        if (can_move_heuristic(board_s, piece, init_coords, new_coords, true))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

board_state *init_board()
{
    board_state *board_s = malloc(sizeof(board_state));
    if (board_s == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board_s->board[i][j].name = ' ';
            board_s->board[i][j].color = ' ';
        }
    }
    for (int i = 0; i < 8; i++)
    {
        board_s->board[1][i].name = 'P';
        board_s->board[6][i].name = 'P';
        board_s->board[0][i].color = 'w';
        board_s->board[1][i].color = 'w';
        board_s->board[6][i].color = 'b';
        board_s->board[7][i].color = 'b';
    }
    for (int i = 0; i < 8; i = i + 7)
    {
        board_s->board[i][0].name = 'R';
        board_s->board[i][7].name = 'R';
        board_s->board[i][1].name = 'N';
        board_s->board[i][6].name = 'N';
        board_s->board[i][2].name = 'B';
        board_s->board[i][5].name = 'B';
        board_s->board[i][3].name = 'Q';
        board_s->board[i][4].name = 'K';
    }
    board_s->white_kingside_castlable = true;
    board_s->white_queenside_castlable = true;
    board_s->black_kingside_castlable = true;
    board_s->black_queenside_castlable = true;
    board_s->game_ended = false;
    board_s->black_pawn_passant = -1;
    board_s->white_pawn_passant = -1;
    board_s->fifty_move_rule = 0;
    return board_s;
}

bool can_move(board_state *board_s, piece piece, coords init_co, coords new_co, bool check_would_stop)
{
    // pre-checks to see if the move is valid
    if (new_co.x < 0 || new_co.x >= 8 || new_co.y < 0 || new_co.y >= 8)
    {
        return false;
    }
    if (get_piece(board_s->board, new_co).color == piece.color)
    {
        return false;
    }

    if (check_would_stop)
    {
        board_state *new_board_s = malloc(sizeof(board_state));
        if (new_board_s == NULL)
        {
            return false;
        }
        *new_board_s = *board_s;
        // printf("can_move: %c (%d, %d) -> (%d, %d)\n", piece.name, init_co.x, init_co.y, new_co.x, new_co.y);
        new_board_s = move_piece_forced(new_board_s, init_co, new_co);
        if (is_attacked(new_board_s, find_king(new_board_s, piece.color), piece.color, false))
        {
            return false;
        }
    }
    // printf("piece is %c of color %c\n", piece.name, piece.color);
    switch (piece.name)
    {
    case 'P':
        return can_move_pawn(board_s, piece, init_co, new_co);
    case 'R':
        return can_move_rook(board_s, piece, init_co, new_co);
    case 'N':
        return can_move_knight(board_s, piece, init_co, new_co);
    case 'B':
        return can_move_bishop(board_s, piece, init_co, new_co);
    case 'Q':
        return can_move_queen(board_s, piece, init_co, new_co);
    case 'K':
        return can_move_king(board_s, piece, init_co, new_co);
    default:
        return false;
    }
}