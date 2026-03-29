#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "chiefray/core/mat3d.hpp"
#include "chiefray/core/vec3d.hpp"
#include "utils4tests.hpp"

using namespace chiefray;

TEST_CASE("Mat3D default constructor yields identity") {
    const Mat3D m{};
    require_vec_close(m * Vec3D{1.25, -2.5, 3.75}, {1.25, -2.5, 3.75});
}

TEST_CASE("Mat3D multiplies vectors in row-major order") {
    const Mat3D m{
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    };

    require_vec_close(m * Vec3D{1.0, -1.0, 2.0}, {5.0, 11.0, 17.0});
}
    
TEST_CASE("Mat3D transposeMultiply multiplies by the transpose") {
    const Mat3D m{
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    };

    require_vec_close(m.transposeMultiply(Vec3D{1.0, -1.0, 2.0}), {11.0, 13.0, 15.0});
}

TEST_CASE("Mat3D identity is neutral for matrix multiplication") {
    const Mat3D identity{};
    const Mat3D m{
        2.0, -1.0, 0.5,
        4.0, 3.0, -2.0,
        -7.0, 8.0, 6.0
    };

    require_mat_close(identity * m, m);
    require_mat_close(m * identity, m);
}

TEST_CASE("Mat3D supports scalar multiplication and division") {
    const Mat3D m{
        1.5, -2.0, 4.0,
        0.5, 3.0, -6.0,
        -8.0, 10.0, 12.0
    };

    require_mat_close(m * 2.0, Mat3D{
        3.0, -4.0, 8.0,
        1.0, 6.0, -12.0,
        -16.0, 20.0, 24.0
    });

    require_mat_close(2.0 * m, Mat3D{
        3.0, -4.0, 8.0,
        1.0, 6.0, -12.0,
        -16.0, 20.0, 24.0
    });

    require_mat_close(m / 2.0, Mat3D{
        0.75, -1.0, 2.0,
        0.25, 1.5, -3.0,
        -4.0, 5.0, 6.0
    });
}

TEST_CASE("Mat3D compound scalar operators mutate in place") {
    Mat3D m{
        1.0, -2.0, 3.0,
        4.0, -5.0, 6.0,
        -7.0, 8.0, -9.0
    };

    m *= 2.0;
    require_mat_close(m, Mat3D{
        2.0, -4.0, 6.0,
        8.0, -10.0, 12.0,
        -14.0, 16.0, -18.0
    });

    m /= 4.0;
    require_mat_close(m, Mat3D{
        0.5, -1.0, 1.5,
        2.0, -2.5, 3.0,
        -3.5, 4.0, -4.5
    });
}

TEST_CASE("Mat3D supports matrix addition and subtraction") {
    const Mat3D a{
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    };
    const Mat3D b{
        9.0, -8.0, 7.0,
        -6.0, 5.0, -4.0,
        3.0, -2.0, 1.0
    };

    require_mat_close(a + b, Mat3D{
        10.0, -6.0, 10.0,
        -2.0, 10.0, 2.0,
        10.0, 6.0, 10.0
    });

    require_mat_close(a - b, Mat3D{
        -8.0, 10.0, -4.0,
        10.0, 0.0, 10.0,
        4.0, 10.0, 8.0
    });
}

TEST_CASE("Mat3D compound matrix operators mutate in place") {
    Mat3D m{
        1.0, 1.0, 1.0,
        2.0, 2.0, 2.0,
        3.0, 3.0, 3.0
    };
    const Mat3D delta{
        0.5, -1.5, 2.0,
        -2.0, 0.0, 4.0,
        1.0, -3.0, 5.0
    };

    m += delta;
    require_mat_close(m, Mat3D{
        1.5, -0.5, 3.0,
        0.0, 2.0, 6.0,
        4.0, 0.0, 8.0
    });

    m -= delta;
    require_mat_close(m, Mat3D{
        1.0, 1.0, 1.0,
        2.0, 2.0, 2.0,
        3.0, 3.0, 3.0
    });
}