/* File generated by Driver<Model>::getFileID("kernels.cl") */
__kernel void kernel_0x2934310_0(unsigned long long param_n,float scalar_offset,__global float *data_a,__constant struct acc_context_t_ *context);

__kernel void kernel_0x2934310_0(unsigned long long param_n,float scalar_offset,__global float *data_a,__constant struct acc_context_t_ *context)
{
  unsigned long local_it_i_tile_0;
  unsigned long local_it_i_gang;
  unsigned long local_it_i_worker;
  unsigned long local_it_i_tile_2;
  for (local_it_i_tile_0 = context -> loops[0] . original . lower; local_it_i_tile_0 < context -> loops[0] . original . lower + context -> loops[0] . tiles[0] . length; local_it_i_tile_0 += context -> loops[0] . tiles[0] . stride) {
    local_it_i_gang = acc_gang_iteration(context,0,local_it_i_tile_0);
    local_it_i_worker = acc_worker_iteration(context,0,local_it_i_gang);
    for (local_it_i_tile_2 = local_it_i_worker; local_it_i_tile_2 < local_it_i_worker + context -> loops[0] . tiles[4] . length; local_it_i_tile_2 += context -> loops[0] . tiles[4] . stride) {
      data_a[local_it_i_tile_2] += scalar_offset;
    }
  }
}
