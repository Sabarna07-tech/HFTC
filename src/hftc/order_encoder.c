// src/hftc/order_encoder.c

#include "order_encoder.h"
#include <string.h>

/*
 * Cross-platform manual big-endian packing:
 *   - 8-byte cl_ord_id
 *   - 4-byte symbol_id
 *   - 1-byte side + 3-byte padding
 *   - 8-byte price (host-order)
 *   - 4-byte quantity
 */

int encode_order(const order_t *order,
                 char *out_buf,
                 size_t buf_len,
                 size_t *out_len)
{
    const size_t needed = 8 + 4 + 1 + 3 + 8 + 4;
    if (buf_len < needed)
        return -1;

    size_t off = 0;

    // 8-byte cl_ord_id, big-endian
    uint64_t v64 = order->cl_ord_id;
    for (int i = 7; i >= 0; --i)
    {
        out_buf[off + i] = (char)(v64 & 0xFF);
        v64 >>= 8;
    }
    off += 8;

    // 4-byte symbol_id, big-endian
    uint32_t v32 = order->symbol_id;
    for (int i = 3; i >= 0; --i)
    {
        out_buf[off + i] = (char)(v32 & 0xFF);
        v32 >>= 8;
    }
    off += 4;

    // side + 3 bytes padding
    out_buf[off++] = order->side;
    memset(out_buf + off, 0, 3);
    off += 3;

    // 8-byte price, host-order (IEEE-754 double)
    memcpy(out_buf + off, &order->price, sizeof(order->price));
    off += sizeof(order->price);

    // 4-byte quantity, big-endian
    v32 = order->quantity;
    for (int i = 3; i >= 0; --i)
    {
        out_buf[off + i] = (char)(v32 & 0xFF);
        v32 >>= 8;
    }
    off += 4;

    *out_len = off;
    return 0;
}
