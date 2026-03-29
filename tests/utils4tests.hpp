#ifndef UTILS4TESTS_HPP
#define UTILS4TESTS_HPP

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "chiefray/core/vec3d.hpp"
#include "chiefray/core/mat3d.hpp"

namespace chiefray {
    inline void require_vec_close(const Vec3D& actual, const Vec3D& expected, double eps = 1e-12) {
        REQUIRE_THAT(actual.x, Catch::Matchers::WithinAbs(expected.x, eps));
        REQUIRE_THAT(actual.y, Catch::Matchers::WithinAbs(expected.y, eps));
        REQUIRE_THAT(actual.z, Catch::Matchers::WithinAbs(expected.z, eps));
    };

    inline void require_mat_close(const Mat3D& actual, const Mat3D& expected, double eps = 1e-12) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                REQUIRE_THAT(actual(i, j), Catch::Matchers::WithinAbs(expected(i, j), eps));
    }
} // namespace chiefray

#endif // UTILS4TESTS_HPP