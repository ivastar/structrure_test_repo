#include<Python.h>
#include<numpy/arrayobject.h>
#include<math.h>
#include<omp.h>

#define IND(a,i) *((double *)(a->data+i*a->strides[0]))

static PyObject *reqramp(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *reqramp(PyObject *self, PyObject *args, PyObject *keywds)
{
  PyObject *etc;
  PyArrayObject *x,*y, *rampparams;
  double goal,m,x0,a,b,c,x1;
  int i;
  npy_intp dims[1];

  //  etc = PyList_New(0);

  static char *kwlist[] = {"rampparams","x","etc",NULL};

  if(!PyArg_ParseTupleAndKeywords(args,keywds,"OO|O",kwlist,&rampparams,&x,&etc))
    {
      return NULL;
    }

  goal = IND(rampparams,0);
  m    = IND(rampparams,1);
  x0   = IND(rampparams,2);
  a    = IND(rampparams,3);
  b    = IND(rampparams,4);
  c    = IND(rampparams,5);
  x1   = IND(rampparams,6);

  dims[0] = x->dimensions[0];

  y = (PyArrayObject *) PyArray_SimpleNew(1,dims,PyArray_DOUBLE);
  #pragma omp parallel for
  for(i=0;i<dims[0];i++)
    {
      IND(y,i) = goal*(1-exp(-1*m*(IND(x,i)-x0)))+a*pow((IND(x,i)-x1),2)	\
						       +b*(IND(x,i)-x1)+c;
    }
  return PyArray_Return(y);
}

static PyMethodDef module_methods[] = {
  {"reqramp",(PyCFunction)reqramp,METH_VARARGS|METH_KEYWORDS},{NULL}};

static char module_docstring[] =
    "This module is used to calcuate the reqramp";

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
    PyInit_reqramp(void)
#else
    initreqramp(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module;
    static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "reqramp",             /* m_name */
    module_docstring,    /* m_doc */
    -1,                  /* m_size */
    module_methods,      /* m_methods */
    NULL,                /* m_reload */
    NULL,                /* m_traverse */
    NULL,                /* m_clear */
    NULL,                /* m_free */
    };
#endif

#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&moduledef);
    if (!module)
    return NULL;
    /* Load `numpy` functionality. */
    import_array();
    return module;
#else
    PyObject *m = Py_InitModule3("reqramp", module_methods, module_docstring);
    if (m == NULL)
    return;
    /* Load `numpy` functionality. */
    import_array();
#endif
}
