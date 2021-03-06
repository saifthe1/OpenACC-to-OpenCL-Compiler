
#include "OpenACC/public/arch.h"
#include "OpenACC/internal/opencl-init.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/internal/compiler.h"
#include "OpenACC/internal/region.h"
#include "OpenACC/private/debug.h"

#include "OpenACC/utils/io.h"
#include "OpenACC/utils/sqlite.h"

#include <stdio.h>
#include <string.h>

#include <assert.h>

unsigned acc_get_platform_desc(cl_platform_id platform) {
  char name[50];
  unsigned r;

  clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
  for (r = 0; r < NUM_OPENCL_PLATFORMS; r++)
    if (strcmp(name, platforms_desc[r].ocl_name) == 0)
      break;
  if (r == NUM_OPENCL_PLATFORMS) {
    printf("[fatal]   Unrecognized OpenCL Platform: \"%s\"\n", name);
    exit(-1);
  }

  return r;
}

unsigned acc_get_device_desc(cl_device_id * device, unsigned r) {
  char name[50];
  unsigned t;

  clGetDeviceInfo(*device, CL_DEVICE_NAME, sizeof(name), name, NULL);
  for (t = 0; t < platforms_desc[r].num_devices; t++) {
//    printf("compare %s\n     to %s\n return %s\n", name, platforms_desc[r].devices_type_desc[s].devices_desc[t].ocl_name, strstr(name, platforms_desc[r].devices_type_desc[s].devices_desc[t].ocl_name));
    if (strstr(name, platforms_desc[r].devices_desc[t].ocl_name) != NULL)
      break;
  }
  if (t == platforms_desc[r].num_devices) {
    printf("[fatal]   Unrecognized OpenCL Device : %s\n", name);
    exit(-1);
  }

  return t;
}

void acc_register_platform_desc(unsigned r, unsigned first, size_t num) {
  assert(platforms_desc[r].device != acc_device_last); /// \todo unsupported platform

  acc_runtime.devices[platforms_desc[r].device].first = first;
  acc_runtime.devices[platforms_desc[r].device].num   = num;
}

void acc_register_device_desc(unsigned r, unsigned t, unsigned first, size_t num) {
  acc_device_t device = platforms_desc[r].devices_desc[t].device;

  assert(device != acc_device_last); /// \todo unsupported device

  acc_runtime.devices[device].first = first;
  acc_runtime.devices[device].num   = num;
}

void acc_collect_ocl_devices() {
  cl_int status = clGetPlatformIDs(0, NULL, &(acc_runtime.opencl_data->num_platforms));
  if (status != CL_SUCCESS) {
    printf("[error]   clGetPlatformIDs return %u when looking for the number of platforms.\n", status);
    return;
  }

  if (acc_runtime.opencl_data->num_platforms == 0) {
    set_flag(f_ocl_devices);
    return;
  }

  acc_runtime.opencl_data->platforms = (cl_platform_id *)malloc(acc_runtime.opencl_data->num_platforms * sizeof(cl_platform_id));
  if (acc_runtime.opencl_data->platforms == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->platforms");
    exit(-1);
  }

  acc_runtime.opencl_data->num_devices = (cl_uint *)malloc((acc_runtime.opencl_data->num_platforms + 1) * sizeof(cl_uint)); // last elem contain sum
  if (acc_runtime.opencl_data->platforms == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->num_devices");
    exit(-1);
  }

  acc_runtime.opencl_data->devices = (cl_device_id **)malloc(acc_runtime.opencl_data->num_platforms * sizeof(cl_device_id *));
  if (acc_runtime.opencl_data->devices == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->devices");
    exit(-1);
  }

  status = clGetPlatformIDs(acc_runtime.opencl_data->num_platforms, acc_runtime.opencl_data->platforms, NULL);
  if (status != CL_SUCCESS) {
    printf("[error]   clGetPlatformIDs return %u when retrieving %u platforms.\n", status, acc_runtime.opencl_data->num_platforms);
    return;
  }

  unsigned i;
  unsigned num_devices = 0;
  for (i = 0; i < acc_runtime.opencl_data->num_platforms; i++) {
    const cl_platform_id platform = acc_runtime.opencl_data->platforms[i];

    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &(acc_runtime.opencl_data->num_devices[i]));
    if(status != CL_SUCCESS) {
     printf("[warning] clGetDeviceIDs return %u when looking for number of devices for platform %u\n", status, i);

     acc_runtime.opencl_data->num_devices[i] = 0;

     continue;
    }

    num_devices += acc_runtime.opencl_data->num_devices[i];
  }
  acc_runtime.opencl_data->num_devices[acc_runtime.opencl_data->num_platforms] = num_devices;

  cl_device_id * devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
  if(devices == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->devices[0]");
    exit(-1);
  }

  for (i = 0; i < acc_runtime.opencl_data->num_platforms; i++) {
    const cl_platform_id platform = acc_runtime.opencl_data->platforms[i];

    if (acc_runtime.opencl_data->num_devices[i] != 0) {
      status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, acc_runtime.opencl_data->num_devices[i], devices, NULL);
      if (status != CL_SUCCESS) {
        printf("[warning] clGetDeviceIDs return %u when retrieving %u devices for platform %u\n", status, acc_runtime.opencl_data->num_devices[i], i);

        acc_runtime.opencl_data->num_devices[i] = 0;
        acc_runtime.opencl_data->devices[i] = NULL;

        continue;
      }

      acc_runtime.opencl_data->devices[i] = devices;
      devices += acc_runtime.opencl_data->num_devices[i];
    }
    else {
      acc_runtime.opencl_data->devices[i] = NULL;
    }
  }

  acc_runtime.opencl_data->devices_data = (acc_device_data_t *)malloc(num_devices * sizeof(acc_device_data_t));
  if(acc_runtime.opencl_data->devices_data == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->devices_data");
    exit(-1);
  }

  for (i = 0; i < num_devices; i++)
    acc_runtime.opencl_data->devices_data[i] = NULL;

  set_flag(f_ocl_devices);
}

