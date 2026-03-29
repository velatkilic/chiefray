#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "chiefray/core/mat3d.hpp"
#include "chiefray/core/vec3d.hpp"
#include "chiefray/raytracing/surfaces.hpp"
#include "utils4tests.hpp"

#include <cmath>

using namespace chiefray;

namespace {
    template <typename SurfaceType>
    Vec3D finite_difference_grad(const SurfaceType& surface, const Vec3D& p, double h = 1e-6) {
        return {
            (surface.evalF({p.x + h, p.y, p.z}) - surface.evalF({p.x - h, p.y, p.z})) / (2.0 * h),
            (surface.evalF({p.x, p.y + h, p.z}) - surface.evalF({p.x, p.y - h, p.z})) / (2.0 * h),
            (surface.evalF({p.x, p.y, p.z + h}) - surface.evalF({p.x, p.y, p.z - h})) / (2.0 * h)};
    }

    Mat3D rotation_x(double angle) {
        const double c = std::cos(angle);
        const double s = std::sin(angle);
        return {1.0, 0.0, 0.0,
                0.0, c, -s,
                0.0, s, c};
    }

    Mat3D rotation_y(double angle) {
        const double c = std::cos(angle);
        const double s = std::sin(angle);
        return {c, 0.0, s,
                0.0, 1.0, 0.0,
                -s, 0.0, c};
    }

    Mat3D rotation_z(double angle) {
        const double c = std::cos(angle);
        const double s = std::sin(angle);
        return {c, -s, 0.0,
                s, c, 0.0,
                0.0, 0.0, 1.0};
    }
} // namespace

TEST_CASE("rotationMatrixFromEulerAngles matches Z-X-negative-Y composition") {
    const double pi = std::acos(-1.0);

    for (double alpha : {0.0, pi / 4.0, pi / 3.0, pi / 2.0, pi, 3.0 * pi / 2.0}) {
        for (double beta : {0.0, pi / 4.0, pi / 3.0, pi / 2.0, pi, 3.0 * pi / 2.0}) {
            for (double gamma : {0.0, pi / 4.0, pi / 3.0, pi / 2.0, pi, 3.0 * pi / 2.0}) {
                CAPTURE(alpha, beta, gamma);

                const Mat3D r = rotationMatrixFromEulerAngles(alpha, beta, gamma);
                const Mat3D expected = rotation_z(gamma) * rotation_x(beta) * rotation_y(-alpha);

                require_mat_close(r, expected);
            }
        }
    }
}

TEST_CASE("rotationMatrixFromEulerAngles reduces to single-axis rotations") {
    const double pi = std::acos(-1.0);

    for (double alpha : {0.0, pi / 6.0, pi / 4.0, pi / 2.0, pi}) {
        CAPTURE(alpha);
        require_mat_close(rotationMatrixFromEulerAngles(alpha, 0.0, 0.0), rotation_y(-alpha));
    }

    for (double beta : {0.0, pi / 6.0, pi / 4.0, pi / 2.0, pi}) {
        CAPTURE(beta);
        require_mat_close(rotationMatrixFromEulerAngles(0.0, beta, 0.0), rotation_x(beta));
    }

    for (double gamma : {0.0, pi / 6.0, pi / 4.0, pi / 2.0, pi}) {
        CAPTURE(gamma);
        require_mat_close(rotationMatrixFromEulerAngles(0.0, 0.0, gamma), rotation_z(gamma));
    }
}

TEST_CASE("rotationMatrixFromEulerAngles transposeMultiply undoes the rotation") {
    const double pi = std::acos(-1.0);
    const Vec3D v{0.3, -0.4, 1.2};

    for (double alpha : {0.0, pi / 4.0, pi / 2.0, pi}) {
        for (double beta : {0.0, pi / 6.0, pi / 3.0, pi / 2.0}) {
            for (double gamma : {0.0, pi / 5.0, pi / 2.0, pi}) {
                CAPTURE(alpha, beta, gamma);

                const Mat3D r = rotationMatrixFromEulerAngles(alpha, beta, gamma);
                require_vec_close(r.transposeMultiply(r * v), v, 1e-12);
            }
        }
    }
}

TEST_CASE("rotationMatrixFromEulerAngles is identity at zero angles") {
    const Mat3D r = rotationMatrixFromEulerAngles(0.0, 0.0, 0.0);
    require_vec_close(r * Vec3D{0.5, -1.0, 2.0}, {0.5, -1.0, 2.0});
}

TEST_CASE("FlatSurface represents z = 0") {
    const FlatSurface s{Mat3D{}};
    const Vec3D p{3.0, -4.0, 1.5};

    REQUIRE_THAT(s.evalF(p), Catch::Matchers::WithinAbs(1.5, 1e-12));
    require_vec_close(s.evalGradF(p), {0.0, 0.0, 1.0});
}

TEST_CASE("RotationallySymmetricSurface reduces to a flat surface when c and a are zero") {
    const RotationallySymmetricSurface<0> s{0.0, 0.0, {}, Mat3D{}};
    const Vec3D p{2.0, -1.0, 7.25};

    REQUIRE_THAT(s.evalF(p), Catch::Matchers::WithinAbs(7.25, 1e-12));
    require_vec_close(s.evalGradF(p), {0.0, 0.0, 1.0});
}

TEST_CASE("RotationallySymmetricSurface matches the paraboloid special case for k = 0") {
    const RotationallySymmetricSurface<0> s{2.0, 0.0, {}, Mat3D{}};
    const Vec3D p{1.0, 2.0, 10.0}; // rho^2 = 5

    REQUIRE_THAT(s.evalF(p), Catch::Matchers::WithinAbs(5.0, 1e-12));
    require_vec_close(s.evalGradF(p), {-2.0, -4.0, 1.0});
}

TEST_CASE("RotationallySymmetricSurface gradient matches finite differences") {
    const RotationallySymmetricSurface<3> s{0.7, -0.25, {0.1, -0.02, 0.003}, Mat3D{}};
    const Vec3D p{0.3, -0.4, 1.2};

    require_vec_close(s.evalGradF(p), finite_difference_grad(s, p), 1e-6);
}