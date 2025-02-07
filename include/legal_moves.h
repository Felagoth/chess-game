#ifndef LEGAL_MOVES_H
#define LEGAL_MOVES_H

#include <stdbool.h>
#include "types.h"

bool can_move_pawn(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);
bool can_move_knight(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);
bool can_move_rook(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);
bool can_move_bishop(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);
bool can_move_queen(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);
bool can_move_king(board_state *board_s, piece selected_piece, coords init_co, coords dest_co);

#endif