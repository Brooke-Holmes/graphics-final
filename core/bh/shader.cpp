#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"

namespace bh {

	struct Camera
	{
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum

		ew::Mat4 ViewMatrix()
		{
			return bh::LookAt(position, target, ew::Vec3(0, 1, 0));
		};

		ew::Mat4 ProjectionMatrix()
		{
			switch (orthographic)
			{
			case true:
				return bh::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
				break;
			case false:
				return bh::Perspective(fov, aspectRatio, nearPlane, farPlane);
				break;
			}
		};
	};

	struct CameraControls
	{
		bool firstMouse;
		double prevMouseX;
		double prevMouseY;
		float yaw;
		float pitch;
		float mouseSensitivity;
		float moveSpeed;
	};
}