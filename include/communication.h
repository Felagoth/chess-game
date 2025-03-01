#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "types.h"

char *communicate_with_child(int pipe_main_to_child[2], int pipe_child_to_main[2], const char *message, char answer[256], int expects_answer);
SubProcess gen_process();
SubProcess empty_process();

#endif
