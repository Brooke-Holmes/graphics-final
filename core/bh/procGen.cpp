#include "procGen.h"
#include "transformations.h"
#include "../ew/transform.h"
#include "../ew/mesh.h"
#include <cmath>

namespace bh {

    ew::MeshData createSphere(float radius, int numSegments) {
        ew::MeshData meshData;

        for (int i = 0; i <= numSegments; i++) {
            float theta = i * (2 * ew::PI) / numSegments;
            for (int j = 0; j <= numSegments; j++) {
                float phi = j * ew::PI / numSegments;
                ew::Vertex v;
                v.pos.x = radius * sin(phi) * cos(theta);
                v.pos.y = radius * cos(phi);
                v.pos.z = -radius * sin(phi) * sin(theta);
                v.normal = ew::Normalize(v.pos);
                v.uv.x = static_cast<float>(i) / numSegments;
                v.uv.y = static_cast<float>(j) / numSegments;
                meshData.vertices.push_back(v);
            }
        }

        for (int i = 0; i < numSegments; i++) {
            for (int j = 0; j < numSegments; j++) {
                int first = (i * (numSegments + 1)) + j;
                int second = first + numSegments + 1;

                meshData.indices.push_back(first);
                meshData.indices.push_back(second + 1);
                meshData.indices.push_back(second);

                meshData.indices.push_back(first);
                meshData.indices.push_back(first + 1);
                meshData.indices.push_back(second + 1);
            }
        }

        return meshData;
    }

    ew::MeshData createCylinder(float height, float radius, int numSegments) {
        ew::MeshData meshData;

        for (int i = 0; i <= numSegments; i++) {
            float theta = i * (2 * ew::PI) / numSegments;
            float x = radius * cos(theta);
            float z = -radius * sin(theta);

            ew::Vertex topVertex;
            topVertex.pos = { x, height / 2, z };
            topVertex.normal = ew::Normalize({ x, 0, z });
            topVertex.uv = { static_cast<float>(i) / numSegments, 0.0f };

            ew::Vertex bottomVertex;
            bottomVertex.pos = { x, -height / 2, z };
            bottomVertex.normal = ew::Normalize({ x, 0, z });
            bottomVertex.uv = { static_cast<float>(i) / numSegments, 1.0f };

            meshData.vertices.push_back(topVertex);
            meshData.vertices.push_back(bottomVertex);
        }

        for (int i = 0; i < numSegments; i++) {
            int current = i * 2;
            int next = (i + 1) * 2;
            // Triangle 1
            meshData.indices.push_back(current);
            meshData.indices.push_back(next);
            meshData.indices.push_back(current + 1);
            // Triangle 2
            meshData.indices.push_back(current + 1);
            meshData.indices.push_back(next);
            meshData.indices.push_back(next + 1);
        }

        ew::Vertex topCenter, bottomCenter;
        topCenter.pos = { 0, height / 2, 0 };
        topCenter.normal = { 0, 1, 0 };
        topCenter.uv = { 0.5f, 1.0f };

        bottomCenter.pos = { 0, -height / 2, 0 };
        bottomCenter.normal = { 0, -1, 0 };
        bottomCenter.uv = { 0.5f, 0.0f };

        meshData.vertices.push_back(topCenter);
        meshData.vertices.push_back(bottomCenter);

        int baseIndex = (numSegments + 1) * 2;
        for (int i = 0; i < numSegments; i++) {
            int current = i * 2;
            int next = (i + 1) * 2;
            // Top face
            meshData.indices.push_back(baseIndex);
            meshData.indices.push_back(next);
            meshData.indices.push_back(current);
            // Bottom face
            meshData.indices.push_back(baseIndex + 1);
            meshData.indices.push_back(current + 1);
            meshData.indices.push_back(next + 1);
        }

        return meshData;
    }

}
