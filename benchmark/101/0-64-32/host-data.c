#include "/media/ssd/projects/currents/openacc-to-opencl-compiler/libopenacc/include/OpenACC/internal/region.h" 
#include "/media/ssd/projects/currents/openacc-to-opencl-compiler/libopenacc/include/OpenACC/internal/compiler.h" 
/* File generated by Driver<Model>::getFileID("host-data.c") */
struct acc_loop_t_ ;
struct acc_kernel_version_t_ ;
struct acc_compiler_data_t_ ;
size_t param_sizes_0x12b7130[1] = {sizeof(unsigned long long )};
size_t scalar_sizes_0x12b7130[1] = {sizeof(float )};
struct acc_loop_t_ loops_0x12b4dc0[1UL] = {{{0L, 0L, 64L, 0L, 32L, 1L, 1L}}};
struct acc_kernel_version_t_ version_0 = {0, 0, 1, loops_0x12b4dc0, "kernel_0x12b7130_0"};
struct acc_kernel_version_t_ *versions[1UL] = {&version_0};
struct acc_kernel_desc_t_ kernel_desc_0_0 = {0, "", 1, param_sizes_0x12b7130, 1, scalar_sizes_0x12b7130, 1, 1, 1, versions, 0};
struct acc_kernel_desc_t_ *kernels_0[1UL] = {&kernel_desc_0_0};
struct acc_region_desc_t_ region_desc_0 = {0, "kernels.cl", 0, 0, 1, kernels_0, 1, 0, 0, 0};
struct acc_region_desc_t_ *regions[1UL] = {&region_desc_0};
struct acc_compiler_data_t_ compiler_data = {"/media/ssd/projects/currents/openacc-to-opencl-compiler/libopenacc/include//../", "lib/opencl/libopenacc.cl", "/media/ssd/projects/currents/openacc-to-opencl-compiler/benchmark/101/0-64-32", 1, regions};

unsigned tiles[4] = {0, 64, 32, 1};
unsigned test_id = 101;

