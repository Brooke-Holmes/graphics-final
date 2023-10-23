#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace bh {
	//Identity matrix
	inline ew::Mat4 Identity()
	{
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s)
	{
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad)
	{
		float sinTheta = sin(rad);
		float cosTheta = cos(rad);
		return ew::Mat4(
			1, 0, 0, 0,
			0, cosTheta, -sinTheta, 0,
			0, sinTheta, cosTheta, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad)
	{
		float sinTheta = sin(rad);
		float cosTheta = cos(rad);
		return ew::Mat4(
			cosTheta, 0, sinTheta, 0,
			0, 1, 0, 0,
			-sinTheta, 0, cosTheta, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad)
	{
		float sinTheta = sin(rad);
		float cosTheta = cos(rad);
		return ew::Mat4(
			cosTheta, -sinTheta, 0, 0,
			sinTheta, cosTheta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t)
	{
		return ew::Mat4(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		);
	};


	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const
		{
			return bh::Translate(position) * RotateX(rotation.x) * RotateY(rotation.y) * RotateZ(rotation.z) * bh::Scale(scale) * Identity();
		};
	};

	//Creates a right handed view space
//eye = eye (camera) position
	//target = position to look at
	//up = up axis, usually(0,1,0)
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up)
	{
		ew::Vec3 zaxis = ew::Normalize(target - eye);
		ew::Vec3 xaxis = ew::Normalize(ew::Cross(zaxis, up));
		ew::Vec3 yaxis = ew::Cross(xaxis, zaxis);

		zaxis *= -1;

		ew::Mat4 viewMatrix =
		{
		  ew::Vec4(xaxis.x, yaxis.x, zaxis.x, 0),
		  ew::Vec4(xaxis.y, yaxis.y, zaxis.y, 0),
		  ew::Vec4(xaxis.z, yaxis.z, zaxis.z, 0),
		  ew::Vec4(-ew::Dot(xaxis, eye), -ew::Dot(yaxis, eye), -ew::Dot(zaxis, eye), 1)
		};

		return viewMatrix;
	};
	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
	{
		float left = -height * aspect * 0.5;
		float right = height * aspect * 0.5;
		float bottom = -height * 0.5;
		float top = height * 0.5;

		float x = 2.0 / (right - left);
		float y = 2.0 / (top - bottom);
		float z = -2.0 / (far - near);
		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(far + near) / (far - near);

		return ew::Mat4
		(
			x, 0, 0, tx,
			0, y, 0, ty,
			0, 0, z, tz,
			0, 0, 0, 1
		);

	};
	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		float radFov = ((fov / 180) * ew::PI);
		float tanHalfFov = tan(radFov / 2);

		float x = 1.0 / (aspect * tanHalfFov);
		float y = 1.0 / tanHalfFov;
		float z = -(far + near) / (far - near);
		float t = -(2.0 * far * near) / (far - near);

		return ew::Mat4
		(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, t,
			0, 0, -1, 0
		);
	};
}