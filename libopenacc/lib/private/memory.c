/*!
 * \addtogroup grp_libopenacc_api_public Public
 * @{
 * 
 * \file   lib/private/memory.c
 * \author Tristan Vanderbruggen
 * \date   10/31/2013
 *
 */

#include "OpenACC/openacc.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/private/memory.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/debug.h"
#include "OpenACC/internal/mem-manager.h"
#include "OpenACC/internal/region.h"

#include "OpenACC/utils/profiling.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef PRINT_INFO
# define PRINT_INFO 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

void acc_distributed_data(struct acc_region_t_ * region, size_t device_idx_, h_void ** host_ptr, size_t * n) {
  // Search corresponding "distributed data" entry in region descriptor
  size_t data_idx;
  for (data_idx = 0; data_idx < region->desc->num_distributed_data; data_idx++)
    if (region->distributed_data[data_idx].ptr == *host_ptr)
      break;
  if (data_idx == region->desc->num_distributed_data) return; // No entry found: NOP (data goes entirely to all devices)

  // Check that found "distributed data" entry is valid
  assert( region->desc->distributed_data[data_idx].nbr_dev == region->num_devices &&
          region->desc->distributed_data[data_idx].portions != NULL
        );

  // Search corresponding "device" entry in region descriptor 
  size_t device_idx;
  for (device_idx = 0; device_idx < region->num_devices; device_idx++)
    if (region->devices[device_idx].device_idx == device_idx_)
      break;
  assert(device_idx < region->num_devices);

  // If distributed data is "e_all" it means it is fully transfered to all device
  if (region->desc->distributed_data[data_idx].mode == e_all) {
    // Unless the portion associated to the current device is empty (avoid sending data to unused device)
    if (region->desc->distributed_data[data_idx].portions[device_idx] == 0)
      *n = 0;
    return;
  }

  // Only support contiguous distribution mode at this point (chunk mode to be implemented)
  assert(region->desc->distributed_data[data_idx].mode == e_contiguous);

  // Compute sum of all portions and offset of this device portion (prev_portion)
  unsigned sum_portions = 0;
  unsigned prev_portion = 0;
  size_t k;
  for (k = 0; k < region->num_devices; k++) {
    sum_portions += region->desc->distributed_data[data_idx].portions[k];
    if (k < device_idx)
      prev_portion += region->desc->distributed_data[data_idx].portions[k];
  }
#if PRINT_INFO
  printf("[info]    region[%u] on device #%u distributed data.\n", region->desc->id, device_idx_);
  printf("[info]        host_ptr     = %X\n", *host_ptr);
  printf("[info]        n            = %d\n", *n);
  printf("[info]        sum_portions = %d\n", sum_portions);
  printf("[info]        prev_portion = %d\n", prev_portion);
#endif
  // Update host_ptr and n to cover the desired portion
  *host_ptr += (*n * prev_portion) / sum_portions;
  *n         = (*n * region->desc->distributed_data[data_idx].portions[device_idx]) / sum_portions;

#if PRINT_INFO
  printf("[info]        host_ptr     = %X\n", *host_ptr);
  printf("[info]        n            = %d\n", *n);
#endif
}

d_void * acc_malloc_(size_t device_idx, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_malloc_(size_t device_idx = %u, size_t n = %d)\n", device_idx, n);
#endif

  cl_int status;
	
  cl_mem buffer = clCreateBuffer(
    /* cl_context context  */ acc_runtime.opencl_data->devices_data[device_idx]->context,
    /* cl_mem_flags flags  */ CL_MEM_READ_WRITE,
    /* size_t size         */ n,
    /* void *host_ptr      */ NULL,
    /* cl_int *errcode_ret */ &status
  );
  if (status != CL_SUCCESS) {
    const char * status_str = acc_ocl_status_to_char(status);
    printf("[fatal]   clCreateBuffer return %s for device %u and size %u.\n", status_str, device_idx, (unsigned)n);
    exit(-1); /// \todo error code
  }

#if PRINT_INFO
  printf("[info]      return %X\n", buffer);
#endif

  return (d_void *)buffer;
}

