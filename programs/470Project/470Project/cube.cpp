#include"cube.h"

unsigned int cube_vao;
unsigned  int cube_ebo;
unsigned  int cube_vbo[2];

void createCube() {
	float vertices[] = { -0.5, -0.5, 0.5, 1.0f, //Front
		0.5, -0.5, 0.5, 1.0f,
		0.5, 0.5, 0.5, 1.0f,
		-0.5, 0.5, 0.5, 1.0f,

		0.5, -0.5, 0.5, 1.0f, //Right
		0.5, -0.5, -0.5, 1.0f,
		0.5, 0.5, -0.5, 1.0f,
		0.5, 0.5, 0.5, 1.0f,

		-0.5, -0.5, -0.5, 1.0f, //Back
		-0.5, 0.5, -0.5, 1.0f,
		0.5, 0.5, -0.5, 1.0f,
		0.5, -0.5, -0.5, 1.0f,
		//Left
		-0.5, -0.5, 0.5, 1.0f,
		-0.5, 0.5, 0.5, 1.0f,
		-0.5, 0.5, -0.5, 1.0f,
		-0.5, -0.5, -0.5, 1.0f,
		//Bottom
		-0.5, -0.5, 0.5, 1.0f,
		-0.5, -0.5, -0.5, 1.0f,
		0.5, -0.5, -0.5, 1.0f,
		0.5, -0.5,  0.5, 1.0f,
		//Top
		-0.5, 0.5, 0.5, 1.0f,
		0.5, 0.5, 0.5, 1.0f,
		0.5, 0.5, -0.5, 1.0f,
		-0.5, 0.5, -0.5, 1.0f
	};
	float normals[] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(2, cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &cube_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (indices), GL_STATIC_DRAW);

	// attribute indices

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

};
void renderCube() {

	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
};
