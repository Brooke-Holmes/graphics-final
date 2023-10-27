#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"

struct Camera {
    ew::Vec3 position; //Camera body position
    struct Camera {
        bool orthographic; //Perspective or orthographic?
        float orthoSize; //Height of orthographic frustum
        float mouseSensitivity = 0.5f;
        float moveSpeed = 0.5f;

        ew::Mat4 ViewMatrix();
        ew::Mat4 ProjectionMatrix();
        ew::Mat4 ViewMatrix()
        {
            return LookAt(position, target, ew::Vec3(0, 1, 0));
        };

        ew::Mat4 ProjectionMatrix()
        {
            switch (orthographic)
            {
            case true:
                return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
                break;
            case false:
                return Perspective(fov, aspectRatio, nearPlane, farPlane);
                break;
            }
        };
    };

