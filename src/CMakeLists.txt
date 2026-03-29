add_library(chiefray INTERFACE)

target_include_directories(chiefray
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)