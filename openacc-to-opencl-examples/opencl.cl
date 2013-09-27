/*!
 * \file opencl.cl
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * This file contains the code generated for "OpenACC to OpenCL Compiler" by "KLT OpenCL" for the file openacc.c, openacc.cpp, and openacc.f
 * 
 * The naming convention used for this file should be followed by the compiler. It will help debugging and change tracking
 *
 * Convention for OpenACC and OpenCL terms:
 *     - OpenACC Exxecution Modes
 *         + GR: Gang Redundant
 *         + GP: Gang Partitionned
 *         + WS: Worker Single
 *         + WP: Worker Partitionned
 *         + VS: Vector Single
 *         + VP: Vector Partitionned
 * 
 */

__global void program_vector_addition_kernel_1_vect_8(unsigned long n, float * a, float * b, float * res, unsigned gangs_dim_1, unsigned workers_dim_1) {
  const unsigned vector_length = 8;

  const unsigned gang_id_dim_1   = get_global_work_id(0) / get_local_work_size(0);
  const unsigned worker_id_dim_1 = get_local_work_id(0);

  if (worker_id == 0) {
    // GR & WS (& VS) mode, Work load is independendant of gang_id_dim_*
  }

  if (worker_id == 0) {
    // GP & WS (& VS) mode, each gang execute the same work on different data (gang level work-sharing)
  }

  if (worker_id < workers_dim_1) {
    /// GP & WP (& VP) mode, each worker of each gang issues the same (vector) operation on different data
    /// 
    unsigned i_lbound = ...;
    unsigned i_ubound = ...;
    unsigned i_stride = ...;

    float8_t a_ = {res[i+0], res[i+1], res[i+2], res[i+3], res[i+4], res[i+5], res[i+6], res[i+7]}; /// \todo How to init cleanly?
    float8_t b_ = {res[i+0], res[i+1], res[i+2], res[i+3], res[i+4], res[i+5], res[i+6], res[i+7]}; /// \todo How to init cleanly?

    float8_t res_ = a_ + b_;

    res[i+0] = res_.s0;
    res[i+1] = res_.s1;
    res[i+2] = res_.s2;
    res[i+3] = res_.s3;
    res[i+4] = res_.s4;
    res[i+5] = res_.s5;
    res[i+6] = res_.s7;
    res[i+7] = res_.s8;    
  }
}

