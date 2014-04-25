
#include "OpenACC/public/arch.h"
#include "OpenACC/private/debug.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/region.h"

#include <stdio.h>

#include <assert.h>

void acc_debug_dump_context(struct acc_region_t_ * region, struct acc_kernel_t_ * kernel, struct acc_context_t_ * context, size_t device_idx) {
  size_t i, j;
  printf("Context for region[%d].kernel[%d] on device #%d:\n", region->desc->id, kernel->desc->id, device_idx);
  for (i = 0; i < context->num_loop; i++) {
    printf("  Loop[%d]:\n", i);
    printf("    context->loops[%d].original.lower  = %d\n", i, context->loops[i].original.lower);
    printf("    context->loops[%d].original.upper  = %d\n", i, context->loops[i].original.upper);
    printf("    context->loops[%d].original.stride = %d\n", i, context->loops[i].original.stride);
    printf("    context->loops[%d].original.nbr_it = %d\n", i, context->loops[i].original.nbr_it);
    for (j = 0; j < 7; j++) {
      printf("       context->loops[%d].tiles[%d].stride = %d\n", i, j, context->loops[i].tiles[j].stride);
      printf("       context->loops[%d].tiles[%d].length = %d\n", i, j, context->loops[i].tiles[j].length);
    }
  }
}

void acc_dbg_dump_runtime() {
  acc_init_once();

  assert(0); /// \todo useless before updating

  printf("Device type : ");
  switch (acc_runtime.curr_device_type) {
    case acc_device_any:    printf("ACC_DEVICE_ANY\n");    break;
    case acc_device_nvidia: printf("ACC_DEVICE_NVIDIA\n"); break;
    case acc_device_amd:    printf("ACC_DEVICE_AMD\n");    break;
    case acc_device_intel:  printf("ACC_DEVICE_INTEL\n");  break;
    default:
      printf("invalid !\n");
  }

  printf("Device Number : %u\n", acc_runtime.curr_device_num);
  
  if (acc_runtime.opencl_data != NULL) {
    printf("OpenCL Data:\n");
    unsigned i, j;
    for (i = 0; i < acc_runtime.opencl_data->num_platforms; i++) {
      char buf[100];
      printf("  Platform %u: \n", i);
      clGetPlatformInfo(acc_runtime.opencl_data->platforms[i], CL_PLATFORM_VENDOR, sizeof(buf), buf, NULL);
      printf("    Vendor: %s\n", buf);
      clGetPlatformInfo(acc_runtime.opencl_data->platforms[i], CL_PLATFORM_NAME, sizeof(buf), buf, NULL);
      printf("    Name: %s\n", buf);
      printf("    Number Devices: %u\n", acc_runtime.opencl_data->num_devices[i]);

      for (j = 0; j < acc_runtime.opencl_data->num_devices[i]; j++) {
        printf("    Device %u: \n", j);
        clGetDeviceInfo((acc_runtime.opencl_data->devices)[i][j], CL_DEVICE_VENDOR, sizeof(buf), buf, NULL);
        printf("      Vendor: %s\n", buf);
        clGetDeviceInfo((acc_runtime.opencl_data->devices)[i][j], CL_DEVICE_NAME, sizeof(buf), buf, NULL);
        printf("      Name: %s\n", buf);
      }
    }
  }
  else
    printf("No OpenCL Data !\n");
}

extern const char * cl_invalid_mem_object_tag = "CL_INVALID_MEM_OBJECT";

