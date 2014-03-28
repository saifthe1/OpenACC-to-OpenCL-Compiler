
__kernel void kernel_0(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j;
  unsigned long local_it_i_tile_0;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_worker;
  unsigned long local_it_k;
  for (local_it_i_tile_0 = context -> loops[0] . original . lower; local_it_i_tile_0 < context -> loops[0] . original . lower + context -> loops[0] . tiles[0] . length; local_it_i_tile_0 += context -> loops[0] . tiles[0] . stride) {
    local_it_i_gang = acc_gang_iteration(context,0,local_it_i_tile_0);
    local_it_i_worker = acc_worker_iteration(context,0,local_it_i_gang);
    for (local_it_j = 0; local_it_j < m; local_it_j += 1) {
      c[(local_it_i_worker - offset_c) * m + local_it_j] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i_worker - offset_c) * m + local_it_j] += a[(local_it_i_worker - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j];
      }
    }
  }
}

__kernel void kernel_1(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_i_worker;
  unsigned long local_it_k;
{
    local_it_i_gang = acc_gang_iteration(context,0,context -> loops[0] . original . lower);
    for (local_it_i_tile_1 = local_it_i_gang; local_it_i_tile_1 < local_it_i_gang + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {
      local_it_i_worker = acc_worker_iteration(context,0,local_it_i_tile_1);
      for (local_it_j = 0; local_it_j < m; local_it_j += 1) {
        c[(local_it_i_worker - offset_c) * m + local_it_j] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_worker - offset_c) * m + local_it_j] += a[(local_it_i_worker - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j];
        }
      }
    }
  }
}

__kernel void kernel_2(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_worker;
  unsigned long local_it_i_tile_2;
  unsigned long local_it_k;
{
    local_it_i_gang = acc_gang_iteration(context,0,context -> loops[0] . original . lower);
    local_it_i_worker = acc_worker_iteration(context,0,local_it_i_gang);
    for (local_it_i_tile_2 = local_it_i_worker; local_it_i_tile_2 < local_it_i_worker + context -> loops[0] . tiles[4] . length; local_it_i_tile_2 += context -> loops[0] . tiles[4] . stride) {
      for (local_it_j = 0; local_it_j < m; local_it_j += 1) {
        c[(local_it_i_tile_2 - offset_c) * m + local_it_j] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_tile_2 - offset_c) * m + local_it_j] += a[(local_it_i_tile_2 - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j];
        }
      }
    }
  }
}

__kernel void kernel_3(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_tile_1;
  unsigned long local_it_j_worker;
  unsigned long local_it_i_tile_0;
  unsigned long local_it_i_gang;
  unsigned long local_it_k;
  for (local_it_i_tile_0 = context -> loops[0] . original . lower; local_it_i_tile_0 < context -> loops[0] . original . lower + context -> loops[0] . tiles[0] . length; local_it_i_tile_0 += context -> loops[0] . tiles[0] . stride) {
    local_it_i_gang = acc_gang_iteration(context,0,local_it_i_tile_0);
    for (local_it_j_tile_1 = context -> loops[1] . original . lower; local_it_j_tile_1 < context -> loops[1] . original . lower + context -> loops[1] . tiles[2] . length; local_it_j_tile_1 += context -> loops[1] . tiles[2] . stride) {
      local_it_j_worker = acc_worker_iteration(context,1,local_it_j_tile_1);
      c[(local_it_i_gang - offset_c) * m + local_it_j_worker] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i_gang - offset_c) * m + local_it_j_worker] += a[(local_it_i_gang - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_worker];
      }
    }
  }
}

__kernel void kernel_4(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_worker;
  unsigned long local_it_j_tile_2;
  unsigned long local_it_i_tile_0;
  unsigned long local_it_i_gang;
  unsigned long local_it_k;
  for (local_it_i_tile_0 = context -> loops[0] . original . lower; local_it_i_tile_0 < context -> loops[0] . original . lower + context -> loops[0] . tiles[0] . length; local_it_i_tile_0 += context -> loops[0] . tiles[0] . stride) {
    local_it_i_gang = acc_gang_iteration(context,0,local_it_i_tile_0);
{
      local_it_j_worker = acc_worker_iteration(context,1,context -> loops[1] . original . lower);
      for (local_it_j_tile_2 = local_it_j_worker; local_it_j_tile_2 < local_it_j_worker + context -> loops[1] . tiles[4] . length; local_it_j_tile_2 += context -> loops[1] . tiles[4] . stride) {
        c[(local_it_i_gang - offset_c) * m + local_it_j_tile_2] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_gang - offset_c) * m + local_it_j_tile_2] += a[(local_it_i_gang - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_tile_2];
        }
      }
    }
  }
}

