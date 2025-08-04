#ifndef BUFFER_RING_H
#define BUFFER_RING_H
#include <stdint.h>
#define RING_SIZE 1024 // power of two

typedef struct
{
    uint64_t head;
    uint64_t tail;
    void *entries[RING_SIZE];
} ring_buffer_t;

void ring_init(ring_buffer_t *rb);
int ring_push(ring_buffer_t *rb, void *item);
void *ring_pop(ring_buffer_t *rb);
#endif