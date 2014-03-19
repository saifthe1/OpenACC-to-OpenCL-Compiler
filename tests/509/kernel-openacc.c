
#include "OpenACC/utils/timer.h"

void kernel_509(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i, j, k;

  acc_timer_start(data_timer);

  acc_timer_start(comp_timer);

  #pragma acc parallel present_or_copyin(a[0:n][0:p], b[0:p][0:m]) \
                       present_or_copyout(c[0:n][0:m]) \
                       devices(acc_nvidia_device, 0, acc_xeonphi_device, 0) \
                       num_gangs(num_gang_0, num_gang_1) \
                       num_workers(num_worker_0, num_worker_1) \
                       vector_length(vector_length_0, vector_length_1)
  #pragma acc loop gang split(contiguous, 9, 7) read(a, [i][0:p]) read(b, ALL) write(c, [i][0:m])
  for (i = 0; i < n; i++) {
    #pragma acc loop worker
    for (j = 0; j < m; j++) {
      c[i][j] = 0.;
      for (k = 0; k < p; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }

  acc_timer_stop(comp_timer);

  acc_timer_stop(data_timer);
}

/*! @} */

