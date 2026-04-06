add_library(chiefray STATIC)

add_subdirectory(io)

target_include_directories(chiefray
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(chiefray
    PUBLIC
        Eigen3::Eigen
    PRIVATE
        TIFF::TIFF
)
