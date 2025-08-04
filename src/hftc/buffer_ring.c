#include <stdint.h>
#include "buffer_ring.h"
void ring_init(ring_buffer_t *rb)
{
    atomic_store(&rb->head, 0);
    atomic_store(&rb->tail, 0);
}

int ring_push(ring_buffer_t *rb, void *item)
{
    uint64_t tail = (uint64_t)atomic_load(&rb->tail);
    uint64_t head = (uint64_t)atomic_load(&rb->head);
    uint64_t next = tail + 1;

    /* full when next - head > RING_SIZE */
    if ((next - head) > RING_SIZE)
        return -1;

    rb->entries[tail & (RING_SIZE - 1)] = item;
    atomic_store(&rb->tail, next);
    return 0;
}

void *ring_pop(ring_buffer_t *rb)
{
    uint64_t head = (uint64_t)atomic_load(&rb->head);
    uint64_t tail = (uint64_t)atomic_load(&rb->tail);

    /* empty when head == tail */
    if (head == tail)
        return NULL;

    void *item = rb->entries[head & (RING_SIZE - 1)];
    atomic_store(&rb->head, head + 1);
    return item;
}
