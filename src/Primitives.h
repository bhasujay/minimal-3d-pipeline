#pragma once
#include "Mesh.h"
#include <cmath>

const float PI = 3.14159265358979323846f;

struct TexturedCube : public TexturedMesh {
	TexturedCube(float width, float height, float depth, Color color = Color(1,1,1,1), float uvScaleX = 1.0f, float uvScaleY = 1.0f) {
		float hw = width / 2.0f;
		float hh = height / 2.0f;
		float hd = depth / 2.0f;

		// Front (+Z)
		vertices.push_back({Vec3(-hw, -hh, hd), color, Vec2(0.0f, 0.0f), Vec3(0, 0, 1)});
		vertices.push_back({Vec3( hw, -hh, hd), color, Vec2(uvScaleX, 0.0f), Vec3(0, 0, 1)});
		vertices.push_back({Vec3( hw,  hh, hd), color, Vec2(uvScaleX, uvScaleY), Vec3(0, 0, 1)});
		vertices.push_back({Vec3(-hw,  hh, hd), color, Vec2(0.0f, uvScaleY), Vec3(0, 0, 1)});

		// Back (-Z)
		vertices.push_back({Vec3( hw, -hh, -hd), color, Vec2(0.0f, 0.0f), Vec3(0, 0, -1)});
		vertices.push_back({Vec3(-hw, -hh, -hd), color, Vec2(uvScaleX, 0.0f), Vec3(0, 0, -1)});
		vertices.push_back({Vec3(-hw,  hh, -hd), color, Vec2(uvScaleX, uvScaleY), Vec3(0, 0, -1)});
		vertices.push_back({Vec3( hw,  hh, -hd), color, Vec2(0.0f, uvScaleY), Vec3(0, 0, -1)});

		// Left (-X)
		vertices.push_back({Vec3(-hw, -hh, -hd), color, Vec2(0.0f, 0.0f), Vec3(-1, 0, 0)});
		vertices.push_back({Vec3(-hw, -hh,  hd), color, Vec2(uvScaleX, 0.0f), Vec3(-1, 0, 0)});
		vertices.push_back({Vec3(-hw,  hh,  hd), color, Vec2(uvScaleX, uvScaleY), Vec3(-1, 0, 0)});
		vertices.push_back({Vec3(-hw,  hh, -hd), color, Vec2(0.0f, uvScaleY), Vec3(-1, 0, 0)});

		// Right (+X)
		vertices.push_back({Vec3( hw, -hh,  hd), color, Vec2(0.0f, 0.0f), Vec3(1, 0, 0)});
		vertices.push_back({Vec3( hw, -hh, -hd), color, Vec2(uvScaleX, 0.0f), Vec3(1, 0, 0)});
		vertices.push_back({Vec3( hw,  hh, -hd), color, Vec2(uvScaleX, uvScaleY), Vec3(1, 0, 0)});
		vertices.push_back({Vec3( hw,  hh,  hd), color, Vec2(0.0f, uvScaleY), Vec3(1, 0, 0)});

		// Top (+Y)
		vertices.push_back({Vec3(-hw,  hh,  hd), color, Vec2(0.0f, 0.0f), Vec3(0, 1, 0)});
		vertices.push_back({Vec3( hw,  hh,  hd), color, Vec2(uvScaleX, 0.0f), Vec3(0, 1, 0)});
		vertices.push_back({Vec3( hw,  hh, -hd), color, Vec2(uvScaleX, uvScaleY), Vec3(0, 1, 0)});
		vertices.push_back({Vec3(-hw,  hh, -hd), color, Vec2(0.0f, uvScaleY), Vec3(0, 1, 0)});

		// Bottom (-Y)
		vertices.push_back({Vec3(-hw, -hh, -hd), color, Vec2(0.0f, 0.0f), Vec3(0, -1, 0)});
		vertices.push_back({Vec3( hw, -hh, -hd), color, Vec2(uvScaleX, 0.0f), Vec3(0, -1, 0)});
		vertices.push_back({Vec3( hw, -hh,  hd), color, Vec2(uvScaleX, uvScaleY), Vec3(0, -1, 0)});
		vertices.push_back({Vec3(-hw, -hh,  hd), color, Vec2(0.0f, uvScaleY), Vec3(0, -1, 0)});

		for (int i = 0; i < 6; ++i) {
			unsigned int base = i * 4;
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
			indices.push_back(base + 2);
			indices.push_back(base + 3);
			indices.push_back(base + 0);
		}
	}
};

struct TexturedPyramid : public TexturedMesh {
	TexturedPyramid(float baseWidth, float baseDepth, float height, Color color = Color(1,1,1,1)) {
		float hw = baseWidth / 2.0f;
		float hd = baseDepth / 2.0f;

		Vec3 apex(0, height, 0);
		Vec3 bl(-hw, 0, hd);
		Vec3 br(hw, 0, hd);
		Vec3 tr(hw, 0, -hd);
		Vec3 tl(-hw, 0, -hd);

		auto addTri = [&](const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec2& uv0, const Vec2& uv1, const Vec2& uv2) {
			Vec3 e1 = v1 - v0;
			Vec3 e2 = v2 - v0;
			Vec3 n = Vec3::cross(e1, e2).normalized();
			unsigned int base = (unsigned int)vertices.size();
			vertices.push_back({v0, color, uv0, n});
			vertices.push_back({v1, color, uv1, n});
			vertices.push_back({v2, color, uv2, n});
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 2);
		};

