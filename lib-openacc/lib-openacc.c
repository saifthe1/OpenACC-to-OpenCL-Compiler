/*!
 * \file lib-openacc.c
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * This file contains the code generated for "OpenACC to OpenCL Compiler" by "MDCG OpenCL Wrapper" for the file openacc.c
 * 
 * The naming convention used for this file should be followed by the compiler. It will help debugging and change tracking
 *
 */

#include "OpenACC/lib-openacc.h"

void fail(acc_error_t);

// Parallel Directive

acc_error_t acc_parallel_start(acc_parallel_t region);

acc_error_t acc_parallel_stop (acc_parallel_t region);

// Loop Directive (Host side)

acc_error_t acc_execute(acc_parallel_t region, acc_loop_t loop);

