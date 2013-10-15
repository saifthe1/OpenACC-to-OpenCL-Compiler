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
__global void vect_add_kernel_1_vect_8(unsigned long n, float * a, float * b, float * res, __constant acc_context_t ctx) {

  // Verify that we are in Gang Partitionned & Worker Partitionned mode 
  if (acc_GP_mode(ctx) && acc_WP_mode(ctx)) {

    // Assume there is no tile before Gang (outter most tile)
    unsigned long loop_1_it_0 = acc_loop_no_tile(ctx, 1, 0);

    // Assume there is no tile between Gang and Vector
    unsigned long loop_1_it_1 = acc_loop_no_tile(ctx, 1, 1);

    // Get the bounds for the tile between Worker and Vector
    acc_bounds_t loop_1_bounds_0 = acc_loop_bounds(ctx, 1, 2);

    // Assume there is no tile after Vector(inner most tile)
    unsigned long loop_1_it_3 = acc_loop_no_tile(ctx, 1, 3);

    for (loop_1_it_0 = loop_1_bounds_0.lower; loop_1_it_0 < loop_1_bounds_0.upper; loop_1_it_0 += loop_1_bounds_0.stride) {

      // Compute the current iteration of loop_1 
      unsigned long loop_1_it = acc_loop_iteration(ctx, 1, loop_1_it_0, loop_1_it_1, loop_1_it_2, loop_1_it_3);

      /// Verify that we are in Vector Partitionned mode
      if (acc_VP_mode(ctx)) {

        /// Vectorized code

        float8_t a_ = {res[i+0], res[i+1], res[i+2], res[i+3], res[i+4], res[i+5], res[i+6], res[i+7]}; /// \todo How to init cleanly?
        float8_t b_ = {res[i+0], res[i+1], res[i+2], res[i+3], res[i+4], res[i+5], res[i+6], res[i+7]};

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
  }
}

