
#include "OpenACC/utils/tuning.h"

#include "OpenACC/internal/compiler.h"

#include <assert.h>

acc_compiler_data_t compiler_data = {
  (const char * ) LIBOPENACC_DIR, /* Absolute directory for OpenACC runtime (needed to compile OpenCL C codes) */
  (const char * ) "opencl/libopenacc.cl",  /* Name of the OpenCL C runtime file */
  (const char * ) KERNEL_DIR, /* Absolute directory where the OpenCL C kernels have been stored. */
  0, NULL
};

int main(int argc, char ** argv) {

  assert(argc == 2);

  char * devices_name[1] = {"ANY"};

  struct acc_tuner_device_ranges_t_ * ranges_per_devices = acc_tuning_build_device_ranges(1);
    ranges_per_devices[0].num_versions = 1;
      size_t versions_0[1] = {1};
      ranges_per_devices[0].version_ids = versions_0;
    ranges_per_devices[0].num_gang_values = 3;
      size_t gang_0[3] = {16, 32, 64};
      ranges_per_devices[0].gang_values = gang_0;
    ranges_per_devices[0].num_worker_values = 3;
      size_t worker_0[3] = {16, 32, 64};
      ranges_per_devices[0].worker_values = worker_0;
    ranges_per_devices[0].num_vector_values = 1;
      size_t vector_0[1] = {1};
      ranges_per_devices[0].vector_values = vector_0;

  struct acc_tuner_data_params_desc_t_ * data_params = acc_tuning_build_data_params(3, "n", "INT", "m", "INT", "p", "INT");

  size_t params_values[6] = {
    256, 256, 256,
    512, 512, 512
  };

  acc_tuning_init(1, devices_name, ranges_per_devices, data_params, 2, params_values, 1, argv[1]);

  return 0;
}

