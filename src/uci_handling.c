#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "communication.h"
#include "types.h"
#include "chess_logic.h"
#include "uci_handling.h"

#define OUTPUT_FILE stderr

bool handle_uci_command(char *token)
{
    while (token != NULL)
    {
        if (strcmp(token, "uciok") == 0)
        {
            return true;
        }
        token = strtok(NULL, "\n");
    }
    return false;
}

Move get_next_move(int pipe_main_to_child[2], int pipe_child_to_main[2], char moves_history[MAX_MSG_LENGTH], int wtime, int btime)
{
    char answer[256] = {0};
    char message[MAX_MSG_LENGTH];
    if (strlen(moves_history) == 0)
    {
        sprintf(message, "position startpos\n");
    }
    else
    {
        sprintf(message, "position startpos moves %s\n", moves_history);
    }
    fprintf(OUTPUT_FILE, "Sending command to child: %s\n", message);
    communicate_with_child(pipe_main_to_child, pipe_child_to_main, message, answer, 0);
    char message2[256];
    sprintf(message2, "go wtime %d btime %d\n", wtime, btime);
    fprintf(OUTPUT_FILE, "Sending command to child: %s\n", message2);

    communicate_with_child(pipe_main_to_child, pipe_child_to_main, message2, answer, 1);
    fprintf(OUTPUT_FILE, "Answer: %s\n", answer);
    Move move;
    if (strcmp(answer, "bestmove (none)\n") == 0)
    {
        move.init_co = empty_coords();
        move.dest_co = empty_coords();
        move.promotion = ' ';
    }
    else
    {
        char *token = strtok(answer, " ");
        token = strtok(NULL, " ");
        move.init_co.y = token[0] - 'a';
        move.init_co.x = token[1] - '1';
        move.dest_co.y = token[2] - 'a';
        move.dest_co.x = token[3] - '1';
        if (strlen(token) == 6)
        {
            fprintf(OUTPUT_FILE, "token: %s\n", token);
            move.promotion = token[4];
        }
        else
        {
            move.promotion = ' ';
        }
    }
    return move;
}

void concatenate_moves(char moves_history[MAX_MSG_LENGTH], Move move)
{
    char move_str0[5];
    char move_str[6];
    sprintf(move_str0, "%c%c%c%c", move.init_co.y + 'a', move.init_co.x + '1', move.dest_co.y + 'a', move.dest_co.x + '1');
    if (move.promotion != ' ')
    {
        sprintf(move_str, "%s%c", move_str0, move.promotion);
    }
    else
    {
        sprintf(move_str, "%s", move_str0);
    }
    if (moves_history[0] != '\0')
    {
        strcat(moves_history, " ");
    }
    strcat(moves_history, move_str);
}

PositionList *play_turn(int pipe_main_to_child[2], int pipe_child_to_main[2], char moves_history[MAX_MSG_LENGTH], PositionList *pos_l, int wtime, int btime)
{
    Move move = get_next_move(pipe_main_to_child, pipe_child_to_main, moves_history, wtime, btime);
    if (is_empty_coords(move.init_co))
    {
        fprintf(OUTPUT_FILE, "No moves given\n");
        free_position_list(pos_l);
        return NULL;
    }
    concatenate_moves(moves_history, move);
    fprintf(OUTPUT_FILE, "moves_history: %s\n", moves_history);

    BoardState *new_board_s = malloc(sizeof(BoardState));
    if (new_board_s == NULL)
    {
        free_position_list(pos_l);
        return NULL;
    }
    *new_board_s = *pos_l->board_s;
    print_board(new_board_s);
    printf("move: %c (%d, %d) -> (%d, %d)\n", get_piece(new_board_s->board, move.init_co).name, move.init_co.x, move.init_co.y, move.dest_co.x, move.dest_co.y);
    new_board_s = move_piece(new_board_s, move);
    print_board(new_board_s);
    PositionList *new_pos_l = save_position(new_board_s, pos_l);
    free(new_board_s);
    new_board_s = new_pos_l->board_s;
    // print_position_list(new_pos_l);
    return new_pos_l;
}

void start_engine(SubProcess sp)
{
    char answer[256];
    fprintf(OUTPUT_FILE, "Sending 'uci' command to child\n");
    communicate_with_child(sp.write_pipe, sp.read_pipe, "uci\n", answer, 1);
    char *token = strtok(answer, "\n");
    if (!handle_uci_command(token))
    {
        fprintf(OUTPUT_FILE, "Error: UCI command to child 1 not handled correctly\n");
        exit(EXIT_FAILURE);
    }

    fprintf(OUTPUT_FILE, "Sending 'isready' command to child 1\n");
    communicate_with_child(sp.write_pipe, sp.read_pipe, "isready\n", answer, 1);
    if (strcmp(answer, "readyok\n") != 0)
    {
        fprintf(OUTPUT_FILE, "Error: Child 1 not ready\n");
        exit(EXIT_FAILURE);
    }
    fprintf(OUTPUT_FILE, "Sending 'ucinewgame' command to child 1\n");
    communicate_with_child(sp.write_pipe, sp.read_pipe, "ucinewgame\n", answer, 0);
}

void stop_engine(SubProcess sp)
{
    char answer[256];
    if (sp.write_pipe[0] == -1)
    {
        return;
    }
    fprintf(OUTPUT_FILE, "Sending 'quit' command to child\n");
    communicate_with_child(sp.write_pipe, sp.read_pipe, "quit\n", answer, 0);
    close(sp.write_pipe[1]);
    close(sp.read_pipe[0]);
    waitpid(sp.pid, NULL, 0);
}