void acc_free_(size_t device_idx, d_void * dev_ptr) {
  cl_int status = clReleaseMemObject((cl_mem)dev_ptr);
  if (status != CL_SUCCESS) {
    const char * status_str = acc_ocl_status_to_char(status);
    printf("[fatal]   clReleaseMemObject return %s for device ptr = %x.\n", status_str, dev_ptr);
    exit(-1); /// \todo error code
  }
}

////////////////////////////////////////////////////////////////////////

d_void * acc_copyin_(size_t device_idx, h_void * host_ptr, size_t n) {

  assert(!acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_malloc_(device_idx, n);

  acc_map_data_(device_idx, host_ptr, dev_ptr, n);

  acc_memcpy_to_device_(device_idx, dev_ptr, host_ptr, n);

  return dev_ptr;
}

void acc_copyin_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_copyin_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_copyin_(region->devices[idx].device_idx, host_ptr, n);
  }
}

d_void * acc_present_or_copyin_(size_t device_idx, h_void * host_ptr, size_t n) {
  if (acc_is_present_(device_idx, host_ptr, n))
    return acc_deviceptr_(device_idx, host_ptr);
  else
    return acc_copyin_(device_idx, host_ptr, n);
}

void acc_present_or_copyin_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_present_or_copyin_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_present_or_copyin_(region->devices[idx].device_idx, host_ptr_, n_);
  }
}

d_void * acc_pcopyin_(size_t device_idx, h_void * host_ptr, size_t n) {
  return acc_present_or_copyin_(device_idx, host_ptr, n);
}

d_void * acc_create_(size_t device_idx, h_void * host_ptr, size_t n) {
  assert(!acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_malloc_(device_idx, n);

  acc_map_data_(device_idx, host_ptr, dev_ptr, n);

  return dev_ptr;
}

void acc_create_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_create_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_create_(region->devices[idx].device_idx, host_ptr_, n_);
  }
}

d_void * acc_present_or_create_(size_t device_idx, h_void * host_ptr, size_t n) {
  if (acc_is_present_(device_idx, host_ptr, n))
    return acc_deviceptr_(device_idx, host_ptr);
  else
    return acc_create_(device_idx, host_ptr, n);
}

void acc_present_or_create_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_present_or_create_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_present_or_create_(region->devices[idx].device_idx, host_ptr_, n_);
  }
}

d_void * acc_pcreate_(size_t device_idx, h_void * host_ptr, size_t n) {
  return acc_present_or_create_(device_idx, host_ptr, n);
}

void acc_copyout_(size_t device_idx, h_void * host_ptr, size_t n) {
  assert(acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_deviceptr_(device_idx, host_ptr);

  acc_memcpy_from_device_(device_idx, host_ptr, dev_ptr, n);
}

void acc_copyout_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_copyout_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_copyout_(region->devices[idx].device_idx, host_ptr_, n_);
  }
}

d_void * acc_present_or_copyout_(size_t device_idx, h_void * host_ptr, size_t n) {
  /// \todo incorrect need to check if {host_ptr, n} is allocated on top of the data-env
  if (acc_is_present_(device_idx, host_ptr, n))
    acc_copyout_(device_idx, host_ptr, n);
  return acc_deviceptr_(device_idx, host_ptr); /// should return NULL if previous executed
}

void acc_present_or_copyout_regions_(struct acc_region_t_ * region, h_void * host_ptr, size_t n) {
#if PRINT_INFO
  printf("[info]  acc_present_or_copyout_regions_(region = #%u, h_void * host_ptr = %X, size_t n = %d)\n", region->desc->id, host_ptr, n);
#endif
  acc_init_region_(region);
  unsigned idx;
  for (idx = 0; idx < region->num_devices; idx++) {
    h_void * host_ptr_ = host_ptr;
    size_t n_ = n;
    acc_distributed_data(region, region->devices[idx].device_idx, &host_ptr_, &n_);
    if (n_ > 0)
      acc_present_or_copyout_(region->devices[idx].device_idx, host_ptr_, n_);
  }
}

