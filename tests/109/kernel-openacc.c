
#include "OpenACC/utils/timer.h"

void kernel_109(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang, unsigned long num_worker, unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i, j, k;

  acc_timer_start(data_timer);

  #pragma acc data copyin(a[0:n][0:p], b[0:p][0:m]) copyout(c[0:n][0:m])
  { // (1)

    acc_timer_start(comp_timer);

    #pragma acc parallel present_or_copyin(a[0:n][0:p], b[0:p][0:m]) present_or_copyout(c[0:n][0:m]) num_gang(num_gang) num_worker(num_worker) vector_length(vector_length)
    { // (2)
      #pragma acc loop gang
      for (i = 0; i < n; i++)
        #pragma acc loop worker
        for (j = 0; j < m; j++) {
          c[i][j] = 0.;
          for (k = 0; k < p; k++)
            c[i][j] += a[i][k] * b[k][j];
        }
    } // (2)

    acc_timer_stop(comp_timer);

  } // (1)

  acc_timer_stop(data_timer);
}

/*! @} */

