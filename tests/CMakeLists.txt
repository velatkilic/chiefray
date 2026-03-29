add_executable(chiefray_tests)

target_sources(chiefray_tests
    PRIVATE
        test_vec3d.cpp
        test_mat3d.cpp
        test_surfaces.cpp
)

target_link_libraries(chiefray_tests
    PRIVATE
        chiefray
        Catch2::Catch2WithMain
)