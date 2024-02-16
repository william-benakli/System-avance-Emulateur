#include "stack.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void stack_init(stack* s) {
    s->size = 0;
}

void stack_push(stack* s, uint16_t value) {
    s->data[s->size] = value;
    s->size++;
    if (s->size >= STACK_SIZE) {
        errno = EOVERFLOW;
        perror("Stack overflow");
        exit(EXIT_FAILURE);
    }
}

uint16_t stack_pop(stack* s) {
    if (s->size == 0) {
        errno = EINVAL;
        perror("Stack underflow");
        exit(EXIT_FAILURE);
    }

    uint16_t value = s->data[s->size - 1];
    s->size--;
    return value;
}

void stack_clear(stack* s) {
    for (size_t i = 0; i < s->size; i++) {
        s->data[i] = 0;
    }
    s->size = 0;
}
