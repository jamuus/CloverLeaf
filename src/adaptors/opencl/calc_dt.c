

void kernel calc_dt_kernel(
    int x_min, int x_max,
    int y_min, int y_max,
    const global double* xarea,
    const global double* yarea,
    const global double* celldx,
    const global double* celldy,
    const global double* volume,
    const global double* density0,
    const global double* energy0 ,
    const global double* pressure,
    const global double* viscosity,
    const global double* soundspeed,
    const global double* xvel0,
    const global double* yvel0,
    global double* dtmin)
{
    int k = get_global_id(1) + y_min;
    int j = get_global_id(0) + x_min;

    double val = calc_dt_kernel_c_(
                     j, k,
                     x_min, x_max,
                     y_min, y_max,
                     xarea,
                     yarea,
                     celldx,
                     celldy,
                     volume,
                     density0,
                     energy0 ,
                     pressure,
                     viscosity,
                     soundspeed,
                     xvel0,
                     yvel0,
                     dtmin);
    WORK_ARRAY(dtmin, j, k) = val;
}

// __kernel void reduce(__global float* buffer,
//                      __local float* scratch,
//                      __const int length,
//                      __global float* result)
// {

//     int global_index = get_global_id(0);
//     float accumulator = INFINITY;
//     // Loop sequentially over chunks of input vector
//     while (global_index < length) {
//         float element = buffer[global_index];
//         accumulator = (accumulator < element) ? accumulator : element;
//         global_index += get_global_size(0);
//     }

//     // Perform parallel reduction
//     int local_index = get_local_id(0);
//     scratch[local_index] = accumulator;
//     barrier(CLK_LOCAL_MEM_FENCE);
//     for (int offset = get_local_size(0) / 2; offset > 0; offset = offset / 2) {
//         if (local_index < offset) {
//             float other = scratch[local_index + offset];
//             float mine = scratch[local_index];
//             scratch[local_index] = (mine < other) ? mine : other;
//         }
//         barrier(CLK_LOCAL_MEM_FENCE);
//     }
//     if (local_index == 0) {
//         result[get_group_id(0)] = scratch[0];
//     }
// }