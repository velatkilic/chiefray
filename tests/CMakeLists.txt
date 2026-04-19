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

if(CHIEFRAY_ENABLE_WARNINGS)
  target_set_warnings(chiefray_tests)
endif()

if(CHIEFRAY_ENABLE_SANITIZERS)
  target_enable_sanitizers(chiefray_tests)
endif()

include(Catch)
catch_discover_tests(chiefray_tests DISCOVERY_MODE PRE_TEST)