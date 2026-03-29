include(FetchContent)
include(CheckLanguage)

# Try to find packages first, if not fetch
find_package(Eigen3 CONFIG QUIET)
if(NOT TARGET Eigen3::Eigen)
    FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 5.0.1
    )
    FetchContent_MakeAvailable(eigen)
endif()

if(CHIEFRAY_ENABLE_OMP)
    find_package(OpenMP REQUIRED)
endif()

if(CHIEFRAY_ENABLE_MPI)
    find_package(MPI REQUIRED)
endif()

if(CHIEFRAY_ENABLE_CUDA)
    check_language(CUDA)
    enable_language(CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()

if(CHIEFRAY_ENABLE_TESTS)
    find_package(Catch2 3 CONFIG QUIET)
    if(NOT TARGET Catch2::Catch2WithMain)
        FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.13.0
        )
        FetchContent_MakeAvailable(Catch2)
    endif()
endif()

if(CHIEFRAY_ENABLE_PYTHON)
    find_package(pybind11 CONFIG QUIET)
    if(NOT TARGET pybind11::module)
        FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v3.0.1
        )
        FetchContent_MakeAvailable(pybind11)
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON) # needed for python bindings
endif()