d_void * acc_pcopyout_(size_t device_idx, h_void * host_ptr, size_t n) {
  return acc_present_or_copyout_(device_idx, host_ptr, n);
}

void acc_delete_(size_t device_idx, h_void * host_ptr, size_t n) {
  assert(acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_deviceptr_(device_idx, host_ptr);

  acc_unmap_data_(device_idx, host_ptr);

  acc_free_(device_idx, dev_ptr);
}

////////////////////////////////////////////////////////////////////////

void acc_update_device_(size_t device_idx, h_void * host_ptr, size_t n) {
  assert(acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_deviceptr_(device_idx, host_ptr);

  acc_memcpy_to_device_(device_idx, dev_ptr, host_ptr, n);
}

void acc_update_self_(size_t device_idx, h_void * host_ptr, size_t n) {
  assert(acc_is_present_(device_idx, host_ptr, n));

  d_void * dev_ptr = acc_deviceptr_(device_idx, host_ptr);

  acc_memcpy_from_device_(device_idx, host_ptr, dev_ptr, n);
}

////////////////////////////////////////////////////////////////////////

d_void * acc_deviceptr_(size_t device_idx, h_void * host_ptr) {

  d_void * dev_ptr = acc_get_deviceptr(device_idx, host_ptr);

  return dev_ptr;
}

h_void * acc_hostptr_(size_t device_idx, d_void * dev_ptr) {

  h_void * host_ptr = acc_get_hostptr(device_idx, dev_ptr);

  return host_ptr;
}

////////////////////////////////////////////////////////////////////////

int acc_is_present_(size_t device_idx, h_void * host_ptr, size_t n) {;
  return acc_check_present(device_idx, host_ptr, n);
}

////////////////////////////////////////////////////////////////////////

void acc_memcpy_to_device_(size_t device_idx, d_void * dest, h_void * src, size_t bytes) {
  cl_event event;

  cl_int status = clEnqueueWriteBuffer(
    /* cl_command_queue command_queue  */ acc_runtime.opencl_data->devices_data[device_idx]->command_queue,
    /* cl_mem buffer                   */ (cl_mem)dest,
    /* cl_bool blocking_write          */ CL_FALSE,
    /* size_t offset                   */ 0,
    /* size_t cb                       */ bytes,
    /* const void *ptr                 */ src,
    /* cl_uint num_events_in_wait_list */ 0,
    /* const cl_event *event_wait_list */ NULL,
    /* cl_event *event                 */ &event
  );
  if (status != CL_SUCCESS) {
    char * status_str = acc_ocl_status_to_char(status);
    printf("[fatal]   clEnqueueWriteBuffer return %s for host ptr = %X to device ptr = %X of size %u.\n", status_str, (unsigned int)src, (unsigned int)dest, (unsigned int)bytes);
    exit(-1); /// \todo error code
  }

  acc_profiling_register_memcpy_to_device(event, device_idx, dest, src, bytes);
}

void acc_memcpy_from_device_(size_t device_idx, h_void * dest, d_void * src, size_t bytes) {
  cl_event event;

  cl_int status = clEnqueueReadBuffer (
    /* cl_command_queue command_queue */ acc_runtime.opencl_data->devices_data[device_idx]->command_queue,
    /* cl_mem buffer */ (cl_mem)src,
    /* cl_bool blocking_read */ CL_FALSE,
    /* size_t offset */ 0,
    /* size_t cb */ bytes,
    /* void *ptr */ dest,
    /* cl_uint num_events_in_wait_list */ 0,
    /* const cl_event *event_wait_list */ NULL,
    /* cl_event *event */ &event
  );
  if (status != CL_SUCCESS) {
    char * status_str = acc_ocl_status_to_char(status);
    printf("[fatal] clEnqueueReadBuffer return %s for device ptr = %X to host ptr = %X of size %u.\n", status_str, (unsigned int)src, (unsigned int)dest, (unsigned int)bytes);
    exit(-1); /// \todo error code
  }

  acc_profiling_register_memcpy_from_device(event, device_idx, dest, src, bytes);
}

#ifdef __cplusplus
}
#endif

/*! @} */

