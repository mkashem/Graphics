#include "pyramid.h"

GLuint pyramid_vao;

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
		//front  
		0.0f, 1.0f, 0.0f,  // apex   
		-1.0f, -1.0f, 1.0f,  // front left   
		1.0f, -1.0f, 1.0f,   // front right 
		//left
		0.0f, 1.0f, 0.0f,  // apex   
		-1.0f, -1.0f, -1.0f, // back left   
		-1.0f, -1.0f, 1.0f,  // front left 
		//right
		0.0f, 1.0f, 0.0f,  // apex   
		1.0f, -1.0f, 1.0f,   // front right   
		1.0f, -1.0f, -1.0f,  // back right 
		//back
		0.0f, 1.0f, 0.0f,  // apex   
		1.0f, -1.0f, -1.0f,   // back right   
		1.0f, -1.0f, -1.0f, // back left     
	};


	float pyramid_normals[] = {
		//bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		//front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		//left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		//right
		1.0f, 0.0f , 0.0f,
		1.0f, 0.0f , 0.0f,
		1.0f, 0.0f , 0.0f,
		//back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f


	};


	float pyramid_textures[] = {
		//borromt
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//front
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		//left
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		//right
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		//back
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,



	
	};



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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_textures), pyramid_textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords


	glBindVertexArray(0);

}

void renderPyramid() {
	glBindVertexArray(pyramid_vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glBindVertexArray(0);
}


