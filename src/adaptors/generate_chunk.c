#include "../kernels/generate_chunk_kernel_c.c"


#if defined(USE_KOKKOS)
void generate_chunk(
    int tile,
    struct chunk_type chunk,
    double* state_density,
    double* state_energy,
    double* state_xvel,
    double* state_yvel,
    double* state_xmin,
    double* state_xmax,
    double* state_ymin,
    double* state_ymax,
    double* state_radius,
    int* state_geometry)
{
    for (int k = chunk.tiles[tile].t_ymin - 2; k <= chunk.tiles[tile].t_ymax + 2; k++) {
        for (int j = chunk.tiles[tile].t_xmin - 2; j <= chunk.tiles[tile].t_xmax + 2; j++) {
            generate_chunk_1_kernel(
                j, k,
                chunk.tiles[tile].t_xmin,
                chunk.tiles[tile].t_xmax,
                chunk.tiles[tile].t_ymin,
                chunk.tiles[tile].t_ymax,
                chunk.tiles[tile].field.energy0,
                chunk.tiles[tile].field.density0,
                chunk.tiles[tile].field.xvel0,
                chunk.tiles[tile].field.yvel0,
                state_energy,
                state_density,
                state_xvel,
                state_yvel
            );
        }
    }

    /* State 1 is always the background state */
    for (int state = 2; state <= number_of_states; state++) {
        double x_cent = state_xmin[FTNREF1D(state, 1)];
        double y_cent = state_ymin[FTNREF1D(state, 1)];

        for (int k = chunk.tiles[tile].t_ymin - 2; k <= chunk.tiles[tile].t_ymax + 2; k++) {
            for (int j = chunk.tiles[tile].t_xmin - 2; j <= chunk.tiles[tile].t_xmax + 2; j++) {
                generate_chunk_kernel_c_(
                    j, k,
                    chunk.tiles[tile].t_xmin,
                    chunk.tiles[tile].t_xmax,
                    chunk.tiles[tile].t_ymin,
                    chunk.tiles[tile].t_ymax,
                    x_cent, y_cent,
                    chunk.tiles[tile].field.vertexx,
                    chunk.tiles[tile].field.vertexy,
                    chunk.tiles[tile].field.cellx,
                    chunk.tiles[tile].field.celly,
                    chunk.tiles[tile].field.density0,
                    chunk.tiles[tile].field.energy0,
                    chunk.tiles[tile].field.xvel0,
                    chunk.tiles[tile].field.yvel0,
                    number_of_states,
                    state,
                    state_density,
                    state_energy,
                    state_xvel,
                    state_yvel,
                    state_xmin,
                    state_xmax,
                    state_ymin,
                    state_ymax,
                    state_radius,
                    state_geometry);
            };
        }
    }
}
#endif


