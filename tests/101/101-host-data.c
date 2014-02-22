/*!
 *  \file tests/acc_compiler_data.c
 *
 * This file provides default version of the code that should be generated by the compiler for the runtime.
 *
 */

#include "OpenACC/internal/compiler.h"
#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/loop.h"

#ifndef ACC_RUNTIME_ABS_DIR
# error "Need OpenACC runtime directory."
#endif

#ifndef ACC_KERNELS_ABS_DIR
# error "Need generated kernels directory."
#endif

#ifndef ACC_RUNTIME_OPENCL_FILE
# define ACC_RUNTIME_OPENCL_FILE "lib/opencl/libopenacc.cl"
#endif

#if !defined(KERNEL_VERSION) || KERNEL_VERSION < 1 || KERNEL_VERSION > 3
# define KERNEL_VERSION 1
#endif

////////////////////////////////////
// Version 1 with tile #1 dynamic //
////////////////////////////////////

/// Dynamic gang, worker, and tile 0 ; tile 1, 2, 3, and vector are empty

struct acc_loop_t_ kernel_0x00_v1_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 0 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 1 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }, ///!< Vector Tile   : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v1 = {
  0, 0, 1, kernel_0x00_v1_loops, "_gang_worker_tile_0"
};

#if 0

/// Dynamic gang, worker, and tile 0 ; tile 1, 2, and 3 are empty ; vector are static size 2

struct acc_loop_t_ kernel_0x00_v1_2_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 0 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 2 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 2 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 2 }, ///!< Vector Tile   : static = 2
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v1_2 = {
  0, 0, 2, kernel_0x00_v1_2_loops, "_gang_worker_tile_0_vector_2"
};

/// Dynamic gang, worker, and tile 0 ; tile 1, 2, and 3 are empty ; vector are static size 4

struct acc_loop_t_ kernel_0x00_v1_4_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 0 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 4 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 4 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 4 }, ///!< Vector Tile   : static = 4
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v1_4 = {
  0, 0, 4, kernel_0x00_v1_4_loops, "_gang_worker_tile_0_vector_4"
};

/// Dynamic gang, worker, and tile 0 ; tile 1, 2, and 3 are empty ; vector are static size 8

struct acc_loop_t_ kernel_0x00_v1_8_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 0 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 8 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 8 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 8 }, ///!< Vector Tile   : static = 8
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v1_8 = {
  0, 0, 8, kernel_0x00_v1_8_loops, "_gang_worker_tile_0_vector_8"
};

/// Dynamic gang, worker, and tile 0 ; tile 1, 2, and 3 are empty ; vector are static size 16

struct acc_loop_t_ kernel_0x00_v1_16_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 0 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 16, 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 16, 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 16}, ///!< Vector Tile   : static = 16
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v1_16 = {
  0, 0, 16, kernel_0x00_v1_16_loops, "_gang_worker_tile_0_vector_16"
};

#endif

////////////////////////////////////
// Version 2 with tile #1 dynamic //
////////////////////////////////////

/// Dynamic gang, worker, and tile 1 ; tile 0, 2, 3, and vector are empty

struct acc_loop_t_ kernel_0x00_v2_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 1 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 1 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }, ///!< Vector Tile   : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v2 = {
  0, 0, 1, kernel_0x00_v2_loops, "_gang_worker_tile_1"
};

#if 0

/// Dynamic gang, worker, and tile 1 ; tile 0, 2, and 3 are empty ; vector are static size 2

struct acc_loop_t_ kernel_0x00_v2_2_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 1 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 2 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 2 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 2 }, ///!< Vector Tile   : static = 2
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v2_2 = {
  0, 0, 2, kernel_0x00_v2_2_loops, "_gang_worker_tile_1_vector_2"
};

/// Dynamic gang, worker, and tile 1 ; tile 0, 2, and 3 are empty ; vector are static size 4

