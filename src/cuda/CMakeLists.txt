add_library(chiefray_cuda STATIC
    trace_rays.cu
)

set_target_properties(chiefray_cuda PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)


target_include_directories(chiefray_cuda
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(chiefray_cuda PUBLIC CUDA::cudart)
