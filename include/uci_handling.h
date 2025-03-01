#ifndef UCI_HANDLING_H
#define UCI_HANDLING_H

#include "types.h"
#define MAX_MSG_LENGTH 32000

bool handle_uci_command(char *token);
Move get_next_move(int pipe_main_to_child[2], int pipe_child_to_main[2], char moves_history[MAX_MSG_LENGTH], int wtime, int btime);
PositionList *play_turn(int pipe_main_to_child[2], int pipe_child_to_main[2], char moves_history[MAX_MSG_LENGTH], PositionList *pos_l, int wtime, int btime);
void concatenate_moves(char moves_history[MAX_MSG_LENGTH], Move move);

void start_engine(SubProcess sp);
void stop_engine(SubProcess sp);

#endif