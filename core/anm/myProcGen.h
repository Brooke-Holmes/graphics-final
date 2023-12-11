#pragma once
#include "../anm/myMesh.h"
#include "../ew/mesh.h"


namespace anm {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions, bool perSegment);
	anm::MeshData createNoisePlane(float width, float height, int subdivisions, bool perSegment, ew::Vec2* points);
}