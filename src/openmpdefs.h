
struct field_type {
    double* density0;
    double* density1;
    double* energy0; double* energy1;
    double* pressure;
    double* viscosity;
    double* soundspeed;
    double* xvel0; double* xvel1;
    double* yvel0; double* yvel1;
    double* vol_flux_x; double* mass_flux_x;
    double* vol_flux_y; double* mass_flux_y;
    //node_flux; stepbymass; volume_change; pre_vo
    double* work_array1;
    //node_mass_post; post_vol
    double* work_array2;
    //node_mass_pre; pre_mass
    double* work_array3;
    //advec_vel; post_mass
    double* work_array4;
    //mom_flux; advec_vol
    double* work_array5;
    //pre_vol; post_ener
    double* work_array6;
    //post_vol; ener_flux
    double* work_array7;
    double* cellx;
    double* celly;
    double* vertexx;
    double* vertexy;
    double* celldx;
    double* celldy;
    double* vertexdx;
    double* vertexdy;
    double* volume;
    double* xarea;
    double* yarea;
};

#define DOUBLEFOR(k_from, k_to, j_from, j_to, body) \
    _Pragma("omp for") \
    for(int k = (k_from); k <= (k_to); k++) { \
        _Pragma("ivdep") \
        for(int j = (j_from); j <= (j_to); j++) { \
            body ;\
        } \
    }

#define DENSITY0(d, i, j)      T1ACCESS(d, i, j)
#define DENSITY1(d, i, j)      T1ACCESS(d, i, j)

#define ENERGY0(d, i, j)       T1ACCESS(d, i, j)
#define ENERGY1(d, i, j)       T1ACCESS(d, i, j)
#define PRESSURE(d, i, j)      T1ACCESS(d, i, j)
#define VISCOSITY(d, i, j)     T1ACCESS(d, i, j)
#define SOUNDSPEED(d, i, j)    T1ACCESS(d, i, j)

#define VEL(d, i, j)           T2ACCESS(d, i, j)
#define XVEL0(d, i, j)         VEL(d, i, j)
#define XVEL1(d, i, j)         VEL(d, i, j)
#define YVEL0(d, i, j)         VEL(d, i, j)
#define YVEL1(d, i, j)         VEL(d, i, j)

#define VOL_FLUX_X(d, i, j)    T2ACCESS(d, i, j)
#define MASS_FLUX_X(d, i, j)   T2ACCESS(d, i, j)
#define VOL_FLUX_Y(d, i, j)    T1ACCESS(d, i, j)
#define MASS_FLUX_Y(d, i, j)   T1ACCESS(d, i, j)

#define CONSTFIELDPARAM     const double* __restrict__
#define FIELDPARAM          double* __restrict__

