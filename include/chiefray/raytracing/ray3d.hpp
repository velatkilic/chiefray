#ifndef CHIEFRAY_RAY3D_HPP
#define CHIEFRAY_RAY3D_HPP

#include "vec3d.hpp"
#include "surfaces.hpp"
#include "mat3d.hpp"

namespace chiefray {
    // Status of the ray tracing operation
    enum class RayTracingStatus {
        Null,
        Success,
        MaxIterationsReached,
        SurfaceIntersectionFailed,
    };

    class Ray3D {
    public:
        Vec3D position;
        Vec3D direction;
        RayTracingStatus status = RayTracingStatus::Null;

        CHIEFRAY_HD constexpr Ray3D(const Vec3D& position, const Vec3D& direction) noexcept
            : position(position), direction(direction) {}
        
            CHIEFRAY_HD constexpr Ray3D() noexcept = default;

            CHIEFRAY_HD [[nodiscard]] constexpr Ray3D transformCoordinatesGlobalToLocal(const Surface& surface) const noexcept {
            const Mat3D& rotationMatrix = surface.rotationMatrix;
            const Vec3D& surfacePosition = surface.position;
            const Vec3D localPosition = rotationMatrix * (position - surfacePosition);
            const Vec3D localDirection = rotationMatrix * direction;
            return Ray3D(localPosition, localDirection);
        }

        CHIEFRAY_HD [[nodiscard]] constexpr Ray3D transformCoordinatesLocalToGlobal(const Surface& surface) const noexcept {
            const Mat3D& rotationMatrix = surface.rotationMatrix;
            const Vec3D& surfacePosition = surface.position;
            const Vec3D globalPosition = rotationMatrix.transposeMultiply(position) + surfacePosition;
            const Vec3D globalDirection = rotationMatrix.transposeMultiply(direction);
            return Ray3D(globalPosition, globalDirection);
        }

    };

    // Intersects the ray with the surface using a Newton-Raphson method
    // Returns the local ray after intersection in local coordinates (since after intersection, refraction, etc is applied in local coordinates)
    template <size_t MaxIterations = 10, typename SurfaceType>
    CHIEFRAY_HD [[nodiscard]] constexpr Ray3D intersectSurface(const Ray3D& ray,const SurfaceType& surface, double err_tol = 1e-6) noexcept {
        Ray3D localRay = ray.transformCoordinatesGlobalToLocal(surface);
        Vec3D& localPosition = localRay.position;
        Vec3D& localDirection = localRay.direction;
        
        // Rare edge case, where ray is propagating perpendicular to the surface
        if (localDirection.z == 0.) {
            localRay.status = RayTracingStatus::SurfaceIntersectionFailed;
            return localRay;
        }

        double s0 = - localPosition.z / localDirection.z;
        double x1 = localPosition.x + s0 * localDirection.x;
        double y1 = localPosition.y + s0 * localDirection.y;
        double s = 0.;

        double err = 2 * err_tol;
        size_t iterations = 0;
        while (fabs(err) > err_tol && iterations < MaxIterations) {
            localPosition.x = x1 + s * localDirection.x;
            localPosition.y = y1 + s * localDirection.y;
            localPosition.z = s * localDirection.z;
            FAndGradDotDir fAndGradDotDir = surface.evalFAndGradDotDir(localPosition, localDirection);
            // Rare edge case, where ray is propagating perpendicular to the surface
            if (fAndGradDotDir.gradDotDir == 0.) {
                localRay.status = RayTracingStatus::SurfaceIntersectionFailed;
                return localRay;
            }
            err = fAndGradDotDir.f / fAndGradDotDir.gradDotDir;
            s -= err;
            iterations++;
        }

        // If max iterations reached, return the local ray with the status set to MaxIterationsReached
        if (iterations == MaxIterations) {
            localRay.status = RayTracingStatus::MaxIterationsReached;
        }

        return localRay;
    }

    // Computes the refracted direction using a Newton-Raphson method
    template <size_t MaxIterations = 10>
    CHIEFRAY_HD [[nodiscard]] constexpr Vec3D refractDirection(const Vec3D& incidentDirection, const Vec3D& surfaceNormal, double n1, double n2, double err_tol = 1e-6) noexcept {
        double mu = n1 / n2;
        double squaredNorm = surfaceNormal.norm_squared();
        double a = mu * incidentDirection.dot(surfaceNormal) / squaredNorm;
        double b = (mu * mu - 1.) / squaredNorm;

        // This is more precise than the sqrt(b^2 - 4ac) / 2a formula
        double prevGamma = -b/(2 * a);
        double gamma = prevGamma;
        double err = 2 * err_tol;
        size_t iterations = 0;
        double numerator = 0., denominator = 0.;
        while (fabs(err) > err_tol && iterations < MaxIterations) {
            numerator = prevGamma * prevGamma - b;
            denominator = 2. * (a + prevGamma);
            gamma = numerator / denominator;
            err = gamma - prevGamma;
            prevGamma = gamma;
            iterations++;
        }

        return mu * incidentDirection + gamma * surfaceNormal;
    }
    
    // Computes the reflected direction
    CHIEFRAY_HD [[nodiscard]] constexpr Vec3D reflectDirection(const Vec3D& incidentDirection, const Vec3D& surfaceNormal) noexcept {
        return incidentDirection - 2. * incidentDirection.dot(surfaceNormal) * surfaceNormal;
    }

} // namespace chiefray

#endif // CHIEFRAY_RAY3D_HPP