#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H

#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "legal_moves.h"

board_state *init_board();

piece empty_piece();
coords empty_coords();
move empty_move();
bool is_empty(piece piece);
bool is_empty_coords(coords coords);
bool is_empty_move(move move);
piece get_piece(piece board[8][8], coords coords);
bool can_move_heuristic(board_state *board_s, piece piece, coords init_co, coords new_co, bool check_would_stop);
bool is_attacked(board_state *board_s, coords co, char color, bool check_would_stop);
bool is_check(board_state *board_s, char color);
bool is_checkmate(board_state *board_s, char color);

board_state *move_pawn_handling(board_state *board_s, piece move_piece, piece dest_piece, coords init_coords, coords new_coords);
board_state *move_king_handling(board_state *board_s, piece move_piece, coords init_coords, coords new_coords);
board_state *move_rook_handling(board_state *board_s, piece move_piece, coords init_coords, coords new_coords);
board_state *move_piece(board_state *board_s, coords init_coords, coords new_coords);

bool can_move(board_state *board_s, piece piece, coords init_co, coords new_co, bool check_would_stop);

#endif