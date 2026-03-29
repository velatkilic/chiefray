#ifndef CHIEFRAY_MAT3D_HPP
#define CHIEFRAY_MAT3D_HPP

#include "platform.hpp"
#include "vec3d.hpp"
#include <array>
#include <cassert>

namespace chiefray {
    class Mat3D {
    public:
        std::array<double, 9> data;

        CHIEFRAY_HD constexpr Mat3D() noexcept
                : data{1.0, 0.0, 0.0,
                    0.0, 1.0, 0.0,
                    0.0, 0.0, 1.0} {}

        CHIEFRAY_HD constexpr Mat3D(double m11, double m12, double m13,
                        double m21, double m22, double m23,
                        double m31, double m32, double m33) noexcept
                : data{m11, m12, m13,
                    m21, m22, m23,
                    m31, m32, m33} {}

        CHIEFRAY_HD constexpr double& operator()(int i, int j) noexcept {
            assert(i >= 0 && i < 3 && j >= 0 && j < 3);
            return data[static_cast<size_t>(i * 3 + j)];
        }

        CHIEFRAY_HD constexpr const double& operator()(int i, int j) const noexcept {
            assert(i >= 0 && i < 3 && j >= 0 && j < 3);
            return data[static_cast<size_t>(i * 3 + j)];
        }

        CHIEFRAY_HD constexpr Vec3D operator*(const Vec3D& v) const noexcept {
            return {data[0] * v.x + data[1] * v.y + data[2] * v.z,
                    data[3] * v.x + data[4] * v.y + data[5] * v.z,
                    data[6] * v.x + data[7] * v.y + data[8] * v.z};
        }

        CHIEFRAY_HD constexpr Vec3D transposeMultiply(const Vec3D& v) const noexcept {
            return {data[0] * v.x + data[3] * v.y + data[6] * v.z,
                    data[1] * v.x + data[4] * v.y + data[7] * v.z,
                    data[2] * v.x + data[5] * v.y + data[8] * v.z};
        }

        CHIEFRAY_HD constexpr Mat3D operator*(const Mat3D& b) const noexcept {
            return {
                data[0]*b.data[0] + data[1]*b.data[3] + data[2]*b.data[6],
                data[0]*b.data[1] + data[1]*b.data[4] + data[2]*b.data[7],
                data[0]*b.data[2] + data[1]*b.data[5] + data[2]*b.data[8],
                data[3]*b.data[0] + data[4]*b.data[3] + data[5]*b.data[6],
                data[3]*b.data[1] + data[4]*b.data[4] + data[5]*b.data[7],
                data[3]*b.data[2] + data[4]*b.data[5] + data[5]*b.data[8],
                data[6]*b.data[0] + data[7]*b.data[3] + data[8]*b.data[6],
                data[6]*b.data[1] + data[7]*b.data[4] + data[8]*b.data[7],
                data[6]*b.data[2] + data[7]*b.data[5] + data[8]*b.data[8]
            };
        }

        CHIEFRAY_HD constexpr Mat3D operator*(double s) const noexcept {
            return {data[0]*s, data[1]*s, data[2]*s,
                    data[3]*s, data[4]*s, data[5]*s,
                    data[6]*s, data[7]*s, data[8]*s};
        }

        CHIEFRAY_HD constexpr Mat3D operator/(double s) const noexcept {
            return {data[0]/s, data[1]/s, data[2]/s,
                    data[3]/s, data[4]/s, data[5]/s,
                    data[6]/s, data[7]/s, data[8]/s};
        }

        CHIEFRAY_HD constexpr Mat3D& operator*=(double s) noexcept {
            for (auto& v : data) v *= s;
            return *this;
        }

        CHIEFRAY_HD constexpr Mat3D& operator/=(double s) noexcept {
            for (auto& v : data) v /= s;
            return *this;
        }

        CHIEFRAY_HD constexpr Mat3D operator+(const Mat3D& o) const noexcept {
            return {data[0]+o.data[0], data[1]+o.data[1], data[2]+o.data[2],
                    data[3]+o.data[3], data[4]+o.data[4], data[5]+o.data[5],
                    data[6]+o.data[6], data[7]+o.data[7], data[8]+o.data[8]};
        }

        CHIEFRAY_HD constexpr Mat3D operator-(const Mat3D& o) const noexcept {
            return {data[0]-o.data[0], data[1]-o.data[1], data[2]-o.data[2],
                    data[3]-o.data[3], data[4]-o.data[4], data[5]-o.data[5],
                    data[6]-o.data[6], data[7]-o.data[7], data[8]-o.data[8]};
        }

        CHIEFRAY_HD constexpr Mat3D& operator+=(const Mat3D& o) noexcept {
            for (size_t i = 0; i < 9; ++i) data[i] += o.data[i];
            return *this;
        }

        CHIEFRAY_HD constexpr Mat3D& operator-=(const Mat3D& o) noexcept {
            for (size_t i = 0; i < 9; ++i) data[i] -= o.data[i];
            return *this;
        }

        CHIEFRAY_HD constexpr double determinant() const noexcept {
            return data[0] * (data[4]*data[8] - data[5]*data[7])
                 - data[1] * (data[3]*data[8] - data[5]*data[6])
                 + data[2] * (data[3]*data[7] - data[4]*data[6]);
        }

        CHIEFRAY_HD constexpr Mat3D inverse() const noexcept {
            const double det = determinant();
            assert(det != 0.0);
            const double inv_det = 1.0 / det;
            return {
                (data[4]*data[8] - data[5]*data[7]) * inv_det,
                (data[2]*data[7] - data[1]*data[8]) * inv_det,
                (data[1]*data[5] - data[2]*data[4]) * inv_det,
                (data[5]*data[6] - data[3]*data[8]) * inv_det,
                (data[0]*data[8] - data[2]*data[6]) * inv_det,
                (data[2]*data[3] - data[0]*data[5]) * inv_det,
                (data[3]*data[7] - data[4]*data[6]) * inv_det,
                (data[1]*data[6] - data[0]*data[7]) * inv_det,
                (data[0]*data[4] - data[1]*data[3]) * inv_det
            };
        }
    };

    CHIEFRAY_HD constexpr Mat3D operator*(double s, const Mat3D& m) noexcept {
        return m * s;
    }

} // namespace chiefray

#endif // CHIEFRAY_MAT3D_HPP