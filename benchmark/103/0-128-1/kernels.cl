/* File generated by Driver<Model>::getFileID("kernels.cl") */
__kernel void kernel_0x1aa7640_0(int param_n,__global float *data_a,__global float *data_c,__global float *data_b,__constant struct acc_context_t_ *context);

__kernel void kernel_0x1aa7640_0(int param_n,__global float *data_a,__global float *data_c,__global float *data_b,__constant struct acc_context_t_ *context)
{
  unsigned long local_it_i_tile_0;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_tile_1;
  unsigned long local_it_i_worker;
  for (local_it_i_tile_0 = context -> loops[0] . original . lower; local_it_i_tile_0 < context -> loops[0] . original . lower + context -> loops[0] . tiles[0] . length; local_it_i_tile_0 += context -> loops[0] . tiles[0] . stride) {
    local_it_i_gang = acc_gang_iteration(context,0,local_it_i_tile_0);
    for (local_it_i_tile_1 = local_it_i_gang; local_it_i_tile_1 < local_it_i_gang + context -> loops[0] . tiles[2] . length; local_it_i_tile_1 += context -> loops[0] . tiles[2] . stride) {
      local_it_i_worker = acc_worker_iteration(context,0,local_it_i_tile_1);
      data_c[local_it_i_worker] = data_a[local_it_i_worker] + data_b[local_it_i_worker];
    }
  }
}