const char * acc_ocl_status_to_char(cl_int status) {
  char * status_str;
  switch (status) {
      case CL_DEVICE_NOT_FOUND:                          return (char *)"CL_DEVICE_NOT_FOUND";
      case CL_DEVICE_NOT_AVAILABLE:                      return (char *)"CL_DEVICE_NOT_AVAILABLE";
      case CL_COMPILER_NOT_AVAILABLE:                    return (char *)"CL_COMPILER_NOT_AVAILABLE";
      case CL_MEM_OBJECT_ALLOCATION_FAILURE:             return (char *)"CL_MEM_OBJECT_ALLOCATION_FAILURE";
      case CL_OUT_OF_RESOURCES:                          return (char *)"CL_OUT_OF_RESOURCES";
      case CL_OUT_OF_HOST_MEMORY:                        return (char *)"CL_OUT_OF_HOST_MEMORY";
      case CL_PROFILING_INFO_NOT_AVAILABLE:              return (char *)"CL_PROFILING_INFO_NOT_AVAILABLE";
      case CL_MEM_COPY_OVERLAP:                          return (char *)"CL_MEM_COPY_OVERLAP";
      case CL_IMAGE_FORMAT_MISMATCH:                     return (char *)"CL_IMAGE_FORMAT_MISMATCH";
      case CL_IMAGE_FORMAT_NOT_SUPPORTED:                return (char *)"CL_IMAGE_FORMAT_NOT_SUPPORTED";
      case CL_BUILD_PROGRAM_FAILURE:                     return (char *)"CL_BUILD_PROGRAM_FAILURE";
      case CL_MAP_FAILURE:                               return (char *)"CL_MAP_FAILURE";
      case CL_INVALID_VALUE:                             return (char *)"CL_INVALID_VALUE";
      case CL_INVALID_DEVICE_TYPE:                       return (char *)"CL_INVALID_DEVICE_TYPE";
      case CL_INVALID_PLATFORM:                          return (char *)"CL_INVALID_PLATFORM";
      case CL_INVALID_DEVICE:                            return (char *)"CL_INVALID_DEVICE";
      case CL_INVALID_CONTEXT:                           return (char *)"CL_INVALID_CONTEXT";
      case CL_INVALID_QUEUE_PROPERTIES:                  return (char *)"CL_INVALID_QUEUE_PROPERTIES";
      case CL_INVALID_COMMAND_QUEUE:                     return (char *)"CL_INVALID_COMMAND_QUEUE";
      case CL_INVALID_HOST_PTR:                          return (char *)"CL_INVALID_HOST_PTR";
      case CL_INVALID_MEM_OBJECT:                        return (char *)"CL_INVALID_MEM_OBJECT";
      case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:           return (char *)"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
      case CL_INVALID_IMAGE_SIZE:                        return (char *)"CL_INVALID_IMAGE_SIZE";
      case CL_INVALID_SAMPLER:                           return (char *)"CL_INVALID_SAMPLER";
      case CL_INVALID_BINARY:                            return (char *)"CL_INVALID_BINARY";
      case CL_INVALID_BUILD_OPTIONS:                     return (char *)"CL_INVALID_BUILD_OPTIONS";
      case CL_INVALID_PROGRAM:                           return (char *)"CL_INVALID_PROGRAM";
      case CL_INVALID_PROGRAM_EXECUTABLE:                return (char *)"CL_INVALID_PROGRAM_EXECUTABLE";
      case CL_INVALID_KERNEL_NAME:                       return (char *)"CL_INVALID_KERNEL_NAME";
      case CL_INVALID_KERNEL_DEFINITION:                 return (char *)"CL_INVALID_KERNEL_DEFINITION";
      case CL_INVALID_KERNEL:                            return (char *)"CL_INVALID_KERNEL";
      case CL_INVALID_ARG_INDEX:                         return (char *)"CL_INVALID_ARG_INDEX";
      case CL_INVALID_ARG_VALUE:                         return (char *)"CL_INVALID_ARG_VALUE";
      case CL_INVALID_ARG_SIZE:                          return (char *)"CL_INVALID_ARG_SIZE";
      case CL_INVALID_KERNEL_ARGS:                       return (char *)"CL_INVALID_KERNEL_ARGS";
      case CL_INVALID_WORK_DIMENSION:                    return (char *)"CL_INVALID_WORK_DIMENSION";
      case CL_INVALID_WORK_GROUP_SIZE:                   return (char *)"CL_INVALID_WORK_GROUP_SIZE";
      case CL_INVALID_WORK_ITEM_SIZE:                    return (char *)"CL_INVALID_WORK_ITEM_SIZE";
      case CL_INVALID_GLOBAL_OFFSET:                     return (char *)"CL_INVALID_GLOBAL_OFFSET";
      case CL_INVALID_EVENT_WAIT_LIST:                   return (char *)"CL_INVALID_EVENT_WAIT_LIST";
      case CL_INVALID_EVENT:                             return (char *)"CL_INVALID_EVENT";
      case CL_INVALID_OPERATION:                         return (char *)"CL_INVALID_OPERATION";
      case CL_INVALID_GL_OBJECT:                         return (char *)"CL_INVALID_GL_OBJECT";
      case CL_INVALID_BUFFER_SIZE:                       return (char *)"CL_INVALID_BUFFER_SIZE";
      case CL_INVALID_MIP_LEVEL:                         return (char *)"CL_INVALID_MIP_LEVEL";
      case CL_INVALID_GLOBAL_WORK_SIZE:                  return (char *)"CL_INVALID_GLOBAL_WORK_SIZE";
#ifdef CL_VERSION_1_1
      case CL_MISALIGNED_SUB_BUFFER_OFFSET:              return (char *)"CL_MISALIGNED_SUB_BUFFER_OFFSET";
      case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return (char *)"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
      case CL_INVALID_PROPERTY:                          return (char *)"CL_INVALID_PROPERTY";
#endif
      default:                                           return (char *)"CL_UNKNOWN_ERROR_CODE";
    }
  return (char *)"CL_UNKNOWN_ERROR_CODE";
}

