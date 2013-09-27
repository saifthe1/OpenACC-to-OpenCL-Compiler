/*!
 * \file opencl.cpp 
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * This file contains the expected output of "OpenACC to OpenCL Compiler" for the file openacc.cpp
 * 
 * The naming convention used for this file should be followed by the compiler. It will help debugging and change tracking
 *
 */

#ifdef ENABLE_PROGRAM_VECT_ADD_C_STYLE
# ifndef ENABLE_LIB_CSTDLIB
#  error "C-style Vector Addition requires C Standard Library"
# endif
#endif

#ifdef ENABLE_LIB_CSTDLIB
# include <cstdlib>
#endif

#include "ACC/acc.hpp"
#include "CL/cl.hpp"

// A C-style Vector Addition

#ifdef ENABLE_PROGRAM_VECT_ADD_C_STYLE

#ifndef VECT_ADD_N
# define VECT_ADD_N 100
#endif

#ifndef VECT_ADD_C_STYLE_DATA_TYPE
# define VECT_ADD_C_STYLE_DATA_TYPE float
#endif

int program_vector_addition_c_style(int argc, char ** argv) {
  const unsigned long n = VECT_ADD_MACRO_N;
  VECT_ADD_C_STYLE_DATA_TYPE a[n];
  VECT_ADD_C_STYLE_DATA_TYPE b[n];
  VECT_ADD_C_STYLE_DATA_TYPE c[n];

  for (unsigned i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

  /// \todo Translate to OpenCL
  #pragma acc parallel
  {
    #pragma acc loop
    for (unsigned i = 0; i < n; i++) {
      res[i] = a[i] + b[i];
    }
  }

  VECT_ADD_C_STYLE_DATA_TYPE error = 0.;
  for (unsigned i = 0; i < n; i++) {
    error += (res[i] - (a[i] + b[i])) * (res[i] - (a[i] + b[i]));
  }

  return error < VECT_ADD_C_STYLE_TOLERANCE;
}

#endif

