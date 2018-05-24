#include "box.h"

unsigned int Boxvao;

void createBox()
{
	const GLfloat box_vertices[] = { -0.15, -0.15, 0.15, 1.0f, //Front
		0.15, -0.15, 0.15, 1.0f,
		0.15, 0.15, 0.15, 1.0f,
		-0.15, 0.15, 0.15, 1.0f,
		0.15, -0.15, 0.15, 1.0f, //Right
		0.15, -0.15, -0.15, 1.0f,
		0.15, 0.15, -0.15, 1.0f,
		0.15, 0.15, 0.15, 1.0f,
		-0.15, -0.15, -0.15, 1.0f, //Back
		-0.15, 0.15, -0.15, 1.0f,
		0.15, 0.15, -0.15, 1.0f,
		0.15, -0.15, -0.15, 1.0f,
		//Left
		-0.15, -0.15, 0.15, 1.0f,
		-0.15, 0.15, 0.15, 1.0f,
		-0.15, 0.15, -0.15, 1.0f,
		-0.15, -0.15, -0.15, 1.0f,
		//Bottom
		-0.15, -0.15, 0.15, 1.0f,
		-0.15, -0.15, -0.15, 1.0f,
		0.15, -0.15, -0.15, 1.0f,
		0.15, -0.15, 0.15, 1.0f,
		//Top
		-0.15, 0.15, 0.15, 1.0f,
		0.15, 0.15, 0.15, 1.0f,
		0.15, 0.15, -0.15, 1.0f,
		-0.15, 0.15, -0.15, 1.0f
	};

	float box_normals[24 * 3] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		//Left
		-1.0, 0.0f, 0.0, 
		-1.0, 0.0f, 0.0, 
		-1.0, 0.0f, 0.0, 
		-1.0, 0.0f, 0.0, 
		//buttom
		0.0, -1.0f, 0.0, 
		0.0, -1.0f, 0.0, 
		0.0, -1.0f, 0.0, 
		0.0, -1.0f, 0.0, 
		//top
		0.0, 1.0f, 0.0, 
		0.0, 1.0f, 0.0, 
		0.0, 1.0f, 0.0, 
		0.0, 1.0f, 0.0, 

	};

	/*float textures[4 * 2] = {
	-2.0f,  -2.0f,
	4.0f,  -2.0f,
	4.0f, 4.0f,
	-2.0,  4.0,
	};
	*/
	float textures[] = {
		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,

		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,

		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,

		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,

		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,

		0.0,  0.0,                // -2.0f, -2.0f,
		1.0,  0.0,                //  4.0f, -2.0f,
		1.0f, 1.0f,               //  4.0f,  4.0f,
		0.0,  1.0,                //  -2.0f, 4.0f,
	};


	GLuint box_indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10,8,10,11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20,21,22,20,22,23,

	};

	glGenVertexArrays(1, &Boxvao);
	glBindVertexArray(Boxvao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_normals), box_normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), box_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void renderBox() {
	glBindVertexArray(Boxvao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


