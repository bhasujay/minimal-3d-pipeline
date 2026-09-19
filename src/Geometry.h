#pragma once
#include <cmath>

// ============================================================
// Vector & Point Types
// ============================================================

struct Vec2 {
	float x, y;
	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float x, float y) : x(x), y(y) {}
};

struct Point2D {
	float x;
	float y;

	Point2D() : x(0.0f), y(0.0f) {}
	Point2D(float x_val, float y_val) : x(x_val), y(y_val) {}
};

struct Point3D {
	float x;
	float y;
	float z;

	Point3D() : x(0.0f), y(0.0f), z(0.0f) {}
	Point3D(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}
};

// General-purpose 3D vector with arithmetic operators
struct Vec3 {
	float x, y, z;

	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
	Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
	Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }

	float length() const { return sqrtf(x * x + y * y + z * z); }

	Vec3 normalized() const {
		float len = length();
		if (len < 1e-8f) return Vec3(0, 0, 0);
		return Vec3(x / len, y / len, z / len);
	}

	static float dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Vec3 cross(const Vec3& a, const Vec3& b) {
		return Vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}
};

// ============================================================
// Color
// ============================================================

struct Color {
	float r;
	float g;
	float b;
	float a;

	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float red, float green, float blue, float alpha = 1.0f) 
		: r(red), g(green), b(blue), a(alpha) {}
};

// ============================================================
// Vertex Types
// ============================================================

struct Vertex2D {
	Point2D position;
	Color color;
};

struct Vertex3D {
	Point3D position;
	Color color;
};

// Vertex with texture coordinates and normal for lit rendering
struct Vertex3DTex {
	Vec3 position;
	Color color;
	Vec2 uv;
	Vec3 normal;

	Vertex3DTex() {}

	// Construction from Vec3 position + Vec2 uv (used by Primitives.h)
	Vertex3DTex(Vec3 pos, Color col, Vec2 texcoord, Vec3 norm = Vec3(0, 1, 0))
		: position(pos), color(col), uv(texcoord), normal(norm) {}

	// Construction from Point3D + separate u,v floats (used by Texture module)
	Vertex3DTex(Point3D pos, Color col, float u_val, float v_val,
	            Vec3 norm = Vec3(0, 1, 0))
		: position(Vec3(pos.x, pos.y, pos.z)), color(col),
		  uv(Vec2(u_val, v_val)), normal(norm) {}
};

// ============================================================
// 4x4 Matrix (Column-Major)
// ============================================================

struct Mat4 {
	float m[16]; // Column-major layout

	Mat4() {
		for (int i = 0; i < 16; ++i) m[i] = 0.0f;
	}

	static Mat4 identity() {
		Mat4 mat;
		mat.m[0] = 1.0f;
		mat.m[5] = 1.0f;
		mat.m[10] = 1.0f;
		mat.m[15] = 1.0f;
		return mat;
	}

	static Mat4 translate(float x, float y, float z) {
		Mat4 mat = identity();
		mat.m[12] = x;
		mat.m[13] = y;
		mat.m[14] = z;
		return mat;
	}

	static Mat4 scale(float x, float y, float z) {
		Mat4 mat = identity();
		mat.m[0] = x;
		mat.m[5] = y;
		mat.m[10] = z;
		return mat;
	}

	static Mat4 rotateX(float angleRad) {
		Mat4 mat = identity();
		float c = cosf(angleRad);
		float s = sinf(angleRad);
		mat.m[5] = c;
		mat.m[6] = s;
		mat.m[9] = -s;
		mat.m[10] = c;
		return mat;
	}

	static Mat4 rotateY(float angleRad) {
		Mat4 mat = identity();
		float c = cosf(angleRad);
		float s = sinf(angleRad);
		mat.m[0] = c;
		mat.m[2] = -s;
		mat.m[8] = s;
		mat.m[10] = c;
		return mat;
	}

	static Mat4 rotateZ(float angleRad) {
		Mat4 mat = identity();
		float c = cosf(angleRad);
		float s = sinf(angleRad);
		mat.m[0] = c;
		mat.m[1] = s;
		mat.m[4] = -s;
		mat.m[5] = c;
		return mat;
	}

	static Mat4 mirrorX() {
		Mat4 mat = identity();
		mat.m[0] = -1.0f;
		return mat;
	}

	static Mat4 mirrorY() {
		Mat4 mat = identity();
		mat.m[5] = -1.0f;
		return mat;
	}

	static Mat4 mirrorZ() {
		Mat4 mat = identity();
		mat.m[10] = -1.0f;
		return mat;
	}

	static Mat4 perspective(float fovRad, float aspect, float znear, float zfar) {
		Mat4 mat;
		float tanHalfFov = tanf(fovRad / 2.0f);
		mat.m[0] = 1.0f / (aspect * tanHalfFov);
		mat.m[5] = 1.0f / tanHalfFov;
		mat.m[10] = -(zfar + znear) / (zfar - znear);
		mat.m[11] = -1.0f;
		mat.m[14] = -(2.0f * zfar * znear) / (zfar - znear);
		mat.m[15] = 0.0f;
		return mat;
	}

	static Mat4 lookAt(float eyeX, float eyeY, float eyeZ,
		float centerX, float centerY, float centerZ,
		float upX, float upY, float upZ) {
		// forward = normalize(center - eye)
		float fx = centerX - eyeX, fy = centerY - eyeY, fz = centerZ - eyeZ;
		float fLen = sqrtf(fx * fx + fy * fy + fz * fz);
		fx /= fLen; fy /= fLen; fz /= fLen;

		// side = normalize(forward x up)
		float sx = fy * upZ - fz * upY;
		float sy = fz * upX - fx * upZ;
		float sz = fx * upY - fy * upX;
		float sLen = sqrtf(sx * sx + sy * sy + sz * sz);
		sx /= sLen; sy /= sLen; sz /= sLen;

		// u = side x forward
		float ux = sy * fz - sz * fy;
		float uy = sz * fx - sx * fz;
		float uz = sx * fy - sy * fx;

		Mat4 mat = identity();
		mat.m[0] = sx;   mat.m[4] = sy;   mat.m[8]  = sz;
		mat.m[1] = ux;   mat.m[5] = uy;   mat.m[9]  = uz;
		mat.m[2] = -fx;  mat.m[6] = -fy;  mat.m[10] = -fz;
		mat.m[12] = -(sx * eyeX + sy * eyeY + sz * eyeZ);
		mat.m[13] = -(ux * eyeX + uy * eyeY + uz * eyeZ);
		mat.m[14] =  (fx * eyeX + fy * eyeY + fz * eyeZ);
		return mat;
	}

	Mat4 operator*(const Mat4& other) const {
		Mat4 result;
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				float sum = 0.0f;
				for (int k = 0; k < 4; ++k) {
					sum += m[k * 4 + r] * other.m[c * 4 + k];
				}
				result.m[c * 4 + r] = sum;
			}
		}
		return result;
	}
};
