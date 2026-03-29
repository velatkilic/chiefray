#ifndef CHIEFRAY_SURFACES_HPP
#define CHIEFRAY_SURFACES_HPP

#include "chiefray/core/vec3d.hpp"
#include "chiefray/core/mat3d.hpp"

#include <array>

namespace chiefray {

    CHIEFRAY_HD [[nodiscard]] inline Mat3D rotationMatrixFromEulerAngles(double alpha, double beta, double gamma) noexcept {
        const double cos_alpha = cos(alpha);
        const double sin_alpha = sin(alpha);
        const double cos_beta = cos(beta);
        const double sin_beta = sin(beta);
        const double cos_gamma = cos(gamma);
        const double sin_gamma = sin(gamma);

        const double a11 = (cos_alpha * cos_gamma) + (sin_alpha * sin_beta * sin_gamma);
        const double a12 = -cos_beta * sin_gamma;
        const double a13 = -(sin_alpha * cos_gamma) + (cos_alpha * sin_beta * sin_gamma);
        const double a21 = (cos_alpha * sin_gamma) - (sin_alpha * sin_beta * cos_gamma);
        const double a22 = cos_beta * cos_gamma;
        const double a23 = -(sin_alpha * sin_gamma) - cos_alpha * sin_beta * cos_gamma;
        const double a31 = sin_alpha * cos_beta;
        const double a32 = sin_beta;
        const double a33 = cos_alpha * cos_beta;

        return Mat3D{a11, a12, a13, a21, a22, a23, a31, a32, a33};
    }

    // A surface that is rotationally symmetric about the z-axis defined by Spencer-Murty
    // F(X,Y,Z) = Z - c p^2/(1 + sqrt(1 - K c^2 p^2)) - Σ(i=1..N)[a_i p^(2i)] = 0
    // where p^2 = X^2 + Y^2.
    // K < 0     : hyperboloid
    // K = 0     : paraboloid
    // 0 < K < 1 : hemelipsoid of revolution about major axis
    // K = 1     : hemisphere
    // K > 1     : hemelipsoid of revolution about minor axis
    // TODO: implement faster simpler versions from the paper
    struct FAndGradDotDir {
        double f;
        double gradDotDir;
    };

    template <size_t numPolynomialCoefficients>
    class RotationallySymmetricSurface {
    public:
        Mat3D rotationMatrix;
        Vec3D position;
        double c = 0.;
        double k = 0.;
        std::array<double, numPolynomialCoefficients> a;

        CHIEFRAY_HD RotationallySymmetricSurface(
            double c_, double k_, const std::array<double, numPolynomialCoefficients>& a_, 
            const Mat3D& rotationMatrix_, const Vec3D& position_ = Vec3D{0., 0., 0.}) noexcept
            : rotationMatrix(rotationMatrix_), position(position_), c(c_), k(k_), a(a_) {}

        CHIEFRAY_HD RotationallySymmetricSurface(
            double c_, double k_, const std::array<double, numPolynomialCoefficients>& a_, 
            double alpha_, double beta_, double gamma_, const Vec3D& position_ = Vec3D{0., 0., 0.}) noexcept
            : rotationMatrix(rotationMatrixFromEulerAngles(alpha_, beta_, gamma_), position_), c(c_), k(k_), a(a_) {}

        CHIEFRAY_HD double evalF(const Vec3D& point) const noexcept {
            double rhoSquared = point.x*point.x + point.y*point.y; 
            double out = point.z;

            // conic
            out -= c * rhoSquared / (1.0 + sqrt(1.0 - k * c * c * rhoSquared));

            // aspheric
            double rho2_power = rhoSquared;
            for (size_t i = 0; i < numPolynomialCoefficients; ++i) {
                // faster version of: out -= a_[i] * std::pow(rhoSquared, i + 1);
                out -= a[i] * rho2_power;
                rho2_power *= rhoSquared;
            }

            return out;
        }

        CHIEFRAY_HD Vec3D evalGradF(const Vec3D& point) const noexcept {
            double rhoSquared = point.x*point.x + point.y*point.y; 
            
            // conic
            double constE = c / sqrt(1 - k * c * c * rhoSquared);

            // aspheric
            double rho2_power = 1.0;
            for (size_t i = 0; i < numPolynomialCoefficients; ++i) {
                // faster version of: constE += 2 * (i + 1) * a_[i] * std::pow(rhoSquared, i);
                constE += 2.0 * static_cast<double>(i + 1) * a[i] * rho2_power;
                rho2_power *= rhoSquared;
            }

            double gradX = -constE * point.x;
            double gradY = -constE * point.y;
            
            return {gradX, gradY, 1.};
        }
        
        // Eval both F and gradDotDir at the same time for faster computation
        CHIEFRAY_HD [[nodiscard]] constexpr FAndGradDotDir evalFAndGradDotDir(const Vec3D& point, const Vec3D& dir) const noexcept {
            double rhoSquared = point.x * point.x + point.y * point.y;
            double sqrtTerm = sqrt(1.0 - k * c * c * rhoSquared);
        
            double f = point.z - c * rhoSquared / (1.0 + sqrtTerm);
            double constE = c / sqrtTerm;
        
            double rho2_power = 1.0;
            for (size_t i = 0; i < numPolynomialCoefficients; ++i) {
                f -= a[i] * rho2_power * rhoSquared; 
                constE += 2.0 * static_cast<double>(i + 1) * a[i] * rho2_power;
                rho2_power *= rhoSquared;
            }
        
            double gradDotDir = -constE * (point.x * dir.x + point.y * dir.y) + dir.z;
            return {f, gradDotDir};
        }
    };

    class FlatSurface {
    public:
        Mat3D rotationMatrix;
        Vec3D position;

        CHIEFRAY_HD FlatSurface(const Mat3D& rotationMatrix_, const Vec3D& position_ = Vec3D{0., 0., 0.}) noexcept
            : rotationMatrix(rotationMatrix_), position(position_) {}

        CHIEFRAY_HD double evalF(const Vec3D& point) const noexcept {
            return point.z;
        }

        CHIEFRAY_HD Vec3D evalGradF(const Vec3D& point) const noexcept {
            return {0., 0., 1.};
        }

        // Eval both F and gradDotDir at the same time for faster computation
        CHIEFRAY_HD [[nodiscard]] constexpr FAndGradDotDir evalFAndGradDotDir(const Vec3D& point, const Vec3D& dir) const noexcept {
            return {point.z, dir.z};
        }
    };

} // namespace chiefray

#endif // CHIEFRAY_SURFACES_HPP