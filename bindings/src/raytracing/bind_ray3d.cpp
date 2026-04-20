#include <nanobind/nanobind.h>
#include <string>

#include "chiefray/raytracing/ray3d.hpp"
#include "chiefray/core/vec3d.hpp"

namespace nb = nanobind;
using namespace chiefray;

void bind_ray3d(nb::module_& m) {
    nb::enum_<RayTracingStatus>(m, "RayTracingStatus")
        .value("Null",                      RayTracingStatus::Null)
        .value("Success",                   RayTracingStatus::Success)
        .value("MaxIterationsReached",      RayTracingStatus::MaxIterationsReached)
        .value("SurfaceIntersectionFailed", RayTracingStatus::SurfaceIntersectionFailed)
        .export_values();

    nb::class_<Ray3D>(m, "Ray3D")
        .def(nb::init<>())
        .def(nb::init<const Vec3D&, const Vec3D&>(),
             nb::arg("position"), nb::arg("direction"))
        .def_rw("position",  &Ray3D::position)
        .def_rw("direction", &Ray3D::direction)
        .def_rw("status",    &Ray3D::status)
        .def("__repr__", [](const Ray3D& r) {
            return "Ray3D(position=Vec3D(" +
                   std::to_string(r.position.x) + ", " +
                   std::to_string(r.position.y) + ", " +
                   std::to_string(r.position.z) + "), direction=Vec3D(" +
                   std::to_string(r.direction.x) + ", " +
                   std::to_string(r.direction.y) + ", " +
                   std::to_string(r.direction.z) + "))";
        });
}
