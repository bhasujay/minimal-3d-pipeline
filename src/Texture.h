#pragma once

// ============================================================
// Procedural Texture Generation Functions
// ============================================================

// Generates a 64x64 brick wall texture (red-brown bricks with gray mortar, staggered rows)
unsigned int generateBrickTexture();

// Generates a 64x64 roof tile texture (dark reddish-brown with tile row lines)
unsigned int generateRoofTexture();

// Generates a 64x64 grass texture (green with noise variation)
unsigned int generateGrassTexture();

// Generates a 64x64 road/asphalt texture (dark gray asphalt with dashed yellow center line)
unsigned int generateRoadTexture();

// Generates a 64x64 wood bark texture (brown with vertical grain)
unsigned int generateBarkTexture();

// Generates a 64x64 leaves texture (green with darker splotches)
unsigned int generateLeavesTexture();

// Generates a 128x256 wooden door texture (wooden planks with golden door handle)
unsigned int generateDoorTexture();

// Generates a plain white texture for metallic/shiny objects
unsigned int generateWhiteTexture();
