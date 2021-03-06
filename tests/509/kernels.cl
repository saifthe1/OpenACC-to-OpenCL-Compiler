/* File generated by Driver<Model>::getFileID("kernels.cl") */

__kernel void kernel_0x3087c70_0(
  int n,
  int m,
  int p,
  __global float * b,
  __global float * a,
  int offset_a,
  __global float * c,
  int offset_c,
  __constant struct acc_context_t_ *context
) {
  unsigned long j_tile_0;
  unsigned long j_worker;
  unsigned long i_tile_0;
  unsigned long i_gang;
  unsigned long k;

  for (
    i_tile_0  = context->loops[0].original.lower;
    i_tile_0  < context->loops[0].original.lower + ctx->loops[0].tiles[e_tile_0].length
    i_tile_0 += context->loops[0].tiles[0].stride
  ) {
    i_gang = acc_gang_iteration(context,0,i_tile_0);
//    if (i_gang >= context->loops[0].original.lower && i_gang < context->loops[0].original.upper) {
      for (
        j_tile_0  = context->loops[1].original.lower;
        j_tile_0  < context->loops[1].original.lower + ctx->loops[1].tiles[e_tile_0].length;
        j_tile_0 += context->loops[1].tiles[0].stride
      ) {
        j_worker = acc_worker_iteration(context,1,j_tile_0);
//        if (j_worker >= context->loops[1].original.lower && j_worker < context->loops[1].original.upper) {
          c[(i_gang - offset_c) * m + j_worker] = 0;
          for (k = 0; k < p; k += 1) {
            c[(i_gang - offset_c) * m + j_worker] += a[(i_gang - offset_a) * p + k] * b[k * m + j_worker];
          }
//        }
      }
//    }
  }
}
