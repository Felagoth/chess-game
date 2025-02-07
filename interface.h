#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "types.h"
#include "chess_logic.h"

int prompt_menu();
void print_board(board_state *board_s, piece selected_piece, coords init_co);
char prompt_promotion(board_state *board_s, piece piece, coords init_coords, coords new_coords);
coords piece_selection(board_state *board_s, int mode, char color);
move move_selection(board_state *board_s, piece selected_piece, coords init_co, int mode);
bool check_checkmate(board_state *board_s, char color);
void dislplay_victory(char color);
void display_draw();
void display_other_stop();
void wait_for_enter();

#endif