#include "camera.h"
#include "transformations.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include <cmath>

ew::Mat4 Camera::ViewMatrix()
{
    return LookAt(position, target, ew::Vec3{0, 1, 0});
}

ew::Mat4 Camera::ProjectionMatrix()
{
    if (orthographic)
    {
        return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
    }
    else
    {
        float fovRad = fov * (3.14159f / 180.0f);
        return Perspective(fovRad, aspectRatio, nearPlane, farPlane);
    }
}
