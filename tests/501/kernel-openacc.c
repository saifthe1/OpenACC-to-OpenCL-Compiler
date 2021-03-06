
#include "OpenACC/utils/timer.h"

void kernel_501(
  unsigned long n, float * a, float offset,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i;

  acc_timer_start(data_timer);

  acc_timer_start(comp_timer);

  #pragma acc parallel present_or_copy(a[0:n]) \
                       devices(acc_nvidia_device, 0, acc_xeonphi_device, 0) \
                       num_gangs(num_gang_0, num_gang_1) \
                       num_workers(num_worker_0, num_worker_1) \
                       vector_length(vector_length_0, vector_length_1)
  #pragma acc loop gang worker split(contiguous, 9, 7) read(a, IDENTITY) write(a, IDENTITY)
  for (i = 0; i < n; i++)
    a[i] += offset;

  acc_timer_stop(comp_timer);

  acc_timer_stop(data_timer);
}

/*! @} */

