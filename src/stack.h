#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define STACK_SIZE 100

struct stack {
    uint16_t* data;
    int size;
};
typedef struct stack stack;

void stack_init(stack* s);
void stack_push(stack* s, uint16_t value);
uint16_t stack_pop(stack* s);

#endif
