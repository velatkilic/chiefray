add_library(chiefray)

target_include_directories(chiefray
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(chiefray
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
    FILES
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/fft.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/vec3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/mat3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/platform.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/raytracing/ray3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/raytracing/surfaces.hpp
  PRIVATE
    core/fft.cpp
)

target_link_libraries(chiefray
    PUBLIC
        Eigen3::Eigen
    PRIVATE
        FFTW3::fftw3
)

if(CHIEFRAY_ENABLE_OMP)
    target_link_libraries(chiefray PRIVATE OpenMP::OpenMP_CXX)
endif()

if(CHIEFRAY_ENABLE_CUDA)
    target_sources(chiefray
      PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
          ${PROJECT_SOURCE_DIR}/include
        FILES
          ${PROJECT_SOURCE_DIR}/include/chiefray/cuda/launch.hpp
    )

    add_subdirectory(cuda)
    target_link_libraries(chiefray PRIVATE chiefray_cuda)
endif()