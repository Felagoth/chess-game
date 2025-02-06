
#include <stdio.h>
#include <stdlib.h>

#include "game_logic.h"
#include "types.h"
#include "chess_logic.h"
#include "interface.h"

void menu()
{
    int mode = prompt_menu();
    if (mode == 0)
    {
        return;
    }
    if (mode == 1)
    {
        pvp();
    }
    else if (mode == 2)
    {
        pva();
    }
    else if (mode == 3)
    {
        semi_free();
    }
    else if (mode == 4)
    {
        free_mode();
    }
    else if (mode == 5)
    {
        settings();
    }
    else
    {
        printf("Invalid mode\n");
        menu(false);
    }
}

void pvp()
{
    board_state *board_s = init_board();
    int mode = 1;
    char color = 'w';
    print_board(board_s, empty_piece(), empty_coords());
    while (!board_s->game_ended)
    {
        // move selection
        board_s = turn(board_s, mode, color);
        if (board_s == NULL)
        {
            return;
        }
        // end of turn
        if (color == 'w')
        {
            if (check_checkmate(board_s, 'b'))
            {
                end_game(board_s, 'w');
                return;
            }
            color = 'b';
            printf("black turn\n");
        }
        else
        {
            if (check_checkmate(board_s, 'w'))
            {
                end_game(board_s, 'b');
                return;
            }
            color = 'w';
            printf("white turn\n");
        }
    }
    free(board_s);
    menu(true);
}

void pva()
{
    printf("pva available soon\n");
}

void semi_free()
{
    board_state *board_s = init_board();
    int mode = 3;
    char color = ' ';
    print_board(board_s, empty_piece(), empty_coords());
    while (!board_s->game_ended)
    {
        board_s = turn(board_s, mode, color);
        if (board_s == NULL)
        {
            return;
        }
        // check for checkmate
        if (check_checkmate(board_s, 'w'))
        {
            dislplay_victory('b');
            free(board_s);
            menu(true);
            return;
        }
        else if (check_checkmate(board_s, 'b'))
        {
            dislplay_victory('w');
            free(board_s);
            menu(true);
            return;
        }
    }
    free(board_s);
    menu(true);
}

void free_mode()
{
    board_state *board_s = init_board();
    int mode = 4;
    char color = ' ';
    print_board(board_s, empty_piece(), empty_coords());
    while (!board_s->game_ended)
    {
        board_s = turn(board_s, mode, color);
        if (board_s == NULL)
        {
            return;
        }
        // check for checkmate
        if (check_checkmate(board_s, 'w'))
        {
            dislplay_victory('w');
            return;
        }
        else if (check_checkmate(board_s, 'b'))
        {
            dislplay_victory('b');
            return;
        }
    }
    free(board_s);
    menu(true);
}

void settings()
{
    printf("settings available soon\n");
    menu(true);
}

board_state *turn(board_state *board_s, int mode, char color)
{
    piece selected_piece = empty_piece();
    coords init_co = empty_coords();
    move selected_move = empty_move();
    // piece selection
    init_co = piece_selection(board_s, mode, color);
    if (is_empty_coords(init_co))
    {
        end_game(board_s, ' ');
        return NULL;
    }
    selected_piece = get_piece(board_s->board, init_co);
    // move selection
    selected_move = move_selection(board_s, selected_piece, init_co, mode);
    if (is_empty_move(selected_move))
    {
        end_game(board_s, ' ');
        return NULL;
    }
    // move piece
    board_s = move_piece(board_s, selected_move.init_co, selected_move.dest_co);
    selected_piece = empty_piece();
    print_board(board_s, selected_piece, init_co);
    return board_s;
}

/*
 * end_game: end the game and display the winner
 * @param board_s: the board state
 * @param color_winner: the color of the winner, '=' if draw, ' ' otherwise
 */
void end_game(board_state *board_s, char color_winner)
{
    if (color_winner != ' ')
    {
        dislplay_victory(color_winner);
    }
    else if (color_winner == '=')
    {
        display_draw();
    }
    else
    {
        display_other_stop();
    }
    board_s->game_ended = true;
    free(board_s);
    wait_for_enter();
    menu();
}