		// Front
		addTri(bl, br, apex, Vec2(0, 0), Vec2(1, 0), Vec2(0.5f, 1));
		// Right
		addTri(br, tr, apex, Vec2(0, 0), Vec2(1, 0), Vec2(0.5f, 1));
		// Back
		addTri(tr, tl, apex, Vec2(0, 0), Vec2(1, 0), Vec2(0.5f, 1));
		// Left
		addTri(tl, bl, apex, Vec2(0, 0), Vec2(1, 0), Vec2(0.5f, 1));

		// Base Quad
		unsigned int baseIdx = (unsigned int)vertices.size();
		Vec3 bn(0, -1, 0);
		vertices.push_back({bl, color, Vec2(0, 1), bn});
		vertices.push_back({tl, color, Vec2(0, 0), bn});
		vertices.push_back({tr, color, Vec2(1, 0), bn});
		vertices.push_back({br, color, Vec2(1, 1), bn});
		
		indices.push_back(baseIdx + 0);
		indices.push_back(baseIdx + 1);
		indices.push_back(baseIdx + 2);
		indices.push_back(baseIdx + 2);
		indices.push_back(baseIdx + 3);
		indices.push_back(baseIdx + 0);
	}
};

struct TexturedPlane : public TexturedMesh {
	TexturedPlane(float width, float depth, Color color = Color(1,1,1,1), float uvScaleX = 1.0f, float uvScaleY = 1.0f) {
		float hw = width / 2.0f;
		float hd = depth / 2.0f;
		Vec3 n(0, 1, 0);

		vertices.push_back({Vec3(-hw, 0,  hd), color, Vec2(0.0f, 0.0f), n});
		vertices.push_back({Vec3( hw, 0,  hd), color, Vec2(uvScaleX, 0.0f), n});
		vertices.push_back({Vec3( hw, 0, -hd), color, Vec2(uvScaleX, uvScaleY), n});
		vertices.push_back({Vec3(-hw, 0, -hd), color, Vec2(0.0f, uvScaleY), n});

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
	}
};

struct TexturedCylinder : public TexturedMesh {
	TexturedCylinder(float radius, float height, int segments = 20, Color color = Color(1,1,1,1)) {
		for (int i = 0; i <= segments; ++i) {
			float angle = 2 * PI * i / segments;
			float x = radius * cosf(angle);
			float z = radius * sinf(angle);
			float u = (float)i / segments;

			Vec3 n = Vec3(cosf(angle), 0, sinf(angle)).normalized();

			vertices.push_back({Vec3(x, 0, z), color, Vec2(u, 0.0f), n});
			vertices.push_back({Vec3(x, height, z), color, Vec2(u, 1.0f), n});
		}

		for (int i = 0; i < segments; ++i) {
			unsigned int base = i * 2;
			indices.push_back(base + 0);
			indices.push_back(base + 1);
			indices.push_back(base + 3);
			indices.push_back(base + 3);
			indices.push_back(base + 2);
			indices.push_back(base + 0);
		}
	}
};

struct TexturedCone : public TexturedMesh {
	TexturedCone(float radius, float height, int segments = 20, Color color = Color(1,1,1,1)) {
		Vec3 apex(0, height, 0);
		
		unsigned int apexIdxStart = (unsigned int)vertices.size();
		
		for (int i = 0; i <= segments; ++i) {
			float angle = 2 * PI * i / segments;
			float x = radius * cosf(angle);
			float z = radius * sinf(angle);
			float u = (float)i / segments;
			
			Vec3 n = Vec3(cosf(angle), radius / height, sinf(angle)).normalized();

			vertices.push_back({apex, color, Vec2(u, 1.0f), n});
			vertices.push_back({Vec3(x, 0, z), color, Vec2(u, 0.0f), n});
		}
		
		for (int i = 0; i < segments; ++i) {
			unsigned int v0 = apexIdxStart + i * 2;
			unsigned int v1 = apexIdxStart + i * 2 + 1;
			unsigned int v2 = apexIdxStart + (i + 1) * 2 + 1;
			
			indices.push_back(v0);
			indices.push_back(v1);
			indices.push_back(v2);
		}
	}
};

struct TexturedSphere : public TexturedMesh {
	TexturedSphere(float radius, int sectors, int stacks, Color color = Color(1,1,1,1)) {
		float lengthInv = 1.0f / radius;

		for (int i = 0; i <= stacks; ++i) {
			float stackAngle = PI / 2 - i * PI / stacks; // starting from pi/2 to -pi/2
			float xy = radius * cosf(stackAngle);
			float z = radius * sinf(stackAngle);

			for (int j = 0; j <= sectors; ++j) {
				float sectorAngle = j * 2 * PI / sectors; // starting from 0 to 2pi

				float x = xy * cosf(sectorAngle);
				float y = xy * sinf(sectorAngle);

				Vec3 pos(x, z, y);
				Vec3 n(x * lengthInv, z * lengthInv, y * lengthInv);
				Vec2 uv((float)j / sectors, (float)i / stacks);

				vertices.push_back({pos, color, uv, n});
			}
		}

		for (int i = 0; i < stacks; ++i) {
			unsigned int k1 = i * (sectors + 1);
			unsigned int k2 = k1 + sectors + 1;

			for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (i != (stacks - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}
};
