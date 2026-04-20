#include <nanobind/nanobind.h>

namespace nb = nanobind;

void bind_vec3d(nb::module_&);
void bind_mat3d(nb::module_&);
void bind_ray3d(nb::module_&);
void bind_surfaces(nb::module_&);
void bind_free_functions(nb::module_&);

#ifdef CHIEFRAY_HAS_CUDA_BINDINGS
void bind_cuda_batch(nb::module_&);
#endif

NB_MODULE(_chiefray_ext, m) {
    m.doc() = "chiefray: C++ raytracing library Python bindings";

    nb::module_ core       = m.def_submodule("core",       "Core linear algebra types");
    nb::module_ raytracing = m.def_submodule("raytracing", "Raytracing surfaces and algorithms");

    bind_vec3d(core);
    bind_mat3d(core);
    bind_ray3d(raytracing);
    bind_surfaces(raytracing);
    bind_free_functions(raytracing);

#ifdef CHIEFRAY_HAS_CUDA_BINDINGS
    nb::module_ cuda = m.def_submodule("cuda", "CUDA-accelerated algorithms");
    bind_cuda_batch(cuda);
#endif
}
