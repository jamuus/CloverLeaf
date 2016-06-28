#include "calc_dt.h"
#include "definitions_c.h"
#include "calc_dt_kernel_c.c"
#include "string.h"


void calc_dt(int *tile,
             double *local_dt,
             char* local_control,
             double *xl_pos,
             double *yl_pos,
             int *jldt,
             int *kldt)
{
    *local_dt = g_big;
    int small = 0,
        l_control;

    calc_dt_kernel_c_(
        &chunk.tiles[*tile].t_xmin,
        &chunk.tiles[*tile].t_xmax,
        &chunk.tiles[*tile].t_ymin,
        &chunk.tiles[*tile].t_ymax,
        &g_small,
        &g_big,
        &dtmin,
        &dtc_safe,
        &dtu_safe,
        &dtv_safe,
        &dtdiv_safe,
        chunk.tiles[*tile].field.xarea,
        chunk.tiles[*tile].field.yarea,
        chunk.tiles[*tile].field.cellx,
        chunk.tiles[*tile].field.celly,
        chunk.tiles[*tile].field.celldx,
        chunk.tiles[*tile].field.celldy,
        chunk.tiles[*tile].field.volume,
        chunk.tiles[*tile].field.density0,
        chunk.tiles[*tile].field.energy0,
        chunk.tiles[*tile].field.pressure,
        chunk.tiles[*tile].field.viscosity,
        chunk.tiles[*tile].field.soundspeed,
        chunk.tiles[*tile].field.xvel0,
        chunk.tiles[*tile].field.yvel0,
        chunk.tiles[*tile].field.work_array1,
        local_dt,
        &l_control,
        xl_pos,
        yl_pos,
        jldt,
        kldt,
        &small
    );

    if (l_control ==  1) strcpy(local_control, "sound");
    if (l_control ==  2) strcpy(local_control, "xvel");
    if (l_control ==  3) strcpy(local_control, "yvel");
    if (l_control ==  4) strcpy(local_control, "div");
}