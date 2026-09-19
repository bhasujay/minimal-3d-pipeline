#pragma once
#include <glad/glad.h>
#include <vector>
#include "Geometry.h"

class TexturedMesh {
public:
	std::vector<Vertex3DTex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;

	TexturedMesh() : VAO(0), VBO(0), EBO(0) {}

	virtual ~TexturedMesh() {
		cleanup();
	}

	void initGL() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3DTex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// location 0: position (3 floats at offset 0)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DTex), (void*)0);

		// location 1: color (4 floats at offset 3*sizeof(float))
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3DTex), (void*)(3 * sizeof(float)));

		// location 2: texcoord (2 floats at offset 7*sizeof(float))
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3DTex), (void*)(7 * sizeof(float)));

		// location 3: normal (3 floats at offset 9*sizeof(float))
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DTex), (void*)(9 * sizeof(float)));

		glBindVertexArray(0);
	}

	void draw() const {
		if (VAO == 0) return;
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void cleanup() {
		if (VAO != 0) {
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO != 0) {
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (EBO != 0) {
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}
	}
};
