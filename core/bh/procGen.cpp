#include "procGen.h"

ew::MeshData myLib::createSphere(float radius, int numSegments)
{
	return ew::MeshData();
}

ew::MeshData myLib::createCylinder(float height, float radius, int numSegments)
{
	return ew::MeshData();
}

ew::MeshData myLib::createPlane(float width, float height, int subdivisions)
{
	for (row = 0; row <= subdivisions)
	{
		for (col = 0; col <= subdivisions)
		{
			v.x = width * (col / subdivisions)
				v.z = -height * (row / subdivisions)
				vertices.push_back(v)
		}
	}
	columns = subDivisions + 1
		for (row = 0; row < subDivisions)
		{
			for (col = 0; col < subDivisions)
			{
				start = row * columns + col
					//Bottom right triangle
					indices.push_back(start)
					indices.push_back(start + 1)
					indices.push_back(start + columns + 1)
					//Top left triangle…
			}
		}

	return ew::MeshData();
}
