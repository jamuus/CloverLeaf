
#include <Kokkos_Core.hpp>
#include "../../kernels/PdV_kernel_c.cc"

using namespace Kokkos;

struct pdv_predict_functor {
    int x_from,
        x_to,
        y_from,
        y_to;
    int x_min,
        x_max,
        y_min,
        y_max;

    field_2d_lt xarea,
                yarea,
                volume,
                density0,
                density1,
                energy0,
                energy1,
                pressure,
                viscosity,
                xvel0,
                xvel1,
                yvel0,
                yvel1,
                work_array1;
    double dt;

    pdv_predict_functor(
        struct tile_type tile,
        int _x_from, int _x_to, int _y_from, int _y_to,
        double _dt):

        x_from(_x_from), x_to(_x_to),
        y_from(_y_from), y_to(_y_to),
        x_min(tile.t_xmin),
        x_max(tile.t_xmax),
        y_min(tile.t_ymin),
        y_max(tile.t_ymax),

        xarea(tile.field.d_xarea),
        yarea(tile.field.d_yarea),
        volume(tile.field.d_volume),
        density0(tile.field.d_density0),
        density1(tile.field.d_density1),
        energy0(tile.field.d_energy0),
        energy1(tile.field.d_energy1),
        pressure(tile.field.d_pressure),
        viscosity(tile.field.d_viscosity),
        xvel0(tile.field.d_xvel0),
        xvel1(tile.field.d_xvel1),
        yvel0(tile.field.d_yvel0),
        yvel1(tile.field.d_yvel1),
        work_array1(tile.field.d_work_array1),

        dt(_dt)
    {}

    void compute()
    {
        parallel_for("pdv_predict", MDRangePolicy<Rank<2>>({y_from, x_from}, {y_to+1, x_to+1}), *this);
    }

    KOKKOS_INLINE_FUNCTION
    void operator()(const int k, const int j) const
    {

            pdv_kernel_predict_c_(
                j, k,
                x_min, x_max, y_min, y_max,
                dt,
                xarea,
                yarea,
                volume,
                density0,
                density1,
                energy0,
                energy1,
                pressure,
                viscosity,
                xvel0,
                xvel1,
                yvel0,
                yvel1,
                work_array1);
    }
};


struct pdv_no_predict_functor {
    int x_from,
        x_to,
        y_from,
        y_to;
    int x_min,
        x_max,
        y_min,
        y_max;

    field_2d_lt xarea,
                yarea,
                volume,
                density0,
                density1,
                energy0,
                energy1,
                pressure,
                viscosity,
                xvel0,
                xvel1,
                yvel0,
                yvel1,
                work_array1;
    double dt;

    pdv_no_predict_functor(
        struct tile_type tile,
        int _x_from, int _x_to, int _y_from, int _y_to,
        double _dt):

        x_from(_x_from), x_to(_x_to), y_from(_y_from), y_to(_y_to),
        x_min(tile.t_xmin),
        x_max(tile.t_xmax),
        y_min(tile.t_ymin),
        y_max(tile.t_ymax),

        xarea(tile.field.d_xarea),
        yarea(tile.field.d_yarea),
        volume(tile.field.d_volume),
        density0(tile.field.d_density0),
        density1(tile.field.d_density1),
        energy0(tile.field.d_energy0),
        energy1(tile.field.d_energy1),
        pressure(tile.field.d_pressure),
        viscosity(tile.field.d_viscosity),
        xvel0(tile.field.d_xvel0),
        xvel1(tile.field.d_xvel1),
        yvel0(tile.field.d_yvel0),
        yvel1(tile.field.d_yvel1),
        work_array1(tile.field.d_work_array1),

        dt(_dt)
    {}

    void compute()
    {
        parallel_for("pdv_no_predict", MDRangePolicy<Rank<2>>({y_from, x_from}, {y_to+1, x_to+1}), *this);
    }

    KOKKOS_INLINE_FUNCTION
    void operator()(const int k, const int j) const
    {
            pdv_kernel_no_predict_c_(
                j, k,
                x_min, x_max, y_min, y_max,
                dt,
                xarea,
                yarea,
                volume,
                density0,
                density1,
                energy0,
                energy1,
                pressure,
                viscosity,
                xvel0,
                xvel1,
                yvel0,
                yvel1,
                work_array1);
    }
};

