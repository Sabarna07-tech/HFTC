#ifndef ORDER_ENCODER_H
#define ORDER_ENCODER_H

#include <stddef.h>
#include <stdint.h>

/**
 * A simple native order format:
 *  8 bytes  client order ID (big-endian)
 *  4 bytes  symbol ID       (big-endian)
 *  1 byte   side ('B'/'S')
 *  3 bytes  padding
 *  8 bytes  price (IEEE-754 double, network order assumed little-endian host)
 *  4 bytes  quantity        (big-endian)
 */
typedef struct
{
    uint64_t cl_ord_id;
    uint32_t symbol_id;
    char side;
    double price;
    uint32_t quantity;
} order_t;

/**
 * encode_order:
 *   in:  order pointer
 *        out_buf of at least 28 bytes
 *   out: out_len set to number of bytes written
 * returns 0 on success, nonzero if buf_len too small.
 */
int encode_order(const order_t *order, char *out_buf, size_t buf_len, size_t *out_len);

#endif /* ORDER_ENCODER_H */