struct acc_loop_t_ kernel_0x00_v2_4_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 1 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 4 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 4 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 4 }, ///!< Vector Tile   : static = 4
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v2_4 = {
  0, 0, 4, kernel_0x00_v2_4_loops, "_gang_worker_tile_1_vector_4"
};

/// Dynamic gang, worker, and tile 1 ; tile 0, 2, and 3 are empty ; vector are static size 8

struct acc_loop_t_ kernel_0x00_v2_8_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 1 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 8 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 8 , 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 8 }, ///!< Vector Tile   : static = 8
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v2_8 = {
  0, 0, 8, kernel_0x00_v2_8_loops, "_gang_worker_tile_1_vector_8"
};

/// Dynamic gang, worker, and tile 1 ; tile 0, 2, and 3 are empty ; vector are static size 16

struct acc_loop_t_ kernel_0x00_v2_16_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Filing Tile 1 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 16, 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 16, 1 }, ///!< Filing Tile 2 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 16}, ///!< Vector Tile   : static = 16
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v2_16 = {
  0, 0, 16, kernel_0x00_v2_16_loops, "_gang_worker_tile_1_vector_16"
};

#endif

////////////////////////////////////
// Version 3 with tile #2 dynamic //
////////////////////////////////////

/// Dynamic gang, worker, and tile 2 ; tile 0, 1, 3, and vector are empty

struct acc_loop_t_ kernel_0x00_v3_loops[1] = {
  {
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 0 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!<   Gang Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_iteration, 0 , 1 }, ///!< Filing Tile 1 : static = 1
    (struct acc_loop_tile_t_){ e_tiling_dynamic         , 0 , 0 }, ///!< Worker Tile   : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static_stride   , 1 , 0 }, ///!< Filing Tile 2 : dynamic
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }, ///!< Vector Tile   : static = 1
    (struct acc_loop_tile_t_){ e_tiling_static          , 1 , 1 }  ///!< Filing Tile 3 : static = 1
  }
};

struct acc_kernel_version_t_ kernel_0x00_v3 = {
  0, 0, 1, kernel_0x00_v3_loops, "_gang_worker_tile_2"
};

/// Kernel descriptor

#if KERNEL_VERSION == 1
acc_kernel_version_t kernel_0x00_versions[1] = {
  &kernel_0x00_v1
};
unsigned tiles[4] = {0, 1, 1, 1};
#elif KERNEL_VERSION == 2
acc_kernel_version_t kernel_0x00_versions[1] = {
  &kernel_0x00_v2
};
unsigned tiles[4] = {1, 0, 1, 1};
#elif KERNEL_VERSION == 3
acc_kernel_version_t kernel_0x00_versions[1] = {
  &kernel_0x00_v3
};
unsigned tiles[4] = {1, 1, 0, 1};
#endif

const size_t kernel_0x00_scalar_sizes[1] = { sizeof(float) };

struct acc_kernel_desc_t_ kernel_0x00_desc = {
  0,
  "kernel_101",
  1, kernel_0x00_scalar_sizes,
  1,
  1,
  1, kernel_0x00_versions,
  NULL
};

acc_kernel_desc_t region_0x00_kernels[1] = { &kernel_0x00_desc };

struct acc_region_desc_t_ region_0x00_desc = {
  0,
  "101-kernels.cl",
  0, NULL,
  1, region_0x00_kernels,
  1, NULL,
  0, NULL
};

acc_region_desc_t regions[1] = { &region_0x00_desc };

acc_compiler_data_t compiler_data = {
  (const char * ) ACC_RUNTIME_ABS_DIR, /* Absolute directory for OpenACC runtime (needed to compile OpenCL C codes) */
  (const char * ) ACC_RUNTIME_OPENCL_FILE,  /* Name of the OpenCL C runtime file */
  (const char * ) ACC_KERNELS_ABS_DIR, /* Absolute directory where the OpenCL C kernels have been stored. */
  1, regions
};
