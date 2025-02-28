#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef struct
{
    char name;  // name of the piece
    char color; // color of the piece ('w' for white, 'b' for black, ' ' for empty)
} Piece;

typedef struct
{
    int x;
    int y;
} Coords;

typedef struct
{
    Coords init_co;
    Coords dest_co;
} Move;

typedef struct
{
    Piece board[8][8];
    bool white_kingside_castlable; // true if white can castle
    bool white_queenside_castlable;
    bool black_kingside_castlable;
    bool black_queenside_castlable;
    bool game_ended;
    int black_pawn_passant; // -1 if no pawn can be taken en passant, otherwise the column of the pawn
    int white_pawn_passant;
    int fifty_move_rule;
} BoardState;

typedef struct position_list
{
    BoardState *board_s;
    struct position_list *tail;
} PositionList;

#endif