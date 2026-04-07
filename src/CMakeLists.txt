add_library(chiefray STATIC)

add_subdirectory(io)

target_include_directories(chiefray
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(chiefray
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
    FILES
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/vec3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/mat3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/core/platform.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/raytracing/ray3d.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/raytracing/surfaces.hpp
      ${PROJECT_SOURCE_DIR}/include/chiefray/io/tiff_io.hpp
)

target_link_libraries(chiefray
    PUBLIC
        Eigen3::Eigen
    PRIVATE
        TIFF::TIFF
)
