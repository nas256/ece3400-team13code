#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include "mapper.h"
#define MAXSIZE 30
 
struct stack
{
    struct xy_pair stk[MAXSIZE];
    int top;
};
typedef struct stack STACK;

STACK missed_op; // DFS stack
STACK path;
STACK visited; // cause im lazy and don't want to make a list

void s_init(struct stack* s);
void s_push(struct stack* s, struct xy_pair xy_pair);
struct xy_pair s_pop(struct stack* s);
uint8_t s_contains(struct stack* s, struct xy_pair xy_pair);
uint8_t s_isEmpty(struct stack* s);
 
#endif
