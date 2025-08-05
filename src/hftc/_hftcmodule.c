#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "buffer_ring.h"   // lock-free ring buffer
#include "market_feed.h"   // market-data parser API
#include "order_encoder.h" // order encoder API

// Global ring buffer instance
static ring_buffer_t global_ring;

// -- Basic add function (hot path) ------------------------------------------
static PyObject *hftc_add(PyObject *self, PyObject *args)
{
    int a, b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return NULL;
    return PyLong_FromLong(a + b);
}

// -- Ring-buffer wrappers ---------------------------------------------------
static PyObject *hftc_ring_init(PyObject *self, PyObject *args)
{
    ring_init(&global_ring);
    Py_RETURN_NONE;
}

static PyObject *hftc_ring_push(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;
    Py_INCREF(obj);
    if (ring_push(&global_ring, obj))
    {
        PyErr_SetString(PyExc_BufferError, "Ring buffer full");
        Py_DECREF(obj);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *hftc_ring_pop(PyObject *self, PyObject *args)
{
    void *item = ring_pop(&global_ring);
    if (!item)
    {
        Py_RETURN_NONE;
    }
    return (PyObject *)item;
}

// -- Market-data parser wrapper --------------------------------------------
static PyObject *hftc_parse_message(PyObject *self, PyObject *args)
{
    const char *buf;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "y#", &buf, &len))
        return NULL;

    PyObject *out_dict = NULL;
    int rc = parse_message(buf, (size_t)len, &out_dict);

    if (rc != 0)
    {
        PyErr_SetString(PyExc_ValueError, "failed to parse market message");
        return NULL;
    }
    return out_dict;
}

// -- encode_order wrapper --------------------------------------------------
static PyObject *hftc_encode_order(PyObject *self, PyObject *args)
{
    unsigned long long cl_ord_id;
    unsigned int symbol_id;
    const char *side_str;
    Py_ssize_t side_len;
    double price;
    unsigned int quantity;

    if (!PyArg_ParseTuple(args, "KIs#dI",
                          &cl_ord_id,
                          &symbol_id,
                          &side_str, &side_len,
                          &price,
                          &quantity))
        return NULL;

    if (side_len != 1)
    {
        PyErr_SetString(PyExc_ValueError, "side must be a single character");
        return NULL;
    }
    char side = side_str[0];

    order_t order = {cl_ord_id, symbol_id, side, price, quantity};
    char buf[28];
    size_t out_len;
    if (encode_order(&order, buf, sizeof(buf), &out_len) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "encode_order failed");
        return NULL;
    }

    return PyBytes_FromStringAndSize(buf, (Py_ssize_t)out_len);
}

// -- Method table -----------------------------------------------------------
static PyMethodDef HftcMethods[] = {
    {"add", hftc_add, METH_VARARGS, "Add two ints."},
    {"ring_init", hftc_ring_init, METH_NOARGS, "Init ring buffer."},
    {"ring_push", hftc_ring_push, METH_VARARGS, "Push object."},
    {"ring_pop", hftc_ring_pop, METH_NOARGS, "Pop object."},
    {"parse_message", hftc_parse_message, METH_VARARGS, "Parse market msg."},
    {"encode_order", hftc_encode_order, METH_VARARGS, "Encode order to bytes."},
    {NULL, NULL, 0, NULL}};

// -- Module definition ------------------------------------------------------
static struct PyModuleDef hftcmodule = {
    PyModuleDef_HEAD_INIT,
    "_hftcmodule",
    "HFTC C Extension Module",
    -1,
    HftcMethods};

PyMODINIT_FUNC PyInit__hftcmodule(void)
{
    ring_init(&global_ring);
    return PyModule_Create(&hftcmodule);
}
