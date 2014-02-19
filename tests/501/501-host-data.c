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

unsigned kernel_0x00_splitted_loops[1] = { 0 };

struct acc_kernel_desc_t_ kernel_0x00_desc = {
  0,
  "kernel_101",
  1, kernel_0x00_scalar_sizes,
  1,
  1,
  1, kernel_0x00_versions,
  1, kernel_0x00_splitted_loops
};

acc_kernel_desc_t region_0x00_kernels[1] = { &kernel_0x00_desc };

struct {
  acc_device_t kind;
  size_t num;
} dev_list[2] = { {acc_device_nvidia, 0}, {acc_device_xeonphi, 0} };

struct acc_region_desc_t_ region_0x00_desc = {
  0,
  "501-kernels.cl",
  0, NULL,
  1, region_0x00_kernels,
  2, dev_list
};

acc_region_desc_t regions[1] = { &region_0x00_desc };

acc_compiler_data_t compiler_data = {
  (const char * ) ACC_RUNTIME_ABS_DIR, /* Absolute directory for OpenACC runtime (needed to compile OpenCL C codes) */
  (const char * ) ACC_RUNTIME_OPENCL_FILE,  /* Name of the OpenCL C runtime file */
  (const char * ) ACC_KERNELS_ABS_DIR, /* Absolute directory where the OpenCL C kernels have been stored. */
  1, regions
};

