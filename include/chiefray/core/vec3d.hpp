#ifndef CHIEFRAY_VEC3D_HPP
#define CHIEFRAY_VEC3D_HPP

#include <cmath>
#include "platform.hpp"

namespace chiefray {
    class Vec3D {
    public:
        double x{0.}, y{0.}, z{0.};

        CHIEFRAY_HD constexpr Vec3D() noexcept = default;

        CHIEFRAY_HD constexpr Vec3D(double x_, double y_, double z_) noexcept
            : x(x_), y(y_), z(z_) {}

        CHIEFRAY_HD constexpr Vec3D operator+(const Vec3D& other) const noexcept {
            return {x + other.x, y + other.y, z + other.z};
        }

        CHIEFRAY_HD constexpr Vec3D operator-(const Vec3D& other) const noexcept {
            return {x - other.x, y - other.y, z - other.z};
        }

        CHIEFRAY_HD constexpr Vec3D operator*(double s) const noexcept {
            return {s*x, s*y, s*z};
        }

        CHIEFRAY_HD constexpr Vec3D operator/(double s) const noexcept {
            return {x/s, y/s, z/s};
        }

        CHIEFRAY_HD constexpr Vec3D& operator+=(const Vec3D& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        CHIEFRAY_HD constexpr Vec3D& operator-=(const Vec3D& rhs) noexcept {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        CHIEFRAY_HD constexpr double dot(const Vec3D& rhs) const noexcept {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        CHIEFRAY_HD constexpr Vec3D cross(const Vec3D& rhs) const noexcept {
            return {y * rhs.z - z * rhs.y,
                    z * rhs.x - x * rhs.z,
                    x * rhs.y - y * rhs.x};
        }

        CHIEFRAY_HD constexpr double norm_squared() const noexcept {
            return dot(*this);
        }

        CHIEFRAY_HD double norm() const {
            return sqrt(norm_squared());
        }

        CHIEFRAY_HD Vec3D normalized() const {
            return *this / norm();
        }

    };

    CHIEFRAY_HD constexpr Vec3D operator*(double s, const Vec3D& vec) noexcept {
        return {s*vec.x, s*vec.y, s*vec.z};
    }
    
} // namespace chiefray

#endif // CHIEFRAY_VEC3D_HPP