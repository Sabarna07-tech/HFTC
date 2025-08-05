#ifndef MARKET_FEED_H
#define MARKET_FEED_H

#include <Python.h>
#include <stddef.h>

/**
 * parse_message:
 *   buf/len → C parse → Python dict
 *
 *   Returns 0 on success (out_dict set to a new PyDict),
 *   nonzero on failure.
 */
int parse_message(const char *buf,
                  size_t len,
                  PyObject **out_dict);

#endif /* MARKET_FEED_H */
