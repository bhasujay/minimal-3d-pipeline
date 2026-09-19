#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>

class Shader {
public:
	unsigned int ID;

	// Constructor compiles vertex and fragment shaders and links them into a program
	Shader(const char* vertexSource, const char* fragmentSource);

	// Activates the shader program
	void use() const;

	// Deletes the shader program
	void cleanup();

	// Uniform setters
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, const float* value) const;
};
