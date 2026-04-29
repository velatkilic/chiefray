cmake_minimum_required(VERSION 3.24)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(chiefray
    VERSION 0.1.0
    DESCRIPTION "Chiefray"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(Cuda)
include(Warnings)
include(Sanitizers)
include(StaticAnalysis)
include(Dependencies)

if(CHIEFRAY_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src)

if(CHIEFRAY_ENABLE_PYTHON)
    add_subdirectory(bindings)
endif()