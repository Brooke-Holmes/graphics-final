//By Autumn Miranda
#include "myProcGen.h"
#include "../anm/myMesh.h"
#include "../ew/ewMath/ewMath.h"

anm::MeshData anm::createNoisePlane(float width, float height, int subdivisions, bool perSegment, std::vector<ew::Vec2>& points)
{
	anm::MeshData plane;
	if (subdivisions <= 0) { subdivisions = 1; }

	//Plane Vertices
	for (int row = 0; row <= subdivisions; row++) {
		for (int col = 0; col <= subdivisions; col++) {
			for (size_t row = 0; row <= subdivisions; row++)
			{
				for (size_t col = 0; col <= subdivisions; col++)
				{
					anm:Vertex v;
					//This section was taken from ew::Mesh because I wanted the planes to all line up without needing a transform
					v.uv.x = ((float)col / subdivisions);
					v.uv.y = ((float)row / subdivisions);
					v.pos.x = -width / 2 + width * v.uv.x;
					v.pos.y = 0;
					v.pos.z = height / 2 - height * v.uv.y;
					v.normal = ew::Vec3(0, 1, 0);


					if (perSegment)
					{
						v.uv = ew::Vec2((col), (row));
					}
					else
					{
						v.uv = ew::Vec2((col / static_cast<float>(subdivisions)), (row / static_cast<float>(subdivisions)));;
					}

					float xDist = v.pos.x - points[0].x;
					float yDist = v.pos.z - points[0].y;
					float distance = sqrt((pow(xDist, 2.0f) + pow(yDist, 2.0f)));
					//this is un-optimized, but it finds the shortest distance between this vertex and the point closest
					for (int j = 1; j < points.size(); j++)
					{
						xDist = v.pos.x - points[j].x;
						yDist = v.pos.z - points[j].y;
						float newDist = sqrt((pow(xDist, 2.0f) + pow(yDist, 2.0f)));
						if (newDist < distance) { distance = newDist; }
					}
					v.dist = distance;
					plane.vertices.push_back(v);
				}
			}
			
		}
	}//end of vertices

	//Plane indices
	int columns = subdivisions + 1;
	for (int row = 0; row < subdivisions; row++) {
		for (int col = 0; col < subdivisions; col++) {
			int start = row * columns + col;
			//bottom triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start + 1);
			plane.indices.push_back(start + columns + 1);

			//top triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start + columns + 1);
			plane.indices.push_back(start + columns);

		}
	}//end of indices


	return plane;
};


anm::MeshData anm::createPlane(float width, float height, int subdivisions, bool perSegment)
{
	anm::MeshData plane;
	if (subdivisions <= 0) { subdivisions = 1; }

	//Plane Vertices
	for (int row = 0; row <= subdivisions; row++) {
		for (int col = 0; col <= subdivisions; col++) {
			ew::Vec3 pos;
			anm::Vertex vertex;
			pos.x = width * (col / static_cast<float>(subdivisions));
			pos.y = 0.0f;
			pos.z = -height * (row / static_cast<float>(subdivisions));
			vertex.pos = pos;
			vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
			if (perSegment) 
			{
				vertex.uv = ew::Vec2((col), (row));
			}
			else 
			{
				vertex.uv = ew::Vec2((col / static_cast<float>(subdivisions)), (row / static_cast<float>(subdivisions)));;
			}
			vertex.dist = -1.0f;//im using this as a null value
			plane.vertices.push_back(vertex);
		}
	}//end of vertices

	//Plane indices
	int columns = subdivisions + 1;
	for (int row = 0; row < subdivisions; row++) {
		for (int col = 0; col < subdivisions; col++) {
			int start = row * columns + col;
			//bottom triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start + 1);
			plane.indices.push_back(start + columns + 1);

			//top triangle
			plane.indices.push_back(start);
			plane.indices.push_back(start + columns + 1);
			plane.indices.push_back(start + columns);

		}
	}//end of indices


	return plane;
};
