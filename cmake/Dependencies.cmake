find_package(Eigen3 CONFIG REQUIRED)

if(CHIEFRAY_ENABLE_OMP)
    find_package(OpenMP REQUIRED)
endif()

if(CHIEFRAY_ENABLE_MPI)
    find_package(MPI REQUIRED)
endif()

if(CHIEFRAY_ENABLE_CUDA)
    include(CheckLanguage)
    check_language(CUDA)
    enable_language(CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()

if(CHIEFRAY_ENABLE_TESTS)
    find_package(Catch2 3 CONFIG REQUIRED)
endif()

if(CHIEFRAY_ENABLE_PYTHON)
    find_package(pybind11 CONFIG REQUIRED)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()