#if defined(USE_OPENMP) || defined(USE_OMPSS)
void generate_chunk(
    int tile,
    struct chunk_type chunk,
    double* state_density,
    double* state_energy,
    double* state_xvel,
    double* state_yvel,
    double* state_xmin,
    double* state_xmax,
    double* state_ymin,
    double* state_ymax,
    double* state_radius,
    int* state_geometry)
{
    #pragma omp parallel
    {

        DOUBLEFOR(chunk.tiles[tile].t_ymin - 2,
        chunk.tiles[tile].t_ymax + 2,
        chunk.tiles[tile].t_xmin - 2,
        chunk.tiles[tile].t_xmax + 2, {
            generate_chunk_1_kernel(
                j, k,
                chunk.tiles[tile].t_xmin,
                chunk.tiles[tile].t_xmax,
                chunk.tiles[tile].t_ymin,
                chunk.tiles[tile].t_ymax,
                chunk.tiles[tile].field.energy0,
                chunk.tiles[tile].field.density0,
                chunk.tiles[tile].field.xvel0,
                chunk.tiles[tile].field.yvel0,
                state_energy,
                state_density,
                state_xvel,
                state_yvel
            );
        });

        /* State 1 is always the background state */
        for (int state = 2; state <= number_of_states; state++)
        {
            double x_cent = state_xmin[FTNREF1D(state, 1)];
            double y_cent = state_ymin[FTNREF1D(state, 1)];


            DOUBLEFOR(chunk.tiles[tile].t_ymin - 2,
            chunk.tiles[tile].t_ymax + 2,
            chunk.tiles[tile].t_xmin - 2,
            chunk.tiles[tile].t_xmax + 2, {
                generate_chunk_kernel_c_(
                    j, k,
                    chunk.tiles[tile].t_xmin,
                    chunk.tiles[tile].t_xmax,
                    chunk.tiles[tile].t_ymin,
                    chunk.tiles[tile].t_ymax,
                    x_cent, y_cent,
                    chunk.tiles[tile].field.vertexx,
                    chunk.tiles[tile].field.vertexy,
                    chunk.tiles[tile].field.cellx,
                    chunk.tiles[tile].field.celly,
                    chunk.tiles[tile].field.density0,
                    chunk.tiles[tile].field.energy0,
                    chunk.tiles[tile].field.xvel0,
                    chunk.tiles[tile].field.yvel0,
                    number_of_states,
                    state,
                    state_density,
                    state_energy,
                    state_xvel,
                    state_yvel,
                    state_xmin,
                    state_xmax,
                    state_ymin,
                    state_ymax,
                    state_radius,
                    state_geometry);
            };
        });
    }
}
#endif

#if defined(USE_OPENCL)
void generate_chunk(
    int tile,
    struct chunk_type chunk,
    double* state_density,
    double* state_energy,
    double* state_xvel,
    double* state_yvel,
    double* state_xmin,
    double* state_xmax,
    double* state_ymin,
    double* state_ymax,
    double* state_radius,
    int* state_geometry)
{
    DOUBLEFOR(chunk.tiles[tile].t_ymin - 2,
              chunk.tiles[tile].t_ymax + 2,
              chunk.tiles[tile].t_xmin - 2,
    chunk.tiles[tile].t_xmax + 2, {
        generate_chunk_1_kernel(
            j, k,
            chunk.tiles[tile].t_xmin,
            chunk.tiles[tile].t_xmax,
            chunk.tiles[tile].t_ymin,
            chunk.tiles[tile].t_ymax,
            chunk.tiles[tile].field.energy0,
            chunk.tiles[tile].field.density0,
            chunk.tiles[tile].field.xvel0,
            chunk.tiles[tile].field.yvel0,
            state_energy,
            state_density,
            state_xvel,
            state_yvel
        );
    });

    /* State 1 is always the background state */
    for (int state = 2; state <= number_of_states; state++) {
        double x_cent = state_xmin[FTNREF1D(state, 1)];
        double y_cent = state_ymin[FTNREF1D(state, 1)];


        DOUBLEFOR(chunk.tiles[tile].t_ymin - 2,
                  chunk.tiles[tile].t_ymax + 2,
                  chunk.tiles[tile].t_xmin - 2,
        chunk.tiles[tile].t_xmax + 2, {
            generate_chunk_kernel_c_(
                j, k,
                chunk.tiles[tile].t_xmin,
                chunk.tiles[tile].t_xmax,
                chunk.tiles[tile].t_ymin,
                chunk.tiles[tile].t_ymax,
                x_cent, y_cent,
                chunk.tiles[tile].field.vertexx,
                chunk.tiles[tile].field.vertexy,
                chunk.tiles[tile].field.cellx,
                chunk.tiles[tile].field.celly,
                chunk.tiles[tile].field.density0,
                chunk.tiles[tile].field.energy0,
                chunk.tiles[tile].field.xvel0,
                chunk.tiles[tile].field.yvel0,
                number_of_states,
                state,
                state_density,
                state_energy,
                state_xvel,
                state_yvel,
                state_xmin,
                state_xmax,
                state_ymin,
                state_ymax,
                state_radius,
                state_geometry);
        };
    });
}
#endif