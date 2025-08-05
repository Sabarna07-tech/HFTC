#include "market_feed.h"
#include <stdint.h>
#include <string.h>

int parse_message(const char *buf,
                  size_t len,
                  PyObject **out_dict)
{
    // our “binary format” = [uint64][double][uint32]
    if (len < sizeof(uint64_t) + sizeof(double) + sizeof(uint32_t))
    {
        return -1;
    }

    // unpack
    uint64_t seqnum;
    memcpy(&seqnum, buf, sizeof(seqnum));

    double price;
    memcpy(&price, buf + sizeof(seqnum), sizeof(price));

    uint32_t size;
    memcpy(&size,
           buf + sizeof(seqnum) + sizeof(price),
           sizeof(size));

    // build Python dict
    PyObject *d = PyDict_New();
    if (!d)
        return -1;

    PyDict_SetItemString(d,
                         "seqnum",
                         PyLong_FromUnsignedLongLong(seqnum));
    PyDict_SetItemString(d,
                         "price",
                         PyFloat_FromDouble(price));
    PyDict_SetItemString(d,
                         "size",
                         PyLong_FromUnsignedLong(size));

    *out_dict = d;
    return 0;
}
