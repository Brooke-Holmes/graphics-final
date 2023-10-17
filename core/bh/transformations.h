#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include <cmath>
#include "../ew/ewMath/ewMath.h"

namespace bh {
    //Identity matrix
    inline ew::Mat4 Identity() {
        return ew::Mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    };

    //Scale on x,y,z axes
    inline ew::Mat4 Scale(ew::Vec3 s) {
        return ew::Mat4(
            s.x, 0, 0, 0,
            0, s.y, 0, 0,
            0, 0, s.z, 0,
            0, 0, 0, 1
        );
    };

    //Rotation around X axis (pitch) in radians
    inline ew::Mat4 RotateX(float rad) {
        float cosTheta = std::cos(rad);
        float sinTheta = std::sin(rad);
        return ew::Mat4(
            1, 0, 0, 0,
            0, cosTheta, -sinTheta, 0,
            0, sinTheta, cosTheta, 0,
            0, 0, 0, 1
        );
    };

    //Rotation around Y axis (yaw) in radians
    inline ew::Mat4 RotateY(float rad) {
        float cosTheta = std::cos(rad);
        float sinTheta = std::sin(rad);
        return ew::Mat4(
            cosTheta, 0, sinTheta, 0,
            0, 1, 0, 0,
            -sinTheta, 0, cosTheta, 0,
            0, 0, 0, 1
        );
    };

    //Rotation around Z axis (roll) in radians
    inline ew::Mat4 RotateZ(float rad) {
        float cosTheta = std::cos(rad);
        float sinTheta = std::sin(rad);
        return ew::Mat4(
            cosTheta, -sinTheta, 0, 0,
            sinTheta, cosTheta, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    };

    //Translate x,y,z
    inline ew::Mat4 Translate(ew::Vec3 t) {
        return ew::Mat4(
            1, 0, 0, t.x,
            0, 1, 0, t.y,
            0, 0, 1, t.z,
            0, 0, 0, 1
        );
    };
}

//Creates a right-handed view space
//eye = eye (camera) position
//target = position to look at
//up = up axis, usually (0,1,0)
inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up)
{
    ew::Vec3 zaxis = ew::Normalize(eye - target);
    ew::Vec3 xaxis = ew::Normalize(ew::Cross(up, zaxis));
    ew::Vec3 yaxis = ew::Cross(zaxis, xaxis);

    return ew::Mat4
    {
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
        0, 0, 0, 1
    };
}

//Orthographic projection
inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
{
    float top = height / 2.0f;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    return ew::Mat4
    {
        2.0f / (right - left), 0, 0, 0,
        0, 2.0f / (top - bottom), 0, 0,
        0, 0, -2.0f / (far - near), 0,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
    };
}

//Perspective projection
//fov = vertical aspect ratio (radians)
inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
    float perspective = tan(fov / 2.0f);
    return ew::Mat4{
        1.0f / (aspect * perspective), 0, 0, 0,
        0, 1.0f / perspective, 0, 0,
        0, 0, -(far + near) / (far - near), -1,
        0, 0, -(2.0f * far * near) / (far - near), 0 };

    struct Transform {
        ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
        ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); // Euler angles (degrees)
        ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);

        ew::Mat4 getModelMatrix() const {
            ew::Mat4 translationMatrix = bh::Translate(position);
            ew::Mat4 rotationMatrix =
                bh::RotateY(rotation.y * (3.14159f / 180.0f)) *
                bh::RotateX(rotation.x * (3.14159f / 180.0f)) *
                bh::RotateZ(rotation.z * (3.14159f / 180.0f));
            ew::Mat4 scaleMatrix = bh::Scale(scale);

            return translationMatrix * rotationMatrix * scaleMatrix;
        }
    };
}