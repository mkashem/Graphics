#include "Plane2.h"

unsigned int plane2_vao;

void createPlane2()
{
	float side = 1.0f;
	float side2 = side / 2.0f;

	float vertices[4 * 3] = {
		// Front
		-4.0, -0.5, 0.0,
		4.0, -0.5, 0.0,
		4.0, 0.5, 0.0,
		-4.0, 0.5, 0.0,
	};

	float normals[4 * 3] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

	};


	/*float textures[4 * 2] = {
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	};*/

	float textures[4 * 2] = {
		-2.0f,  -2.0f,
		4.0f,  -2.0f,
		4.0f, 4.0f,
		-2.0,  4.0,
	};


	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,

	};

	glGenVertexArrays(1, &plane2_vao);
	glBindVertexArray(plane2_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void renderPlane2() {
	glBindVertexArray(plane2_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


