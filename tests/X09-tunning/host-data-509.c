
#include "OpenACC/openacc.h"
#include "OpenACC/internal/compiler.h"
#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/loop.h"

#ifndef NUM_DEVICES
# error "Need to define number of devices (NUM_DEVICES)"
#endif

extern unsigned portions[NUM_DEVICES];

extern size_t version_by_devices[NUM_DEVICES];

extern struct {
  acc_device_t kind;
  size_t num;
} dev_list[NUM_DEVICES];

struct acc_loop_t_ loops_0[3] = {
  {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_0 = {
  0, 0, 1,
  loops_0,
  "_0",
  acc_device_any
};

struct acc_loop_t_ loops_1[3] = {
  {{1L, 0L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_1 = {
  0, 0, 1,
  loops_1,
  "_1",
  acc_device_any
};

struct acc_loop_t_ loops_2[3] = {
  {{1L, 0L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_2 = {
  0, 0, 1,
  loops_2,
  "_2",
  acc_device_any
};

struct acc_loop_t_ loops_3[3] = {
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_3 = {
  0, 0, 1,
  loops_3,
  "_3",
  acc_device_any
};

struct acc_loop_t_ loops_4[3] = {
  {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_4 = {
  0, 0, 1,
  loops_4,
  "_4",
  acc_device_any
};

struct acc_loop_t_ loops_5[3] = {
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_5 = {
  0, 0, 1,
  loops_5,
  "_5",
  acc_device_any
};

struct acc_loop_t_ loops_6[3] = {
  {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_6 = {
  0, 0, 1,
  loops_6,
  "_6",
  acc_device_any
};

struct acc_loop_t_ loops_7[3] = {
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_7 = {
  0, 0, 1,
  loops_7,
  "_7",
  acc_device_any
};

struct acc_loop_t_ loops_8[3] = {
  {{1L, 1L, 0L, 0L, 1L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_8 = {
  0, 0, 1,
  loops_8,
  "_8",
  acc_device_any
};

struct acc_loop_t_ loops_9[3] = {
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{0L, 0L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_9 = {
  0, 0, 1,
  loops_9,
  "_9",
  acc_device_any
};

struct acc_loop_t_ loops_10[3] = {
  {{1L, 1L, 1L, 0L, 0L, 1L, 1L}}, {{1L, 0L, 0L, 1L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_10 = {
  0, 0, 1,
  loops_10,
  "_10",
  acc_device_any
};

struct acc_loop_t_ loops_11[3] = {
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_11 = {
  0, 0, 1,
  loops_11,
  "_11",
  acc_device_any
};

struct acc_loop_t_ loops_12[3] = {
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_12 = {
  0, 0, 1,
  loops_12,
  "_12",
  acc_device_any
};

struct acc_loop_t_ loops_13[3] = {
  {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}, {{0L, 0L, 1L, 0L, 1L, 1L, 1L}}, {{0L, 1L, 1L, 1L, 1L, 1L, 1L}}
};

struct acc_kernel_version_t_ version_13 = {
  0, 0, 1,
  loops_13,
  "_13",
  acc_device_any
};

struct acc_kernel_version_t_ * versions[14] = {
  &version_0,
  &version_1,
  &version_2,
  &version_3,
  &version_4,
  &version_5,
  &version_6,
  &version_7,
  &version_8,
  &version_9,
  &version_10,
  &version_11,
  &version_12,
  &version_13
};

const struct acc_loop_splitter_t_ kernel_splitted_loop = { 0, e_contiguous, NUM_DEVICES, portions, 0 };

const size_t param_sizes[3] = { sizeof(int) , sizeof(int) , sizeof(int) };

struct acc_kernel_desc_t_ kernel_desc_0_0 = {
  0,
  "kernel",
  3, param_sizes,
  0, NULL,
  3,
  3,
  14, versions,
  &kernel_splitted_loop,
  version_by_devices
};

struct acc_kernel_desc_t_ * kernels_0[1] = { &kernel_desc_0_0 };

const struct acc_data_distribution_t_ region_0_data_dist [3] = {
  { e_contiguous, NUM_DEVICES, portions, 0 },
  { e_contiguous, NUM_DEVICES, portions, 0 },
  { e_all, NUM_DEVICES, portions, 0 }
};

struct acc_region_desc_t_ region_desc_0 = {
  0,
  "kernels-509.cl",
  0, NULL,
  1, kernels_0,
  NUM_DEVICES, dev_list,
  3, region_0_data_dist
};


struct acc_region_desc_t_ * regions[1] = { &region_desc_0 };

struct acc_compiler_data_t_ compiler_data = { LIBOPENACC_DIR, "lib/opencl/libopenacc.cl", KERNEL_DIR, 1, regions };