__kernel void kernel_5(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_tile_1;
  unsigned long local_it_j_worker;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_k;
{
    local_it_i_gang = acc_gang_iteration(context,0,context -> loops[0] . original . lower);
    for (local_it_i_tile_1 = local_it_i_gang; local_it_i_tile_1 < local_it_i_gang + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {
      for (local_it_j_tile_1 = context -> loops[1] . original . lower; local_it_j_tile_1 < context -> loops[1] . original . lower + context -> loops[1] . tiles[2] . length; local_it_j_tile_1 += context -> loops[1] . tiles[2] . stride) {
        local_it_j_worker = acc_worker_iteration(context,1,local_it_j_tile_1);
        c[(local_it_i_tile_1 - offset_c) * m + local_it_j_worker] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_tile_1 - offset_c) * m + local_it_j_worker] += a[(local_it_i_tile_1 - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_worker];
        }
      }
    }
  }
}

__kernel void kernel_6(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_worker;
  unsigned long local_it_j_tile_2;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_k;
{
    local_it_i_gang = acc_gang_iteration(context,0,context -> loops[0] . original . lower);
    for (local_it_i_tile_1 = local_it_i_gang; local_it_i_tile_1 < local_it_i_gang + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {{
        local_it_j_worker = acc_worker_iteration(context,1,context -> loops[1] . original . lower);
        for (local_it_j_tile_2 = local_it_j_worker; local_it_j_tile_2 < local_it_j_worker + context -> loops[1] . tiles[4] . length; local_it_j_tile_2 += context -> loops[1] . tiles[4] . stride) {
          c[(local_it_i_tile_1 - offset_c) * m + local_it_j_tile_2] = 0;
          for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
            c[(local_it_i_tile_1 - offset_c) * m + local_it_j_tile_2] += a[(local_it_i_tile_1 - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_tile_2];
          }
        }
      }
    }
  }
}

__kernel void kernel_7(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_tile_0;
  unsigned long local_it_j_gang;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_i_worker;
  unsigned long local_it_k;
  for (local_it_i_tile_1 = context -> loops[0] . original . lower; local_it_i_tile_1 < context -> loops[0] . original . lower + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {
    local_it_i_worker = acc_worker_iteration(context,0,local_it_i_tile_1);
    for (local_it_j_tile_0 = context -> loops[1] . original . lower; local_it_j_tile_0 < context -> loops[1] . original . lower + context -> loops[1] . tiles[0] . length; local_it_j_tile_0 += context -> loops[1] . tiles[0] . stride) {
      local_it_j_gang = acc_gang_iteration(context,1,local_it_j_tile_0);
      c[(local_it_i_worker - offset_c) * m + local_it_j_gang] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i_worker - offset_c) * m + local_it_j_gang] += a[(local_it_i_worker - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_gang];
      }
    }
  }
}

__kernel void kernel_8(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_gang;
  unsigned long local_it_j_tile_1;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_i_worker;
  unsigned long local_it_k;
  for (local_it_i_tile_1 = context -> loops[0] . original . lower; local_it_i_tile_1 < context -> loops[0] . original . lower + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {
    local_it_i_worker = acc_worker_iteration(context,0,local_it_i_tile_1);
{
      local_it_j_gang = acc_gang_iteration(context,1,context -> loops[1] . original . lower);
      for (local_it_j_tile_1 = local_it_j_gang; local_it_j_tile_1 < local_it_j_gang + context -> loops[1] . tiles[2] . length; local_it_j_tile_1 += context -> loops[1] . tiles[2] . stride) {
        c[(local_it_i_worker - offset_c) * m + local_it_j_tile_1] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_worker - offset_c) * m + local_it_j_tile_1] += a[(local_it_i_worker - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_tile_1];
        }
      }
    }
  }
}

__kernel void kernel_9(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_j_tile_0;
  unsigned long local_it_j_gang;
  unsigned long local_it_i_worker;
  unsigned long local_it_i_tile_2;
  unsigned long local_it_k;
{
    local_it_i_worker = acc_worker_iteration(context,0,context -> loops[0] . original . lower);
    for (local_it_i_tile_2 = local_it_i_worker; local_it_i_tile_2 < local_it_i_worker + context -> loops[0] . tiles[4] . length; local_it_i_tile_2 += context -> loops[0] . tiles[4] . stride) {
      for (local_it_j_tile_0 = context -> loops[1] . original . lower; local_it_j_tile_0 < context -> loops[1] . original . lower + context -> loops[1] . tiles[0] . length; local_it_j_tile_0 += context -> loops[1] . tiles[0] . stride) {
        local_it_j_gang = acc_gang_iteration(context,1,local_it_j_tile_0);
        c[(local_it_i_tile_2 - offset_c) * m + local_it_j_gang] = 0;
        for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
          c[(local_it_i_tile_2 - offset_c) * m + local_it_j_gang] += a[(local_it_i_tile_2 - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_gang];
        }
      }
    }
  }
}

