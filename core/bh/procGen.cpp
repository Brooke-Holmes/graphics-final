#include "procGen.h"
#include "transformations.h"

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
	Transform v;
	for (int row = 0; row <= subdivisions)
	{
		for (int col = 0; col <= subdivisions)
		{
			v.x = width * (col / subdivisions);
			v.z = -height * (row / subdivisions);
			vertices.push_back(v);
		}
	}
	int columns = subDivisions + 1;
		for (int row = 0; row < subDivisions)
		{
			for (int col = 0; col < subDivisions)
			{
				int start = row * columns + col;
					//Bottom right triangle
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + columns + 1);
					//Top left triangle…
				indices.push_back(start);
				indices.push_back(start - 1);
				indices.push_back(start + columns - 1);
			}
		}

	return ew::MeshData();
}
