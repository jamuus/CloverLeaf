
MPI_CC_INTEL = mpiicpc
MPI_CC_GNU = mpic++
CC_CUDA = nvcc_wrapper
MPI_CC_ = mpic++


FLAGS_GNU = -std=c++11 -Wall -Wpedantic -g -Wno-unknown-pragmas -O3 -march=native -lm
# FLAGS_INTEL = -std=c++11 -O3 -g -restrict -march=native -fp-model strict
FLAGS_INTEL = -std=c++11 -O3 -g -restrict -march=native -no-prec-div -fno-alias
FLAGS_CUDA = 
FLAGS_ = 

MPI_FLAGS = $(FLAGS_$(COMPILER))
MPI_CC = $(MPI_CC_$(COMPILER))

OBJECTS = data_c.o \
	definitions_c.o \
	report.o \
	generate_chunk.o \
	ideal_gas.o \
	timer_c.o \
	viscosity.o \
	calc_dt.o \
	PdV.o \
	revert.o \
	flux_calc.o \
	advection.o \
	reset_field.o \
	visit.o \
	accelerate.o

	# update_tile_halo.o 
MPIOBJECTS = clover.o \
	initialise.o \
	hydro.o \
	update_halo.o \
	timestep.o \
	start.o \
	field_summary.o



default: build

ifdef USE_KOKKOS
    ifeq ($(USE_KOKKOS),gpu)
        CXX = ${KOKKOS_PATH}/bin/nvcc_wrapper
        MPI_FLAGS += -lcudart
        FLAGS += -O3

        KOKKOS_CUDA_OPTIONS = "enable_lambda"
        KOKKOS_DEVICE= "Cuda"
    else
        CXX = $(MPI_CC)
        FLAGS += $(MPI_FLAGS)
    endif
    MPI_FLAGS += -DUSE_KOKKOS
    FLAGS += -DUSE_KOKKOS
    CC = $(CXX)
    include $(KOKKOS_PATH)/Makefile.kokkos

endif

ifdef USE_OMPSS
    CC = mcc
    FLAGS += --ompss $(MPI_FLAGS) -DUSE_OMPSS
    MPI_FLAGS += --ompss -DUSE_OMPSS
endif

ifdef USE_OPENMP
    CC = $(MPI_CC)
    
    FLAGS += -fopenmp $(MPI_FLAGS) -DUSE_OPENMP
    MPI_FLAGS += -fopenmp -DUSE_OPENMP
endif

ifdef USE_OPENCL
    CC = $(MPI_CC)

    ifeq ($(shell uname),Darwin)
        MPI_FLAGS += -framework OpenCL -DUSE_OPENCL
    else
        MPI_FLAGS += -lOpenCL -DUSE_OPENCL
    endif
    FLAGS += $(MPI_FLAGS)
endif

OBJDIR    = obj
MPIOBJDIR = mpiobj
SRCDIR    = src

_OBJECTS = $(addprefix $(OBJDIR)/, $(OBJECTS))
_SOURCES = $(addprefix $(SRCDIR)/, $(OBJECTS:.o=.c))
_MPIOBJECTS = $(addprefix $(MPIOBJDIR)/, $(MPIOBJECTS))
_MPISOURCES = $(addprefix $(SRCDIR)/, $(MPIOBJECTS:.o=.c))

# -include $(_OBJECTS:.o=.d)
# -include $(_MPIOBJECTS:.o=.d)

depend:
	makedepend -- $(FLAGS) -- $(_SOURCES) $(_MPISOURCES)

build: $(_OBJECTS) $(_MPIOBJECTS) Makefile $(KOKKOS_LINK_DEPENDS) $(KERNELS)
	$(MPI_CC) $(MPI_FLAGS) $(KOKKOS_CPPFLAGS) $(EXTRA_PATH) $(_OBJECTS) $(_MPIOBJECTS) $(SRCDIR)/clover_leaf.c $(KOKKOS_LIBS) $(KOKKOS_LDFLAGS) -o clover_leaf

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(KOKKOS_CPP_DEPENDS)
	$(CC) $(FLAGS) $(MPIINCLUDE) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(EXTRA_INC) -c $< -o $@
	# $(CC) $(FLAGS) $(MPIINCLUDE) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(EXTRA_INC) -MM $< > $(OBJDIR)/$*.d

$(MPIOBJDIR)/%.o: $(SRCDIR)/%.c $(KOKKOS_CPP_DEPENDS)
	$(MPI_CC) $(MPI_FLAGS) $(KOKKOS_CPPFLAGS) $(EXTRA_INC) -c $< -o $@
	# $(MPI_CC) $(MPI_FLAGS) $(KOKKOS_CPPFLAGS) $(EXTRA_INC) -MM $< > $(MPIOBJDIR)/$*.d


fast: $(_SOURCES) $(_MPISOURCES) Makefile $(KOKKOS_LINK_DEPENDS) $(KERNELS)
	$(MPI_CC) $(MPI_FLAGS) $(KOKKOS_CPPFLAGS) $(EXTRA_PATH) $(_SOURCES) $(_MPISOURCES) $(SRCDIR)/clover_leaf.c $(KOKKOS_LIBS) $(KOKKOS_LDFLAGS) -o clover_leaf

clean: 
	rm -f $(OBJDIR)/* $(MPIOBJDIR)/* *.o clover_leaf

print-%  : ; @echo $* = $($*)# DO NOT DELETE
