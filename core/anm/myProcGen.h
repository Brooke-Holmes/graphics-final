//By Autumn Miranda
#pragma once
#include "../anm/myMesh.h"
#include "../ew/mesh.h"


namespace anm {
	anm::MeshData createPlane(float width, float height, int subdivisions, bool perSegment);
	anm::MeshData createNoisePlane(float width, float height, int subdivisions, bool perSegment, std::vector<ew::Vec2>& points);
}