#ifndef tests_h
#define tests_h

#include "CuTest.h"

CuSuite* CompfilesGetSuite();

CuSuite* fileUtilGetSuite();

CuSuite *scannerUtilGetSuite();

CuSuite* scannerGetSuite();

CuSuite* tokensTestGetSuite();

CuSuite* dfaTestGetSuite();

#endif
