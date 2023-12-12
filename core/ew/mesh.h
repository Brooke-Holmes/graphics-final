/*
	Author: Eric Winebrenner
*/

#pragma once
#include "ewMath/ewMath.h"

namespace ew {
	struct Vertex {
		ew::Vec3 pos;
		ew::Vec3 normal;
		ew::Vec2 uv;

		inline ew::Vec3 getPos()const { return pos; }
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	enum class DrawMode {
		TRIANGLES = 0,
		POINTS = 1
	};

	class Mesh {
	public:
		Mesh();
		Mesh(const MeshData& meshData);
		void load(const MeshData& meshData);
		void draw(DrawMode drawMode = DrawMode::TRIANGLES)const;
		inline int getNumVertices()const { return m_numVertices; }
		inline int getNumIndices()const { return m_numIndices; }
		inline int getVBO()const { return m_vbo; }
		void getPosData(std::vector<ew::Vec3>& positions)//returns all the positions
		{
			for (int i = 0; i < m_numVertices; i++) 
			{
				positions.push_back(theData.vertices[i].pos);
			}
		}
		void planeCorners(ew::Vec3* corners, int width = 6) //width should be the number of columns
		{
			int end = m_numVertices-1;

			corners[0] = theData.vertices[0].pos;
			corners[1] = theData.vertices[(int)width-1].pos;
			corners[2] = theData.vertices[end-width].pos;
			corners[3] = theData.vertices[end].pos;		}
		ew::MeshData getData() { return theData; }
	private:
		bool m_initialized = false;
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		int m_numVertices = 0;
		int m_numIndices = 0;
		MeshData theData;
	};
}