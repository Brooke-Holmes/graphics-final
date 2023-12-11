/*
	Based on mesh.h by Eric Winebrenner
*/

#pragma once
#include "../ew/ewMath/ewMath.h"

namespace anm{
	struct Vertex {
		ew::Vec3 pos;
		ew::Vec3 normal;
		ew::Vec2 uv;
		float dist;
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
		
	private:
		bool m_initialized = false;
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		int m_numVertices = 0;
		int m_numIndices = 0;
	};
}