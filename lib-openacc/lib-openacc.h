/*!
 * \file lib-openacc.h
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * C API of the implementation specific part of the OpenACC Runtime for "OpenACC to OpenCL Compiler"
 *
 */

#ifndef __LIB_OPENACC_H__
#define __LIB_OPENACC_H__

#include "OpenACC/openacc.h"

// Errors

typedef int acc_error_t;
void acc_fail_if_error(acc_error_t);

// Pointer to Data-Structure types

typedef struct acc_device_t_        * acc_device_t;
typedef struct acc_runtime_t_       * acc_runtime_t;
typedef struct acc_parallel_t_      * acc_parallel_t;
typedef struct acc_loop_t_          * acc_loop_t;
typedef struct acc_opencl_kernel_t_ * acc_opencl_kernel_t;

// Runtime

struct acc_runtime_t_ {
  acc_device_t device;

  unsigned num_kernel;
  acc_opencl_kernel_t * kernels;
};
typedef acc_runtime_t_ * acc_runtime_t;
extern acc_runtime_t acc_runtime;

// Devices

enum acc_device_e {
  e_acc_device_opencl_default,
  e_acc_device_unknown
};

struct acc_device_t_ {
  
};

acc_build_device(e_opencl_default)

// Parallel support

struct acc_parallel_t_ {
  unsigned num_dims;           ///< Number of dimension in the parallel region (currently only 1 supported by OpenACC directives)
  unsigned long * num_gang;    ///< Number of gang   for each dimension
  unsigned long * num_worker;  ///< Number of worker for each dimension

  unsigned vector_length; ///< Vector size used for this parallel region (needed to determine workers chunk size)
};
typedef acc_parallel_t_ * acc_parallel_t;

acc_parallel_t acc_build_parallel(unsigned, unsigned long *, unsigned long *, unsigned);

/*! \func acc_parallel_start
 *  \param[region] pointer to a parallel region descriptor
 *  \return a non-zero value if an error occured
 */
acc_error_t acc_parallel_start(acc_parallel_t region);

/*! \func acc_parallel_stop
 *  \param[region] pointer to a parallel region descriptor
 *  \return a non-zero value if an error occured
 */
acc_error_t acc_parallel_stop (acc_parallel_t region);

// Loop Directive (Host side)

struct acc_loop_t_ {
  unsigned kernel_id;

  unsigned num_dims;
  unsigned long * gang;
  unsigned long * worker;

  unsigned long vector;
};

/*! \func acc_build_loop
 *
 *  Build a OpenACC loop descriptor
 *
 */
acc_loop_t acc_build_computation();

/*! \func acc_enqueue
 *
 * Called by the Host. It di
 *
 *  \param[region] pointer to a parallel region descriptor
 *  \param[loop] pointer to a loop descriptor
 *  \return a non-zero value if an error occured
 */
acc_error_t acc_enqueue(acc_parallel_t region, acc_loop_t loop);



#endif /* __LIB_OPENACC_H__ */

