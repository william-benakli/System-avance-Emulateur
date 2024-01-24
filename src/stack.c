#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

void stack_init(stack* s) {
    s->data = malloc(STACK_SIZE * sizeof(uint16_t));
    s->size = 0;
}

void stack_push(stack* s, uint16_t value) {
    s->data[s->size] = value;
    s->size++;
    if (s->size >= STACK_SIZE) {
        perror("Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
}

uint16_t stack_pop(stack* s) {
    if (s->size == 0) {
        perror("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }

    uint16_t value = s->data[s->size - 1];
    s->size--;
    return value;
}
