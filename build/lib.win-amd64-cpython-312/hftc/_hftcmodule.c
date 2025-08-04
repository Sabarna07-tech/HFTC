#include <Python.h>

// Example: add two ints as a hot path
static PyObject *hftc_add(PyObject *self, PyObject *args)
{
    int a, b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return NULL;
    return PyLong_FromLong(a + b);
}

static PyMethodDef HftcMethods[] = {
    {"add", hftc_add, METH_VARARGS, "Add two integers."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef hftcmodule = {
    PyModuleDef_HEAD_INIT,
    "_hftcmodule",
    "HFTC C Extension Module",
    -1,
    HftcMethods};

PyMODINIT_FUNC PyInit__hftcmodule(void)
{
    return PyModule_Create(&hftcmodule);
}