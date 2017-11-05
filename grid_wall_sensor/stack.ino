/*
 * C program to implement stack. Stack is a LIFO data structure.
 * Stack operations: PUSH(insert operation), POP(Delete operation)
 * and Display stack.
 */
#include "stack.h"

void s_init(struct stack* s){
  s->top = -1;
}

/*  Function to add an element to the stack */
void s_push (struct stack* s, struct xy_pair xy_pair)
{
    if (s->top == (MAXSIZE - 1))
    {
        Serial.println ("Stack is Full\n");
        return;
    }
    else
    {
        s->top = s->top + 1;
        s->stk[s->top] = xy_pair;
    }
    return;
}

/*  Function to delete an element from the stack */
struct xy_pair s_pop (struct stack* s)
{
    struct xy_pair xy_pair;
    xy_pair.x = -1;
    xy_pair.y = -1;
    if (s->top == - 1)
    {
        printf ("Stack is Empty\n");
        return xy_pair;
    }
    else
    {
        xy_pair = s->stk[s->top];
        s->top = s->top - 1;
    }
    return xy_pair;
}

uint8_t s_contains(struct stack* s, xy_pair xy_pair){
  for (uint8_t i = s->top; i > -1; i--)
    if ( s->stk[i].x == xy_pair.x && s->stk[i].y == xy_pair.y)
      return 1;
  return 0; 
}

uint8_t s_isEmpty(struct stack* s){
  return s->top == -1;
}

