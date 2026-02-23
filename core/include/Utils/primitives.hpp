#pragma once

#include <glad/glad.h>

/// @TODO:
/// Better naming convention required
/// Right now it's too hacky
/// 
/// Also, using manual vertices suck so bad, need to fix those.
/// Might not be needed tho, I'll just use a model loader in
/// the future to handle primitives.
namespace Primitives {

/// 
/// Planes
/// 
unsigned int plane_VBO, plane_VAO, plane_EBO;
static const float plane_vertices[] = {
	// positions          // colors           // texture coords
	 1.0f,  0.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 1.0f,  0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f,  0.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.0f,  0.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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

/// 
/// Cube
/// 
unsigned int cube_VBO, cube_VAO;
static const float cube_vertices[] = {
	// back face
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
	// front face
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
	// left face
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
	// right face
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
	// bottom face
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
	// top face
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f // bottom-left
};

void GenerateVAOCube() {
	glGenVertexArrays(1, &cube_VAO);
	glGenBuffers(1, &cube_VBO);

	glBindVertexArray(cube_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void UseVAOCube() {
	if (!cube_VAO) {
		GenerateVAOCube();
	}

	glBindVertexArray(cube_VAO);
}

/// 
/// Skybox Cube
///
unsigned int skybox_VBO, skybox_VAO;
static const float skybox_vertices[] = {
	// positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void GenerateVAOSkybox() {
	glGenVertexArrays(1, &skybox_VAO);
	glGenBuffers(1, &skybox_VBO);

	glBindVertexArray(skybox_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void UseVAOSkybox() {
	if (!skybox_VAO) {
		GenerateVAOSkybox();
	}

	glBindVertexArray(skybox_VAO);
}

void UnbindVAO() {
	glBindVertexArray(0);
}
}