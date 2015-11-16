/* File: _fentmodule.c
 * This file is auto-generated with f2py (version:2).
 * f2py is a Fortran to Python Interface Generator (FPIG), Second Edition,
 * written by Pearu Peterson <pearu@cens.ioc.ee>.
 * See http://cens.ioc.ee/projects/f2py2e/
 * Generation date: Sun Nov 15 18:54:55 2015
 * $Revision:$
 * $Date:$
 * Do not edit this file directly unless you know what you are doing!!!
 */

#ifdef __cplusplus
extern "C" {
#endif

/*********************** See f2py2e/cfuncs.py: includes ***********************/
#include "Python.h"
#include <stdarg.h>
#include "fortranobject.h"
#include <math.h>

/**************** See f2py2e/rules.py: mod_rules['modulebody'] ****************/
static PyObject *_fent_error;
static PyObject *_fent_module;

/*********************** See f2py2e/cfuncs.py: typedefs ***********************/
#ifdef _WIN32
typedef __int64 long_long;
#else
typedef long long long_long;
typedef unsigned long long unsigned_long_long;
#endif


/****************** See f2py2e/cfuncs.py: typedefs_generated ******************/
/*need_typedefs_generated*/

/********************** See f2py2e/cfuncs.py: cppmacros **********************/
#if defined(PREPEND_FORTRAN)
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F
#else
#define F_FUNC(f,F) _##f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F##_
#else
#define F_FUNC(f,F) _##f##_
#endif
#endif
#else
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F
#else
#define F_FUNC(f,F) f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F##_
#else
#define F_FUNC(f,F) f##_
#endif
#endif
#endif
#if defined(UNDERSCORE_G77)
#define F_FUNC_US(f,F) F_FUNC(f##_,F##_)
#else
#define F_FUNC_US(f,F) F_FUNC(f,F)
#endif

#define rank(var) var ## _Rank
#define shape(var,dim) var ## _Dims[dim]
#define old_rank(var) (PyArray_NDIM((PyArrayObject *)(capi_ ## var ## _tmp)))
#define old_shape(var,dim) PyArray_DIM(((PyArrayObject *)(capi_ ## var ## _tmp)),dim)
#define fshape(var,dim) shape(var,rank(var)-dim-1)
#define len(var) shape(var,0)
#define flen(var) fshape(var,0)
#define old_size(var) PyArray_SIZE((PyArrayObject *)(capi_ ## var ## _tmp))
/* #define index(i) capi_i ## i */
#define slen(var) capi_ ## var ## _len
#define size(var, ...) f2py_size((PyArrayObject *)(capi_ ## var ## _tmp), ## __VA_ARGS__, -1)

#define CHECKSCALAR(check,tcheck,name,show,var)\
  if (!(check)) {\
    char errstring[256];\
    sprintf(errstring, "%s: "show, "("tcheck") failed for "name, var);\
    PyErr_SetString(_fent_error,errstring);\
    /*goto capi_fail;*/\
  } else 
#ifdef DEBUGCFUNCS
#define CFUNCSMESS(mess) fprintf(stderr,"debug-capi:"mess);
#define CFUNCSMESSPY(mess,obj) CFUNCSMESS(mess) \
  PyObject_Print((PyObject *)obj,stderr,Py_PRINT_RAW);\
  fprintf(stderr,"\n");
#else
#define CFUNCSMESS(mess)
#define CFUNCSMESSPY(mess,obj)
#endif

#ifndef max
#define max(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a < b) ? (a) : (b))
#endif


/************************ See f2py2e/cfuncs.py: cfuncs ************************/
static int f2py_size(PyArrayObject* var, ...)
{
  npy_int sz = 0;
  npy_int dim;
  npy_int rank;
  va_list argp;
  va_start(argp, var);
  dim = va_arg(argp, npy_int);
  if (dim==-1)
    {
      sz = PyArray_SIZE(var);
    }
  else
    {
      rank = PyArray_NDIM(var);
      if (dim>=1 && dim<=rank)
        sz = PyArray_DIM(var, dim-1);
      else
        fprintf(stderr, "f2py_size: 2nd argument value=%d fails to satisfy 1<=value<=%d. Result will be 0.\n", dim, rank);
    }
  va_end(argp);
  return sz;
}

static int int_from_pyobj(int* v,PyObject *obj,const char *errmess) {
  PyObject* tmp = NULL;
  if (PyInt_Check(obj)) {
    *v = (int)PyInt_AS_LONG(obj);
    return 1;
  }
  tmp = PyNumber_Int(obj);
  if (tmp) {
    *v = PyInt_AS_LONG(tmp);
    Py_DECREF(tmp);
    return 1;
  }
  if (PyComplex_Check(obj))
    tmp = PyObject_GetAttrString(obj,"real");
  else if (PyString_Check(obj) || PyUnicode_Check(obj))
    /*pass*/;
  else if (PySequence_Check(obj))
    tmp = PySequence_GetItem(obj,0);
  if (tmp) {
    PyErr_Clear();
    if (int_from_pyobj(v,tmp,errmess)) {Py_DECREF(tmp); return 1;}
    Py_DECREF(tmp);
  }
  {
    PyObject* err = PyErr_Occurred();
    if (err==NULL) err = _fent_error;
    PyErr_SetString(err,errmess);
  }
  return 0;
}

static int long_long_from_pyobj(long_long* v,PyObject *obj,const char *errmess) {
  PyObject* tmp = NULL;
  if (PyLong_Check(obj)) {
    *v = PyLong_AsLongLong(obj);
    return (!PyErr_Occurred());
  }
  if (PyInt_Check(obj)) {
    *v = (long_long)PyInt_AS_LONG(obj);
    return 1;
  }
  tmp = PyNumber_Long(obj);
  if (tmp) {
    *v = PyLong_AsLongLong(tmp);
    Py_DECREF(tmp);
    return (!PyErr_Occurred());
  }
  if (PyComplex_Check(obj))
    tmp = PyObject_GetAttrString(obj,"real");
  else if (PyString_Check(obj) || PyUnicode_Check(obj))
    /*pass*/;
  else if (PySequence_Check(obj))
    tmp = PySequence_GetItem(obj,0);
  if (tmp) {
    PyErr_Clear();
    if (long_long_from_pyobj(v,tmp,errmess)) {Py_DECREF(tmp); return 1;}
    Py_DECREF(tmp);
  }
  {
    PyObject* err = PyErr_Occurred();
    if (err==NULL) err = _fent_error;
    PyErr_SetString(err,errmess);
  }
  return 0;
}


/********************* See f2py2e/cfuncs.py: userincludes *********************/
/*need_userincludes*/

/********************* See f2py2e/capi_rules.py: usercode *********************/


/* See f2py2e/rules.py */
extern void F_FUNC(minmaxr,MINMAXR)(float*,int*,float*,float*);
extern void F_FUNC_US(emi_from_margins,EMI_FROM_MARGINS)(long_long*,long_long*,long_long*,long_long*,double*);
/*eof externroutines*/

/******************** See f2py2e/capi_rules.py: usercode1 ********************/


/******************* See f2py2e/cb_rules.py: buildcallback *******************/
/*need_callbacks*/

/*********************** See f2py2e/rules.py: buildapi ***********************/

/********************************** minmaxr **********************************/
static char doc_f2py_rout__fent_minmaxr[] = "\
amin,amax = minmaxr(a,[n])\n\nWrapper for ``minmaxr``.\
\n\nParameters\n----------\n"
"a : input rank-1 array('f') with bounds (n)\n"
"\nOther Parameters\n----------------\n"
"n : input int, optional\n    Default: len(a)\n"
"\nReturns\n-------\n"
"amin : float\n"
"amax : float";
/* extern void F_FUNC(minmaxr,MINMAXR)(float*,int*,float*,float*); */
static PyObject *f2py_rout__fent_minmaxr(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(float*,int*,float*,float*)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  float *a = NULL;
  npy_intp a_Dims[1] = {-1};
  const int a_Rank = 1;
  PyArrayObject *capi_a_tmp = NULL;
  int capi_a_intent = 0;
  PyObject *a_capi = Py_None;
  int n = 0;
  PyObject *n_capi = Py_None;
  float amin = 0;
  float amax = 0;
  static char *capi_kwlist[] = {"a","n",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    "O|O:_fent.minmaxr",\
    capi_kwlist,&a_capi,&n_capi))
    return NULL;
/*frompyobj*/
  /* Processing variable a */
  ;
  capi_a_intent |= F2PY_INTENT_IN;
  capi_a_tmp = array_from_pyobj(NPY_FLOAT,a_Dims,a_Rank,capi_a_intent,a_capi);
  if (capi_a_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(_fent_error,"failed in converting 1st argument `a' of _fent.minmaxr to C/Fortran array" );
  } else {
    a = (float *)(PyArray_DATA(capi_a_tmp));

  /* Processing variable amin */
  /* Processing variable amax */
  /* Processing variable n */
  if (n_capi == Py_None) n = len(a); else
    f2py_success = int_from_pyobj(&n,n_capi,"_fent.minmaxr() 1st keyword (n) can't be converted to int");
  if (f2py_success) {
  CHECKSCALAR(len(a)>=n,"len(a)>=n","1st keyword n","minmaxr:n=%d",n) {
/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
        (*f2py_func)(a,&n,&amin,&amax);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("ff",amin,amax);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
  } /*CHECKSCALAR(len(a)>=n)*/
  } /*if (f2py_success) of n*/
  /* End of cleaning variable n */
  /* End of cleaning variable amax */
  /* End of cleaning variable amin */
  if((PyObject *)capi_a_tmp!=a_capi) {
    Py_XDECREF(capi_a_tmp); }
  }  /*if (capi_a_tmp == NULL) ... else of a*/
  /* End of cleaning variable a */
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/******************************* end of minmaxr *******************************/

/****************************** emi_from_margins ******************************/
static char doc_f2py_rout__fent_emi_from_margins[] = "\
emi = emi_from_margins(a,b,[r,c])\n\nWrapper for ``emi_from_margins``.\
\n\nParameters\n----------\n"
"a : input rank-1 array('q') with bounds (r)\n"
"b : input rank-1 array('q') with bounds (c)\n"
"\nOther Parameters\n----------------\n"
"r : input long, optional\n    Default: len(a)\n"
"c : input long, optional\n    Default: len(b)\n"
"\nReturns\n-------\n"
"emi : float";
/* extern void F_FUNC_US(emi_from_margins,EMI_FROM_MARGINS)(long_long*,long_long*,long_long*,long_long*,double*); */
static PyObject *f2py_rout__fent_emi_from_margins(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(long_long*,long_long*,long_long*,long_long*,double*)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  long_long *a = NULL;
  npy_intp a_Dims[1] = {-1};
  const int a_Rank = 1;
  PyArrayObject *capi_a_tmp = NULL;
  int capi_a_intent = 0;
  PyObject *a_capi = Py_None;
  long_long r = 0;
  PyObject *r_capi = Py_None;
  long_long *b = NULL;
  npy_intp b_Dims[1] = {-1};
  const int b_Rank = 1;
  PyArrayObject *capi_b_tmp = NULL;
  int capi_b_intent = 0;
  PyObject *b_capi = Py_None;
  long_long c = 0;
  PyObject *c_capi = Py_None;
  double emi = 0;
  static char *capi_kwlist[] = {"a","b","r","c",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    "OO|OO:_fent.emi_from_margins",\
    capi_kwlist,&a_capi,&b_capi,&r_capi,&c_capi))
    return NULL;
/*frompyobj*/
  /* Processing variable a */
  ;
  capi_a_intent |= F2PY_INTENT_IN;
  capi_a_tmp = array_from_pyobj(NPY_LONGLONG,a_Dims,a_Rank,capi_a_intent,a_capi);
  if (capi_a_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(_fent_error,"failed in converting 1st argument `a' of _fent.emi_from_margins to C/Fortran array" );
  } else {
    a = (long_long *)(PyArray_DATA(capi_a_tmp));

  /* Processing variable emi */
  /* Processing variable b */
  ;
  capi_b_intent |= F2PY_INTENT_IN;
  capi_b_tmp = array_from_pyobj(NPY_LONGLONG,b_Dims,b_Rank,capi_b_intent,b_capi);
  if (capi_b_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(_fent_error,"failed in converting 2nd argument `b' of _fent.emi_from_margins to C/Fortran array" );
  } else {
    b = (long_long *)(PyArray_DATA(capi_b_tmp));

  /* Processing variable c */
  if (c_capi == Py_None) c = len(b); else
    f2py_success = long_long_from_pyobj(&c,c_capi,"_fent.emi_from_margins() 2nd keyword (c) can't be converted to long_long");
  if (f2py_success) {
  CHECKSCALAR(len(b)>=c,"len(b)>=c","2nd keyword c","emi_from_margins:c=%ld",c) {
  /* Processing variable r */
  if (r_capi == Py_None) r = len(a); else
    f2py_success = long_long_from_pyobj(&r,r_capi,"_fent.emi_from_margins() 1st keyword (r) can't be converted to long_long");
  if (f2py_success) {
  CHECKSCALAR(len(a)>=r,"len(a)>=r","1st keyword r","emi_from_margins:r=%ld",r) {
/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
        (*f2py_func)(a,&r,b,&c,&emi);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("d",emi);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
  } /*CHECKSCALAR(len(a)>=r)*/
  } /*if (f2py_success) of r*/
  /* End of cleaning variable r */
  } /*CHECKSCALAR(len(b)>=c)*/
  } /*if (f2py_success) of c*/
  /* End of cleaning variable c */
  if((PyObject *)capi_b_tmp!=b_capi) {
    Py_XDECREF(capi_b_tmp); }
  }  /*if (capi_b_tmp == NULL) ... else of b*/
  /* End of cleaning variable b */
  /* End of cleaning variable emi */
  if((PyObject *)capi_a_tmp!=a_capi) {
    Py_XDECREF(capi_a_tmp); }
  }  /*if (capi_a_tmp == NULL) ... else of a*/
  /* End of cleaning variable a */
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/************************** end of emi_from_margins **************************/
/*eof body*/

/******************* See f2py2e/f90mod_rules.py: buildhooks *******************/
/*need_f90modhooks*/

/************** See f2py2e/rules.py: module_rules['modulebody'] **************/

/******************* See f2py2e/common_rules.py: buildhooks *******************/

/*need_commonhooks*/

/**************************** See f2py2e/rules.py ****************************/

static FortranDataDef f2py_routine_defs[] = {
  {"minmaxr",-1,{{-1}},0,(char *)F_FUNC(minmaxr,MINMAXR),(f2py_init_func)f2py_rout__fent_minmaxr,doc_f2py_rout__fent_minmaxr},
  {"emi_from_margins",-1,{{-1}},0,(char *)F_FUNC_US(emi_from_margins,EMI_FROM_MARGINS),(f2py_init_func)f2py_rout__fent_emi_from_margins,doc_f2py_rout__fent_emi_from_margins},

/*eof routine_defs*/
  {NULL}
};

static PyMethodDef f2py_module_methods[] = {

  {NULL,NULL}
};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "_fent",
  NULL,
  -1,
  f2py_module_methods,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif

#if PY_VERSION_HEX >= 0x03000000
#define RETVAL m
PyMODINIT_FUNC PyInit__fent(void) {
#else
#define RETVAL
PyMODINIT_FUNC init_fent(void) {
#endif
  int i;
  PyObject *m,*d, *s;
#if PY_VERSION_HEX >= 0x03000000
  m = _fent_module = PyModule_Create(&moduledef);
#else
  m = _fent_module = Py_InitModule("_fent", f2py_module_methods);
#endif
  Py_TYPE(&PyFortran_Type) = &PyType_Type;
  import_array();
  if (PyErr_Occurred())
    {PyErr_SetString(PyExc_ImportError, "can't initialize module _fent (failed to import numpy)"); return RETVAL;}
  d = PyModule_GetDict(m);
  s = PyString_FromString("$Revision: $");
  PyDict_SetItemString(d, "__version__", s);
#if PY_VERSION_HEX >= 0x03000000
  s = PyUnicode_FromString(
#else
  s = PyString_FromString(
#endif
    "This module '_fent' is auto-generated with f2py (version:2).\nFunctions:\n"
"  amin,amax = minmaxr(a,n=len(a))\n"
"  emi = emi_from_margins(a,b,r=len(a),c=len(b))\n"
".");
  PyDict_SetItemString(d, "__doc__", s);
  _fent_error = PyErr_NewException ("_fent.error", NULL, NULL);
  Py_DECREF(s);
  for(i=0;f2py_routine_defs[i].name!=NULL;i++)
    PyDict_SetItemString(d, f2py_routine_defs[i].name,PyFortranObject_NewAsAttr(&f2py_routine_defs[i]));


/*eof initf2pywraphooks*/
/*eof initf90modhooks*/

/*eof initcommonhooks*/


#ifdef F2PY_REPORT_ATEXIT
  if (! PyErr_Occurred())
    on_exit(f2py_report_on_exit,(void*)"_fent");
#endif

  return RETVAL;
}
#ifdef __cplusplus
}
#endif
