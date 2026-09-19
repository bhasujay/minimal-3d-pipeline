#include "Texture.h"
#include <glad/glad.h>

namespace {

// Clamps an integer value to the [0, 255] byte range
unsigned char clampByte(int v) {
	return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

// Simple integer hash for pseudo-random variation in textures
unsigned int hashNoise(unsigned int x) {
	x = ((x >> 16) ^ x) * 0x45d9f3bu;
	x = ((x >> 16) ^ x) * 0x45d9f3bu;
	x = (x >> 16) ^ x;
	return x;
}

// Creates an OpenGL texture with GL_REPEAT and GL_LINEAR filtering
unsigned int createProceduralTexture(const unsigned char* data, int width, int height) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return textureID;
}

} // anonymous namespace

// Brick wall texture — red-brown bricks with gray mortar lines
unsigned int generateBrickTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];
	int brickH = 8, brickW = 16, mortarW = 1;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int row = y / brickH;
			int offset = (row % 2 == 0) ? 0 : brickW / 2; // stagger alternate rows
			int localY = y % brickH;
			int localX = (x + offset) % brickW;

			if (localY < mortarW || localX < mortarW) {
				// Mortar (gray)
				data[idx]     = 180;
				data[idx + 1] = 180;
				data[idx + 2] = 170;
			} else {
				// Brick with subtle noise
				int v = (int)(hashNoise(x * 7919u + y * 7901u) % 30) - 15;
				data[idx]     = clampByte(160 + v);
				data[idx + 1] = clampByte(70 + v / 2);
				data[idx + 2] = clampByte(50 + v / 2);
			}
		}
	}
	return createProceduralTexture(data, W, H);
}

// Roof tile texture — dark reddish-brown with tile row lines
unsigned int generateRoofTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];
	int tileH = 8;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int localY = y % tileH;
			int v = (int)(hashNoise(x * 3571u + y * 3559u) % 20) - 10;

			if (localY == 0) {
				// Tile edge line (dark)
				data[idx]     = 60;
				data[idx + 1] = 50;
				data[idx + 2] = 50;
			} else {
				data[idx]     = clampByte(140 + v);
				data[idx + 1] = clampByte(60 + v / 2);
				data[idx + 2] = clampByte(45 + v / 2);
			}
		}
	}
	return createProceduralTexture(data, W, H);
}

// Grass texture — green with natural variation
unsigned int generateGrassTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int v = (int)(hashNoise(x * 5171u + y * 5153u) % 40) - 20;
			data[idx]     = clampByte(50 + v / 2);
			data[idx + 1] = clampByte(140 + v);
			data[idx + 2] = clampByte(40 + v / 2);
		}
	}
	return createProceduralTexture(data, W, H);
}

// Road / asphalt texture — dark gray with subtle noise and center line
unsigned int generateRoadTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int v = (int)(hashNoise(x * 4271u + y * 4253u) % 20) - 10;
			int base = 85;
			data[idx]     = clampByte(base + v);
			data[idx + 1] = clampByte(base + v);
			data[idx + 2] = clampByte(base + v);

			// Dashed yellow center road marking
			if (y >= 30 && y <= 33 && (x / 10) % 2 == 0) {
				data[idx]     = 220;
				data[idx + 1] = 200;
				data[idx + 2] = 50;
			}
		}
	}
	return createProceduralTexture(data, W, H);
}

// Wood bark texture — brown with vertical grain lines
unsigned int generateBarkTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int v = (int)(hashNoise(x * 2671u + y * 2657u) % 30) - 15;
			int stripe = (x % 6 < 2) ? -15 : 0; // vertical grain pattern
			data[idx]     = clampByte(100 + v + stripe);
			data[idx + 1] = clampByte(65 + v / 2 + stripe / 2);
			data[idx + 2] = clampByte(35 + v / 2 + stripe / 2);
		}
	}
	return createProceduralTexture(data, W, H);
}

// Leaves texture — green with darker splotches
unsigned int generateLeavesTexture() {
	const int W = 64, H = 64;
	unsigned char data[W * H * 3];

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int v = (int)(hashNoise(x * 1171u + y * 1153u) % 50) - 25;
			data[idx]     = clampByte(30 + v / 2);
			data[idx + 1] = clampByte(120 + v);
			data[idx + 2] = clampByte(25 + v / 3);
		}
	}
	return createProceduralTexture(data, W, H);
}

// Door texture — wooden planks with golden door handle
unsigned int generateDoorTexture() {
	const int W = 128, H = 256;
	unsigned char data[W * H * 3];
	int plankW = 32;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			int idx = (y * W + x) * 3;
			int v = (int)(hashNoise(x * 1871u + y * 1861u) % 20) - 10;
			int plankEdge = (x % plankW == 0) ? -30 : 0;

			data[idx]     = clampByte(120 + v + plankEdge);
			data[idx + 1] = clampByte(75 + v / 2 + plankEdge / 2);
			data[idx + 2] = clampByte(40 + v / 2 + plankEdge / 2);

			// Door handle (golden plate and knob)
			if (x >= 92 && x <= 104 && y >= 110 && y <= 140) {
				data[idx]     = 210;
				data[idx + 1] = 180;
				data[idx + 2] = 50;
			}
			if (x >= 95 && x <= 101 && y >= 122 && y <= 128) {
				data[idx]     = 255;
				data[idx + 1] = 220;
				data[idx + 2] = 80;
			}
		}
	}

	return createProceduralTexture(data, W, H);
}

// Plain white texture for shiny objects
unsigned int generateWhiteTexture() {
	const int W = 4, H = 4;
	unsigned char data[W * H * 3];
	for (int i = 0; i < W * H * 3; ++i) {
		data[i] = 255;
	}
	return createProceduralTexture(data, W, H);
}
