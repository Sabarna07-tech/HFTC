#include <Python.h>
#include "buffer_ring.h" // lock-free ring buffer

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

// -- Ring-buffer wrappers ----------------------------------------------------
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
    int res = ring_push(&global_ring, obj);
    if (res)
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

// -- Method table ------------------------------------------------------------
static PyMethodDef HftcMethods[] = {
    {"add", hftc_add, METH_VARARGS, "Add two integers."},
    {"ring_init", hftc_ring_init, METH_NOARGS, "Initialize global ring buffer."},
    {"ring_push", hftc_ring_push, METH_VARARGS, "Push object onto ring buffer."},
    {"ring_pop", hftc_ring_pop, METH_NOARGS, "Pop object from ring buffer (or None)."},
    {NULL, NULL, 0, NULL}};

// -- Module definition --------------------------------------------------------
static struct PyModuleDef hftcmodule = {
    PyModuleDef_HEAD_INIT,
    "_hftcmodule",
    "HFTC C Extension Module",
    -1,
    HftcMethods};

PyMODINIT_FUNC PyInit__hftcmodule(void)
{
    // Initialize ring buffer on module import
    ring_init(&global_ring);
    return PyModule_Create(&hftcmodule);
}