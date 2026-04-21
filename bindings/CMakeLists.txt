set(CHIEFRAY_BINDING_SOURCES
    src/module.cpp
    src/core/bind_vec3d.cpp
    src/core/bind_mat3d.cpp
    src/raytracing/bind_ray3d.cpp
    src/raytracing/bind_surfaces.cpp
    src/raytracing/bind_free_functions.cpp
)

nanobind_add_module(
    _chiefray_ext
    # Target the stable ABI for Python 3.12+, which reduces
    # the number of binary wheels that must be built. This
    # does nothing on older Python versions
    STABLE_ABI

    # Build libnanobind statically and merge it into the
    # extension (which itself remains a shared library)
    #
    # If your project builds multiple extensions, you can
    # replace this flag by NB_SHARED to conserve space by
    # reusing a shared libnanobind across libraries
    NB_STATIC
    
    ${CHIEFRAY_BINDING_SOURCES}
)

target_link_libraries(_chiefray_ext PRIVATE chiefray)
target_include_directories(_chiefray_ext PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(CHIEFRAY_ENABLE_CUDA)
    target_compile_definitions(_chiefray_ext PRIVATE CHIEFRAY_HAS_CUDA_BINDINGS)
    target_sources(_chiefray_ext PRIVATE src/cuda/bind_cuda_batch.cpp)
endif()

if(CHIEFRAY_ENABLE_MPI)
    target_sources(_chiefray_ext PRIVATE src/mpi/bind_mpi_distributor.cpp)
endif()

install(TARGETS _chiefray_ext LIBRARY DESTINATION chiefray COMPONENT python)
install(DIRECTORY python/chiefray/ DESTINATION chiefray COMPONENT python)
