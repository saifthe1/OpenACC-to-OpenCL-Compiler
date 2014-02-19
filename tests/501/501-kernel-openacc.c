
void kernel_101(
  unsigned long n, float * a, float offset,
  unsigned long num_gang, unsigned long num_worker, unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i;

  acc_timer_start(data_timer);

  #pragma acc data copy(a[0:n])
  { // (1)

    acc_timer_start(comp_timer);

    #pragma acc parallel present_or_copy(a[0:n]) \
                         devices(acc_nvidia_device, 0, acc_xeonphi_device, 0) \
                         num_gangs(16, 32) num_workers(1024, 128) vector_length(1, 1)
    { // (2)
      #pragma acc loop gang worker vector split(contiguous, 3, 2)
      for (i = 0; i < n; i++)
        a[i] += offset;
    } // (2)

    acc_timer_stop(comp_timer);

  } // (1)

  acc_timer_stop(data_timer);
}

/*! @} */

