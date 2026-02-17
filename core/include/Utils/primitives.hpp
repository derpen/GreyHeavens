#pragma once

#include <glad/glad.h>

/// @TODO:
/// Better naming convention required
/// Right now it's too hacky
namespace Primitives {
unsigned int plane_VBO, plane_VAO, plane_EBO;
static const float plane_vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

static const unsigned int plane_indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

void GenerateVAOPlane() {
	glGenVertexArrays(1, &plane_VAO);
	glGenBuffers(1, &plane_VBO);
	glGenBuffers(1, &plane_EBO);

	glBindVertexArray(plane_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void UseVAOPlane() {
	if (!plane_VAO) {
		GenerateVAOPlane();
	}

	glBindVertexArray(plane_VAO);
}

void UnbindVAO() {
	glBindVertexArray(0);
}
}