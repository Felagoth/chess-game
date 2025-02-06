#ifndef GAME_MODES_H
#define GAME_MODES_H

#include <stdio.h>

#include "types.h"
#include "chess_logic.h"
#include "interface.h"

void menu(bool first_time);
void pvp();
void pva();
void semi_free();
void free_mode();
void settings();
board_state *turn(board_state *board_s, int mode, char color);

#endif