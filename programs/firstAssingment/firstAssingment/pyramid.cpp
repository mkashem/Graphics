#include "pyramid.h"

unsigned int pyramid_vao;

void createPyramid()
{
	const GLfloat pyramid_vertices[] = {
		
	// Bottom
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		// side
		0.0f, 1.0f, 0.0f, // apex
		-1.0f, -1.0f, 1.0f, // front left
		1.0f, -1.0f, 1.0f, // front right

		0.0f, 1.0f, 0.0f, // apex
		-1.0f, -1.0f, -1.0f, // back left
		-1.0f, -1.0f, 1.0f, // front left

		0.0f, 1.0f, 0.0f, // apex
		1.0f, -1.0f, 1.0f, // front right
		1.0f, -1.0f, -1.0f, // back right

		0.0f, 1.0f, 0.0f, // apex
		1.0f, -1.0f, -1.0f, // back right
		-1.0f, -1.0f, -1.0f, // back left
	};
	float pyramid_normals[] = {
		// Bottom
		0.0f, -1.0f, 0.0f, // normal 1
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,

	};

	/*float textures[4 * 2] = {
	-2.0f,  -2.0f,
	4.0f,  -2.0f,
	4.0f, 4.0f,
	-2.0,  4.0,
	};
	*/
	float textures[] = {
		0.0f,  0.0f,                // -2.0f, -2.0f,
		1.0f,  0.0f,                //  4.0f, -2.0f,
		1.0f, 1.0f,

		0.0f,  1.0f,                //  -2.0f, 4.0f,
		0.0f,  0.0f,                // -2.0f, -2.0f,
		1.0f,  1.0f,

		0.5f, 1.0f,               //  4.0f,  4.0f,
		0.0f,  0.0f,                //  -2.0f, 4.0f,
		1.0f, 0.0f,

		0.5,  1.0,                // -2.0f, -2.0f,
		0.0,  0.0,                //  4.0f, -2.0f,
		0.0f, 1.0f,               //  4.0f,  4.0f,
								  //  -2.0f, 4.0f,

	   0.5,  1.0,                // -2.0f, -2.0f,
	   0.0,  0.0,                //  4.0f, -2.0f,
	   0.0f, 1.0f,               //  4.0f,  4.0f,

	   0.5,  0.0,                // -2.0f, -2.0f,
	   1.0,  0.0,                //  4.0f, -2.0f,
	   1.0f, 0.0f,               //  4.0f,  4.0f,



	};

	/*
	GLuint pyramid_indices[] = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10,8,10,11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20,21,22,20,22,23,

	};
	*/
	glGenVertexArrays(1, &pyramid_vao);
	glBindVertexArray(pyramid_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_normals), pyramid_normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

								   //	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
								   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void renderPyramid() {
	glBindVertexArray(pyramid_vao);
	//glDrawArrays(GL_TRIANGLES, 54, GL_UNSIGNED_INT,0);
	//glClearColor(1.0, 0.0, 1.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 18);
	//glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);

	//	glFlush();
}


