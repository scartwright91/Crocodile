#pragma once

#include <cmath>

namespace Crocodile
{
    namespace s3d
    {

        class Quaternion {

        public:
            float w, x, y, z;

            Quaternion() : w(1), x(0), y(0), z(0) {}
            Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

            Quaternion operator*(const Quaternion& q) const {
                return Quaternion(
                    w * q.w - x * q.x - y * q.y - z * q.z,
                    w * q.x + x * q.w + y * q.z - z * q.y,
                    w * q.y - x * q.z + y * q.w + z * q.x,
                    w * q.z + x * q.y - y * q.x + z * q.w
                );
            }

            Quaternion conjugate() const {
                return Quaternion(w, -x, -y, -z);
            }

            static Quaternion fromAxisAngle(float angle, float x, float y, float z) {
                float halfAngle = angle * 0.5f;
                float s = sin(halfAngle);
                return Quaternion(cos(halfAngle), x * s, y * s, z * s);
            }

            void normalize() {
                float norm = sqrt(w * w + x * x + y * y + z * z);
                if (norm > 0.0f) {
                    float invNorm = 1.0f / norm;
                    w *= invNorm;
                    x *= invNorm;
                    y *= invNorm;
                    z *= invNorm;
                }
            }
        };

    }
}