void acc_load_ocl_sources() {
  unsigned i;

  char ocl_rt_file[512];
  strcpy(ocl_rt_file, compiler_data.acc_runtime_dir);
  strcat(ocl_rt_file, "/");
  strcat(ocl_rt_file, compiler_data.acc_runtime_ocl);
  acc_runtime.opencl_data->runtime_sources = readSource(ocl_rt_file);

  if (compiler_data.regions == NULL) {
    char * env_versions_db = getenv("ACC_VERSIONS_DB");
    if (env_versions_db != NULL && env_versions_db[0] != '\0') {
      sqlite3 * versions_db = acc_sqlite_open(env_versions_db, 1, 1);
      acc_sqlite_load_compiler_data(versions_db, NULL);
      acc_sqlite_close(versions_db);
    }
  }

  if (compiler_data.num_regions <= 0) {
    printf("[error]   There is not any OpenCL region (parallel/kernel constructs) listed! Check that acc_init_kernel_first is called (and correct).\n");

    set_flag(f_ocl_sources);

    return;
  }

  acc_runtime.opencl_data->region_sources = (char **)malloc(compiler_data.num_regions * sizeof(char *));
  if (acc_runtime.opencl_data->region_sources == NULL) {
    perror("[fatal]   malloc: acc_runtime.opencl_data->region_sources");
    exit(-1);
  }

  char ocl_kernel_file[512];
  for (i = 0; i < compiler_data.num_regions; i++) {
    ocl_kernel_file[0] = '\0';
    strcpy(ocl_kernel_file, compiler_data.acc_kernels_dir);
    strcat(ocl_kernel_file, "/");
    strcat(ocl_kernel_file, compiler_data.regions[i]->file);
    acc_runtime.opencl_data->region_sources[i] = readSource(ocl_kernel_file);
  }

  set_flag(f_ocl_sources);
}

void acc_init_ocl_devices() {
  unsigned i, j;

  for (i = 0; i < acc_device_last; i++) {
    acc_runtime.devices[i].first = 0;
    acc_runtime.devices[i].num   = 0;
  }

  acc_runtime.devices[acc_device_any].first = 0;
  acc_runtime.devices[acc_device_any].num   = acc_runtime.opencl_data->num_devices[acc_runtime.opencl_data->num_platforms];

  /*!
   * \note Mapping devices from OpenCL to OpenACC.
   *
   * This "algorithm" computes the mapping between OpenACC devices and OpenCL devices.
   *
   * Obtained from OpenCL:
   *   - platforms as an array of cl_platform_id
   *   - devices   as an array of cl_device_id
   * Devices are sorted by platform then type (CL_DEVICE_TYPE) and model (CL_DEVICE_NAME).
   * Sorting by platforms is explicit (from OpenCL calls). \todo Check that other order are also true (or enforce)
   *
   * OpenACC devices are organized over 2 levels:
   *   -# Platform : OpenCL Platforms associted to each vendor (Intel, Nvidia, AMD, Altera, ...)
   *   -# Model    : OpenCL device's name
   * Each level addresses all devices addressed by its "children".
   * Each [platform], and [platform, model] have its own tag in acc_device_t (enumeration).
   *
   * This "algorithm" associates to each of this tag the index of the first matching cl_device_id and the number of matching cl_device_id.
   * 
   * 2 induction variables to go over OpenCL information: (explicit loops)
   *   - i: OpenCL Platforms
   *   - j: OpenCL Devices (for each platform[i])
   *
   * 3 mapping coordinates: (obtained by lookup)
   *   - r: plaform, platforms_desc[r]
   *   - t: model,   platforms_desc[r].devices_desc[t]
   *
   * 3 indexes and counters
   *   - idx : index of the first cl_device_id that match platform[i]
   *   - base_t : index of the first cl_device_id matching platforms_desc[r].devices_desc[t]
   *   - cnt_t  : number of devices matching platforms_desc[r].devices_desc[t]
   */
  unsigned idx = 0;
  for (i = 0; i < acc_runtime.opencl_data->num_platforms; i++) {

    unsigned r = acc_get_platform_desc(acc_runtime.opencl_data->platforms[i]);

    acc_register_platform_desc(r, idx, acc_runtime.opencl_data->num_devices[i]);

    unsigned base_t = idx;

    unsigned cnt_t = 1;
 
    // First device

    unsigned t = acc_get_device_desc(&(acc_runtime.opencl_data->devices[i][0]), r);

    // Remaining devices, assumes that they are sorted by type then name.

    for (j = 1; j < acc_runtime.opencl_data->num_devices[i]; j++) {
      unsigned prev_t = t;

      t = acc_get_device_desc(&(acc_runtime.opencl_data->devices[i][j]), r);

      // When the device model change
      if (t != prev_t) {
        acc_register_device_desc(r, prev_t, base_t, cnt_t);
        base_t += cnt_t;
        cnt_t = 0;
      }

      cnt_t++;
    }

    acc_register_device_desc(r, t, base_t, cnt_t);

    idx += acc_runtime.opencl_data->num_devices[i];

    assert( (base_t + cnt_t == idx) );
  }

  set_flag(f_acc_devices);
}

