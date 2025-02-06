#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef struct piece
{
    char name;  // name of the piece
    char color; // color of the piece ('w' for white, 'b' for black, ' ' for empty)
} piece;

typedef struct coords
{
    int x;
    int y;
} coords;

typedef struct move
{
    coords init_co;
    coords dest_co;
} move;

typedef struct board_state
{
    piece board[8][8];
    bool white_kingside_castlable; // true if white can castle
    bool white_queenside_castlable;
    bool black_kingside_castlable;
    bool black_queenside_castlable;
    bool game_ended;
    int black_pawn_passant; // -1 if no pawn can be taken en passant, otherwise the column of the pawn
    int white_pawn_passant;
    int number_pos_repet; // just for fun
} board_state;

#endif