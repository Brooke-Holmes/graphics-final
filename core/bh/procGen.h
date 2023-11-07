#pragma once
#include "../ew/mesh.h"
#include "../ew/transform.h"
#include "../ew/mesh.h"

namespace bh {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}
