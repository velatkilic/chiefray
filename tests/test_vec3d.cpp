#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>

#include "chiefray/core/vec3d.hpp"

using namespace chiefray;

TEST_CASE("Vec3D default construction initializes to zero") {
    const Vec3D v{};

    REQUIRE(v.x == 0.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(v.z == 0.0);
}

TEST_CASE("Vec3D supports vector addition and subtraction") {
    const Vec3D a{1.0, 2.0, 3.0};
    const Vec3D b{4.0, -5.0, 6.0};

    const Vec3D sum = a + b;
    REQUIRE(sum.x == 5.0);
    REQUIRE(sum.y == -3.0);
    REQUIRE(sum.z == 9.0);

    const Vec3D diff = a - b;
    REQUIRE(diff.x == -3.0);
    REQUIRE(diff.y == 7.0);
    REQUIRE(diff.z == -3.0);
}

TEST_CASE("Vec3D supports scalar multiplication and division") {
    const Vec3D v{1.5, -2.0, 4.0};

    const Vec3D scaled_right = v * 2.0;
    REQUIRE(scaled_right.x == 3.0);
    REQUIRE(scaled_right.y == -4.0);
    REQUIRE(scaled_right.z == 8.0);

    const Vec3D scaled_left = 2.0 * v;
    REQUIRE(scaled_left.x == 3.0);
    REQUIRE(scaled_left.y == -4.0);
    REQUIRE(scaled_left.z == 8.0);

    const Vec3D divided = v / 2.0;
    REQUIRE(divided.x == 0.75);
    REQUIRE(divided.y == -1.0);
    REQUIRE(divided.z == 2.0);
}

TEST_CASE("Vec3D compound operators mutate in place") {
    Vec3D v{1.0, 2.0, 3.0};

    v += Vec3D{4.0, -2.0, 1.0};
    REQUIRE(v.x == 5.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(v.z == 4.0);

    v -= Vec3D{1.0, 1.0, 2.0};
    REQUIRE(v.x == 4.0);
    REQUIRE(v.y == -1.0);
    REQUIRE(v.z == 2.0);
}

TEST_CASE("Vec3D computes dot products and norms") {
    const Vec3D v{1.0, 2.0, 3.0};
    const Vec3D w{-2.0, 0.5, 4.0};

    REQUIRE(v.dot(w) == 11.0);
    REQUIRE(v.norm_squared() == 14.0);
    REQUIRE_THAT(v.norm(), Catch::Matchers::WithinAbs(std::sqrt(14.0), 1e-12));
}
