#ifndef BUFFER_RING_H
#define BUFFER_RING_H

// ring size must be a compile-time constant
#define RING_SIZE 1024 /* power of two for mask wrapping */

#ifdef _MSC_VER
#include <Windows.h>
typedef struct
{
    volatile LONG64 head;
    volatile LONG64 tail;
    void *entries[RING_SIZE];
} ring_buffer_t;
// Win32 interlocked operations:
#define atomic_store(p, v) InterlockedExchange64((volatile LONG64 *)(p), (v))
#define atomic_load(p) InterlockedCompareExchange64((volatile LONG64 *)(p), 0, 0)
#else
#include <stdatomic.h>
typedef struct
{
    atomic_uint_least64_t head;
    atomic_uint_least64_t tail;
    void *entries[RING_SIZE];
} ring_buffer_t;
#define atomic_store(p, v) atomic_store_explicit(p, v, memory_order_seq_cst)
#define atomic_load(p) atomic_load_explicit(p, memory_order_seq_cst)
#endif

void ring_init(ring_buffer_t *rb);
int ring_push(ring_buffer_t *rb, void *item);
void *ring_pop(ring_buffer_t *rb);

#endif /* BUFFER_RING_H */
