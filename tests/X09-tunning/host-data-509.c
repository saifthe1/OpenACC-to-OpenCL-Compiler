
#include "OpenACC/openacc.h"
#include "OpenACC/internal/compiler.h"
#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/loop.h"

#ifndef GPU_PORTION
# error "Need to define GPU portion"
#endif

#ifndef GPU_KERNEL_ID
# error "Need to define GPU kernel ID"
#endif

#ifndef MIC_PORTION
# error "Need to define MIC portion"
#endif

#ifndef MIC_KERNEL_ID
# error "Need to define MIC kernel ID"
#endif

unsigned portions[2] = { GPU_PORTION , MIC_PORTION };

size_t param_sizes[3] = { sizeof(int) , sizeof(int) , sizeof(int) };

size_t gpu_kernel_id = GPU_KERNEL_ID;
size_t mic_kernel_id = MIC_KERNEL_ID;

struct acc_loop_t_ loops_gtx_460[3] = {
#if GPU_KERNEL_ID == 0
  {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 1
  {{1L, 0L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 2
  {{1L, 0L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 3
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 4
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 5
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 6
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 7
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 8
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 9
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 10
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 11
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 12
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif GPU_KERNEL_ID == 13
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#else
#  error "Wrong value for GPU_KERNEL_ID"
#endif
};

struct acc_kernel_version_t_ version_gtx_460 = {
  0, 0, 1,
  loops_gtx_460,
#if GPU_KERNEL_ID == 0
  "_0",
#elif GPU_KERNEL_ID == 1
  "_1",
#elif GPU_KERNEL_ID == 2
  "_2",
#elif GPU_KERNEL_ID == 3
  "_3",
#elif GPU_KERNEL_ID == 4
  "_4",
#elif GPU_KERNEL_ID == 5
  "_5",
#elif GPU_KERNEL_ID == 6
  "_6",
#elif GPU_KERNEL_ID == 7
  "_7",
#elif GPU_KERNEL_ID == 8
  "_8",
#elif GPU_KERNEL_ID == 9
  "_9",
#elif GPU_KERNEL_ID == 10
  "_10",
#elif GPU_KERNEL_ID == 11
  "_11",
#elif GPU_KERNEL_ID == 12
  "_12",
#elif GPU_KERNEL_ID == 13
  "_13",
#else
#  error "Wrong value for GPU_KERNEL_ID"
#endif
  acc_device_GTX_460
};

struct acc_loop_t_ loops_xeonphi[3] = {
#if MIC_KERNEL_ID == 0
  {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 1
  {{1L, 0L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 2
  {{1L, 0L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 3
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 4
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 5
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 6
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 7
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 8
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 9
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 10
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 11
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 12
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#elif MIC_KERNEL_ID == 13
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
#else
#  error "Wrong value for MIC_KERNEL_ID"
#endif
};

struct acc_kernel_version_t_ version_xeonphi = {
  0, 0, 1,
  loops_xeonphi,
#if MIC_KERNEL_ID == 0
  "_0",
#elif MIC_KERNEL_ID == 1
  "_1",
#elif MIC_KERNEL_ID == 2
  "_2",
#elif MIC_KERNEL_ID == 3
  "_3",
#elif MIC_KERNEL_ID == 4
  "_4",
#elif MIC_KERNEL_ID == 5
  "_5",
#elif MIC_KERNEL_ID == 6
  "_6",
#elif MIC_KERNEL_ID == 7
  "_7",
#elif MIC_KERNEL_ID == 8
  "_8",
#elif MIC_KERNEL_ID == 9
  "_9",
#elif MIC_KERNEL_ID == 10
  "_10",
#elif MIC_KERNEL_ID == 11
  "_11",
#elif MIC_KERNEL_ID == 12
  "_12",
#elif MIC_KERNEL_ID == 13
  "_13",
#else
#  error "Wrong value for MIC_KERNEL_ID"
#endif
  acc_device_xeonphi
};

struct acc_kernel_version_t_ * versions[2] = {
  &version_gtx_460,
  &version_xeonphi
};

const struct acc_loop_splitter_t_ kernel_splitted_loop = { 0, e_contiguous, 2, portions, 0 };

struct acc_kernel_desc_t_ kernel_desc_0_0 = {
  0,
  "kernel",
  3, param_sizes,
  0, NULL,
  3,
  3,
  1, versions,
  &kernel_splitted_loop
};

struct acc_kernel_desc_t_ * kernels_0[1] = { &kernel_desc_0_0 };

const struct {
  acc_device_t kind;
  size_t num;
} dev_list[2] = { {acc_device_GTX_460, 0}, {acc_device_xeonphi, 0} };

const struct acc_data_distribution_t_ region_0_data_dist [2] = {
  { e_contiguous, 2, portions, 0 },
  { e_contiguous, 2, portions, 0 }
};

struct acc_region_desc_t_ region_desc_0 = {
  0,
  "kernels-509.cl",
  0, NULL,
  1, kernels_0,
  2, dev_list,
  2, region_0_data_dist
};


struct acc_region_desc_t_ * regions[1] = { &region_desc_0 };

struct acc_compiler_data_t_ compiler_data = { LIBOPENACC_DIR, "lib/opencl/libopenacc.cl", KERNEL_DIR, 1, regions };

