find_package(Eigen3 CONFIG REQUIRED)
find_package(FFTW3 CONFIG REQUIRED)

if(CHIEFRAY_ENABLE_OMP)
    find_package(OpenMP REQUIRED)
endif()

if(CHIEFRAY_ENABLE_MPI)
    find_package(MPI REQUIRED)
endif()

if(CHIEFRAY_ENABLE_CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()

if(CHIEFRAY_ENABLE_TESTS)
    find_package(Catch2 3 CONFIG REQUIRED)
endif()

if(CHIEFRAY_ENABLE_PYTHON)
    # Try to import all Python components potentially needed by nanobind
    find_package(Python 3.9
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)
    find_package(nanobind CONFIG REQUIRED)
endif()