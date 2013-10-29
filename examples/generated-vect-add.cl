/*!
 * \file generated-vect-add.cl
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * \note The naming convention used for this file are to be a guide line for the compiler.
 *
 * \note Comments abbreviations:
 *     - GR: OpenACC Gang   Redundant    Execution Modes
 *     - GP: OpenACC Gang   Partitionned Execution Modes
 *     - WS: OpenACC Worker Single       Execution Modes
 *     - WP: OpenACC Worker Partitionned Execution Modes
 *     - VS: OpenACC Vector Single       Execution Modes
 *     - VP: OpenACC Vector Partitionned Execution Modes
 * 
 */

#include "OpenACC/lib-openacc.cl"

/*!
 *  OpenCL C kernel for Vector Addition as it would be generated.
 */
__global void vect_add_kernel_1_vect_8(
    unsigned long n,
    float * a, float * b, float * res,
    __constant struct acc_context_t * ctx
) {
  // Verify that we are in Gang Partitionned & Worker Partitionned mode 
  if (acc_GP_mode(ctx) && acc_WP_mode(ctx)) {

    // Get the bounds for the tile between Worker and Vector
    acc_bounds_t loop_1_bounds_2 = acc_loop_bounds(ctx, 1, 2);

    for (loop_1_it_0 = loop_1_bounds_2.lower;
         loop_1_it_0 < loop_1_bounds_2.upper;
         loop_1_it_0 += loop_1_bounds_2.stride) {

      // Compute the current iteration of loop_1 
      unsigned long loop_1_it = acc_loop_iteration(ctx, 1, 0, 0, loop_1_it_2, 0);

      cl_float8 a_ = vload8(loop_1_it, a);
      cl_float8 b_ = vload8(loop_1_it, b);

      cl_float8 res_ = a_ + b_;

      vstore8(res_, loop_1_it, res);
    }
  }
}

