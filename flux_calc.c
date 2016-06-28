#include "flux_calc.h"
#include "flux_calc_kernel_c.c"
#include "definitions_c.h"
#include "timer_c.h"


void flux_calc()
{
    double kernel_time;

    if (profiler_on) kernel_time = timer();

    for (int tile = 0; tile < tiles_per_chunk; tile++) {
        flux_calc_kernel_c_(
            &chunk.tiles[tile].t_xmin,
            &chunk.tiles[tile].t_xmax,
            &chunk.tiles[tile].t_ymin,
            &chunk.tiles[tile].t_ymax,
            &dt,
            chunk.tiles[tile].field.xarea,
            chunk.tiles[tile].field.yarea,
            chunk.tiles[tile].field.xvel0,
            chunk.tiles[tile].field.yvel0,
            chunk.tiles[tile].field.xvel1,
            chunk.tiles[tile].field.yvel1,
            chunk.tiles[tile].field.vol_flux_x,
            chunk.tiles[tile].field.vol_flux_y);
    }

    if (profiler_on) profiler.flux += timer() - kernel_time;
}