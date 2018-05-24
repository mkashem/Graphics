#include "box.h"

GLuint box_vao;
//GLuint box_vbo;
//GLuint box_ebo;

void createBox()
{
	const GLfloat box_vertices[] = { 
		//Front 
		-0.5, -0.5, 0.5, 1.0f,  
		0.5, -0.5, 0.5, 1.0f,   
		0.5, 0.5, 0.5, 1.0f,   
		-0.5, 0.5, 0.5, 1.0f, 
		//Right
		-0.5, -0.5, 0.5, 1.0f,   
		0.5, -0.5, -0.5, 1.0f,   
		0.5, 0.5, -0.5, 1.0f,   
		0.5, 0.5, 0.5, 1.0f, 
		//Back 
		-0.5, -0.5, -0.5, 1.0f,    
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
	
	float box_normals[] = {
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
		//Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		//left
		-1.0f, 0.0f , 0.0f,
		-1.0f, 0.0f , 0.0f,
		-1.0f, 0.0f , 0.0f,
		-1.0f, 0.0f , 0.0f,
		//bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		//top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,


	};


	float box_textures[] = {
		//front
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	//right
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	//back
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	//left
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	//bottom
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	//top
	0.0,  0.0,                // -2.0f, -2.0f,
	1.0,  0.0,                //  4.0f, -2.0f,
	1.0f, 1.0f,               //  4.0f,  4.0f,
	0.0,  1.0,                //  -2.0f, 4.0f,
	};





	GLuint box_indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23

	};


	glGenVertexArrays(1, &box_vao);
	glBindVertexArray(box_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(box_normals), box_normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_textures), box_textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), box_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void renderBox() {
	glBindVertexArray(box_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


