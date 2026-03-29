cmake_minimum_required(VERSION 3.24)

project(chiefray
    VERSION 0.1.0
    DESCRIPTION "Chiefray"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(Warnings)
include(Sanitizers)
include(StaticAnalysis)
include(Dependencies)

if(CHIEFRAY_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src)