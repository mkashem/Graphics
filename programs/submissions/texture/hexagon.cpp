#include"hexagon.h"
unsigned int hexagon_vao;
void createHexagon()
{
	const GLfloat hexagon_vertices[] = {

		0.0,0.0,0.0,1.0, //index 0
	   -0.45, -0.75, 0.0, 1.0, //idex 1
    	0.45, -0.75, 0.0, 1.0, //index 2
		0.90, 0.0, 0.0, 1.0, //index 3
		0.45, 0.75, 0.0, 1.0, //index 4
		-0.45, 0.75, 0.0, 1.0, //index 5
		-0.90, 0.0, 0.0, 1.0, //index 6

	};
	float hexagon_normals[] = {
		// Bottom
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

	};

	/*float textures[4 * 2] = {
	-2.0f,  -2.0f,
	4.0f,  -2.0f,
	4.0f, 4.0f,
	-2.0,  4.0,
	};
	*/
	float textures[] = {
		0.5,0.5,
		0.25, 0.0, 					   
		0.75, 0.0,  //index 3									
		1.0,0.5,
		0.75,1.0,
		0.25,1.0,
		0.0,0.5,
	};
	
	GLuint hexagon_indices[] = 
	{
	0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1

	};
	

	glGenVertexArrays(1, &hexagon_vao);
	glBindVertexArray(hexagon_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagon_vertices), hexagon_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagon_normals), hexagon_normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexagon_indices), hexagon_indices, GL_STATIC_DRAW);  // indices is not attribute. its element.
	

	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindVertexArray(0);

}
void renderHexagon() {
	glBindVertexArray(hexagon_vao);

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	//	glFlush();
}
