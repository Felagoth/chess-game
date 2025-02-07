
#include <stdio.h>

#include "types.h"
#include "chess_logic.h"

int prompt_menu()
{
    int mode;
    printf("\n\n\n\n\nWelcome to Felagoth's chess game!\n\nIn what mode do you want to play?\n\t1. Player vs Player\n\t2. Player vs AI (available soon)\n\t3. semi free mode\n\t4. free mode\n\t5. settings\n\t0. leave game\n");
    scanf(" %d", &mode);
    return mode;
}

void print_board(board_state *board_s, piece selected_piece, coords init_co)
{
    printf("\n");
    coords dest_co;
    for (int i = 7; i >= 0; i--)
    {
        printf("  +---+---+---+---+---+---+---+---+\n");
        printf("%d ", i + 1);
        for (int j = 0; j < 8; j++)
        {
            dest_co.x = i;
            dest_co.y = j;
            char label_piece = board_s->board[i][j].name;
            if (board_s->board[i][j].color == 'b')
            {
                label_piece = label_piece + 32;
            }
            if (can_move_heuristic(board_s, selected_piece, init_co, dest_co, true))
            {

                printf("|<%c>", label_piece);
            }
            else
            {
                printf("| %c ", label_piece);
            }
        }
        printf("|\n");
    }
    printf("  +---+---+---+---+---+---+---+---+\n");
    printf("    a   b   c   d   e   f   g   h\n");
}

char prompt_promotion(board_state *board_s, piece piece, coords init_coords, coords new_coords)
{
    char promotion = ' ';
    while (promotion != 'Q' && promotion != 'R' && promotion != 'N' && promotion != 'B')
    {
        printf("Enter the piece you want to promote to (Q, R, N, B): ");
        scanf(" %c", &promotion);
        if (promotion != 'Q' && promotion != 'R' && promotion != 'N' && promotion != 'B')
        {
            printf("Invalid piece\n");
        }
    }
    printf("Promoting pawn to %c\n", promotion);
    return promotion;
}

coords piece_selection(board_state *board_s, int mode, char color)
{
    piece selected_piece = empty_piece();
    coords init_co = empty_coords();
    char init_column;
    int init_row;
    while (is_empty(selected_piece))
    {
        // piece selection
        printf("Enter the coordinates of the piece you want to move: ");
        scanf(" %c%d", &init_column, &init_row);
        if (init_column == 'q')
        {
            return empty_coords();
        }
        init_co.x = init_row - 1;
        init_co.y = (int)(init_column - 'a');

        // check if the coordinates are valid
        if (init_co.x < 0 || init_co.x >= 8 || init_co.y < 0 || init_co.y >= 8)
        {
            printf("Invalid coordinates\n");
        }
        // check if the piece selected exists and is of the right color
        else
        {
            selected_piece = get_piece(board_s->board, init_co);
            if (selected_piece.color == ' ')
            {
                printf("Invalid piece\n");
            }
            else if (selected_piece.color != color && mode == 1)
            {
                printf("Invalid piece color\n");
                selected_piece = empty_piece();
            }
        }
        // print the board with the selected piece
        if (!is_empty(selected_piece))
        {
            print_board(board_s, selected_piece, init_co);
            printf("selected piece: %c (%c%d)\n", selected_piece.name, init_column, init_row);
        }
    }
    return init_co;
}

move move_selection(board_state *board_s, piece selected_piece, coords init_co, int mode)
{
    move selected_move = empty_move();
    coords dest_co = empty_coords();
    piece dest_piece = empty_piece();
    char dest_column;
    int dest_row;

    // move selection
    printf("Enter the coordinates of the destination: ");
    scanf(" %c%d", &dest_column, &dest_row);
    if (dest_column == 'q')
    {
        return empty_move();
    }
    dest_co.x = dest_row - 1;
    dest_co.y = (int)(dest_column - 'a');
    selected_move.init_co = init_co;
    selected_move.dest_co = dest_co;

    // check if the coordinates are valid else redo move selection
    if (init_co.x < 0 || init_co.x >= 8 || init_co.y < 0 || init_co.y >= 8 || dest_co.x < 0 || dest_co.x >= 8 || dest_co.y < 0 || dest_co.y >= 8)
    {
        printf("Invalid coordinates\n");
        selected_move = move_selection(board_s, selected_piece, init_co, mode);
    }
    // check if the piece selected is of the same color as the player, if so, redo move selection with the new piece
    else
    {
        dest_piece = get_piece(board_s->board, dest_co);
        if (selected_piece.color == dest_piece.color)
        {
            print_board(board_s, dest_piece, dest_co);
            printf("you selected another piece\n");
            printf("selected piece: %c (%c%d)\n", dest_piece.name, dest_column, dest_row);
            selected_move = move_selection(board_s, dest_piece, dest_co, mode);
            selected_piece = dest_piece;
        }
    }

    // if the move is invalid, redo move selection
    if (!can_move(board_s, selected_piece, selected_move.init_co, selected_move.dest_co, true) && mode != 4)
    {
        printf("%c (%d, %d)->(%d, %d)\n", selected_piece.name, selected_move.init_co.x, selected_move.init_co.y, selected_move.dest_co.x, selected_move.dest_co.y);
        printf("Invalid move\n");
        selected_move = move_selection(board_s, selected_piece, init_co, mode);
    }
    // printf("%c (%d, %d)->(%d, %d)\n", selected_piece.name, selected_move.init_co.x, selected_move.init_co.y, selected_move.dest_co.x, selected_move.dest_co.y);
    return selected_move;
}

bool check_checkmate(board_state *board_s, char color)
{
    if (is_check(board_s, color))
    {
        printf("Check!\n");
        if (is_checkmate(board_s, color))
        {
            printf("Checkmate!\n");
            return true;
        }
    }
    return false;
}

void dislplay_victory(char color)
{
    if (color == 'w')
    {
        printf("\nVictory for white\n");
    }
    else
    {
        printf("\nVictory for black\n");
    }
    fflush(stdout);
    getchar();
}

void display_draw()
{
    printf("\nDraw\n");
    fflush(stdout);
    getchar();
}

void display_other_stop()
{
    printf("\nThe game stopped before true end\n");
    fflush(stdout);
    getchar();
}

void wait_for_enter()
{
    printf("Press [Enter] to continue . . .");
    fflush(stdout);
    getchar();
}