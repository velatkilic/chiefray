#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <string>

#include "chiefray/core/vec3d.hpp"

namespace nb = nanobind;
using namespace chiefray;

using NpVec = nb::ndarray<nb::numpy, double, nb::shape<3>, nb::c_contig, nb::device::cpu>;

void bind_vec3d(nb::module_& m) {
    nb::class_<Vec3D>(m, "Vec3D")
        .def(nb::init<>(), "Default-construct (0, 0, 0)")
        .def(nb::init<double, double, double>(), nb::arg("x"), nb::arg("y"), nb::arg("z"))
        .def("__init__", [](Vec3D* self, NpVec a) {
            new (self) Vec3D(a(0), a(1), a(2));
        }, nb::arg("array"), "Construct from a length-3 numpy array")

        .def_rw("x", &Vec3D::x)
        .def_rw("y", &Vec3D::y)
        .def_rw("z", &Vec3D::z)

        .def("__add__",  [](const Vec3D& a, const Vec3D& b) { return a + b; })
        .def("__sub__",  [](const Vec3D& a, const Vec3D& b) { return a - b; })
        .def("__mul__",  [](const Vec3D& v, double s)        { return v * s; })
        .def("__rmul__", [](const Vec3D& v, double s)        { return s * v; })
        .def("__truediv__", [](const Vec3D& v, double s)     { return v / s; })
        .def("__iadd__", [](Vec3D& a, const Vec3D& b) -> Vec3D& { return a += b; },
             nb::rv_policy::reference)
        .def("__isub__", [](Vec3D& a, const Vec3D& b) -> Vec3D& { return a -= b; },
             nb::rv_policy::reference)

        .def("dot",          &Vec3D::dot,          nb::arg("other"))
        .def("cross",        &Vec3D::cross,        nb::arg("other"))
        .def("norm_squared", &Vec3D::norm_squared)
        .def("norm",         &Vec3D::norm)
        .def("normalized",   &Vec3D::normalized)

        .def("to_numpy", [](const Vec3D& v) {
            double* b = new double[3]{v.x, v.y, v.z};
            nb::capsule own(b, [](void* p) noexcept { delete[] static_cast<double*>(p); });
            return nb::ndarray<nb::numpy, double, nb::shape<3>>(b, {3}, own);
        }, "Return a (3,) numpy array copy")

        .def("__len__", [](const Vec3D&) { return 3; })
        .def("__getitem__", [](const Vec3D& v, int i) -> double {
            if (i == 0) return v.x;
            if (i == 1) return v.y;
            if (i == 2) return v.z;
            throw nb::index_error("Vec3D index out of range");
        })
        .def("__setitem__", [](Vec3D& v, int i, double val) {
            if (i == 0) { v.x = val; return; }
            if (i == 1) { v.y = val; return; }
            if (i == 2) { v.z = val; return; }
            throw nb::index_error("Vec3D index out of range");
        })
        .def("__repr__", [](const Vec3D& v) {
            return "Vec3D(" + std::to_string(v.x) + ", "
                            + std::to_string(v.y) + ", "
                            + std::to_string(v.z) + ")";
        });
}
