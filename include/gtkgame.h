#ifndef GTKGAME_H
#define GTKGAME_H

#include <stdio.h>
#include <gtk/gtk.h>
#include "types.h"

void start();
void menugtk(GtkApplication *app, GtkWidget *window);
void pvp(GtkApplication *app, GtkWidget *window);
void pva(GtkApplication *app, GtkWidget *window);
void semi_free(GtkApplication *app, GtkWidget *window);
void free_mode(GtkApplication *app, GtkWidget *window);
void settings(GtkApplication *app);
char prompt_promotion(BoardState *board_state, Piece move_piece, Coords init_coords, Coords new_coords);
void display_draw(GtkWidget *window);
void display_victory(char color, GtkWidget *window);

#endif