__kernel void kernel_10(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_i_worker;
  unsigned long local_it_i_tile_2;
  unsigned long local_it_k;
  unsigned long local_it_j_gang;
  unsigned long local_it_j_tile_1;
{
    local_it_i_worker = acc_worker_iteration(context,0,context -> loops[0] . original . lower);
    for (local_it_i_tile_2 = local_it_i_worker; local_it_i_tile_2 < local_it_i_worker + context -> loops[0] . tiles[4] . length; local_it_i_tile_2 += context -> loops[0] . tiles[4] . stride) {{
        local_it_j_gang = acc_gang_iteration(context,1,context -> loops[1] . original . lower);
        for (local_it_j_tile_1 = local_it_j_gang; local_it_j_tile_1 < local_it_j_gang + context -> loops[1] . tiles[2] . length; local_it_j_tile_1 += context -> loops[1] . tiles[2] . stride) {
          c[(local_it_i_tile_2 - offset_c) * m + local_it_j_tile_1] = 0;
          for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
            c[(local_it_i_tile_2 - offset_c) * m + local_it_j_tile_1] += a[(local_it_i_tile_2 - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_tile_1];
          }
        }
      }
    }
  }
}

__kernel void kernel_11(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_i;
  unsigned long local_it_k;
  unsigned long local_it_j_tile_0;
  unsigned long local_it_j_gang;
  unsigned long local_it_j_worker;
  for (local_it_i = 0; local_it_i < n; local_it_i += 1) {
    for (local_it_j_tile_0 = context -> loops[1] . original . lower; local_it_j_tile_0 < context -> loops[1] . original . lower + context -> loops[1] . tiles[0] . length; local_it_j_tile_0 += context -> loops[1] . tiles[0] . stride) {
      local_it_j_gang = acc_gang_iteration(context,1,local_it_j_tile_0);
      local_it_j_worker = acc_worker_iteration(context,1,local_it_j_gang);
      c[(local_it_i - offset_c) * m + local_it_j_worker] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i - offset_c) * m + local_it_j_worker] += a[(local_it_i - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_worker];
      }
    }
  }
}

__kernel void kernel_12(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_i;
  unsigned long local_it_k;
  unsigned long local_it_j_tile_0;
  unsigned long local_it_j_gang;
  unsigned long local_it_j_worker;
  for (local_it_i = 0; local_it_i < n; local_it_i += 1) {
    for (local_it_j_tile_0 = context -> loops[1] . original . lower; local_it_j_tile_0 < context -> loops[1] . original . lower + context -> loops[1] . tiles[0] . length; local_it_j_tile_0 += context -> loops[1] . tiles[0] . stride) {
      local_it_j_gang = acc_gang_iteration(context,1,local_it_j_tile_0);
      local_it_j_worker = acc_worker_iteration(context,1,local_it_j_gang);
      c[(local_it_i - offset_c) * m + local_it_j_worker] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i - offset_c) * m + local_it_j_worker] += a[(local_it_i - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_worker];
      }
    }
  }
}

__kernel void kernel_13(
  int n,
  int m,
  int p,
  __global float * a,
  int offset_a,
  __global float * b,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long local_it_i;
  unsigned long local_it_k;
  unsigned long local_it_j_tile_0;
  unsigned long local_it_j_gang;
  unsigned long local_it_j_worker;
  for (local_it_i = 0; local_it_i < n; local_it_i += 1) {
    for (local_it_j_tile_0 = context -> loops[1] . original . lower; local_it_j_tile_0 < context -> loops[1] . original . lower + context -> loops[1] . tiles[0] . length; local_it_j_tile_0 += context -> loops[1] . tiles[0] . stride) {
      local_it_j_gang = acc_gang_iteration(context,1,local_it_j_tile_0);
      local_it_j_worker = acc_worker_iteration(context,1,local_it_j_gang);
      c[(local_it_i - offset_c) * m + local_it_j_worker] = 0;
      for (local_it_k = 0; local_it_k < p; local_it_k += 1) {
        c[(local_it_i - offset_c) * m + local_it_j_worker] += a[(local_it_i - offset_a) * p + local_it_k] * b[local_it_k * m + local_it_j_worker];
      }
    }
  }
}

