/*!
 * \file openacc.cpp 
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * These examples test: "OpenACC to OpenCL Compiler" for C++.
 * 
 * C++ OpenACC examples:
 *    - program_(name) functions are OpenACC Programs exposing/isolating some part(s) of OpenACC in C++
 *        + program_(name) have corresponding (CONFIG)_PROGRAM_(NAME) preprocessor macro
 *        + program_(name) take the classic argc/argv arguments
 *    - kernel_(name) functions are  OpenACC Kernels that are to be called 
 *        + kernel_(name) have corresponding (CONFIG)_PROGRAM_(NAME) preprocessor macro
 *        + kernel_(name) can have any kind of arguments
 *    - ENABLE_LIB_(NAME) preprocessor macros includes necessary header files
 *
 * The naming convention is NOT used by the compiler. However, in this file it is required for automated testing and checking of the results.
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

// A C-style Vector Addition

#ifdef ENABLE_PROGRAM_VECT_ADD_C_STYLE

#ifndef VECT_ADD_N
# define VECT_ADD_N 100
#endif

int program_vector_addition_c_style(int argc, char ** argv) {
  const unsigned long n = VECT_ADD_MACRO_N;
  float a[n];
  float b[n];
  float c[n];

  for (unsigned i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

  #pragma acc parallel copyin(a[0:n], b[n]) copyout(c) num_gangs(16) num_worker(64) vector_length(8)
  {
    #pragma acc loop independant gang(16) worker(64) vector(8)
    for (unsigned i = 0; i < n; i++) {
      res[i] = a[i] + b[i];
    }
  }

  float error = 0.;
  for (unsigned i = 0; i < n; i++) {
    error += (res[i] - (a[i] + b[i])) * (res[i] - (a[i] + b[i]));
  }

  return error < VECT_ADD_C_STYLE_TOLERANCE;
}

#endif


#if 0 || ENABLE_PROGRAM_VECT_ADD_FUNCTION_TEMPLATE

template <typename scalar_t, typename data_t>
void program_vector_addition(scalar_t n, data_t * A, data_t * B) {

}

#endif

