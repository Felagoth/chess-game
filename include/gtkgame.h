#ifndef GTKGAME_H
#define GTKGAME_H

#include <stdio.h>
#include <gtk/gtk.h>
#include "types.h"

void menu();
void play1(GtkApplication *app, GtkWidget *window);
void menugtk(GtkApplication *app, GtkWidget *window);
void pvp(GtkApplication *app);
void pva(GtkApplication *app);
void semi_free(GtkApplication *app);
void free_mode(GtkApplication *app);
void settings(GtkApplication *app);
board_state *turn(board_state *board_s, int mode, char color);
void end_game(board_state *board_s, char color_winner);

#endif