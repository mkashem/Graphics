#include "cone.h"

GLuint  cone_vao;
GLuint  cone_vbo;
GLuint  cone_ebo;


static const double kPI = 3.1415926535897932384626433832795;

int index = 0;
GLuint indices[NumIndices];
vec4 points[NumConePoints + 1];
vec3 normals[NumConePoints + 1];
vec2 tex[NumConePoints + 1];

void createCone()
{

	points[index][0] = 0.0;
	points[index][1] = 1.0;
	points[index][2] = 0.0;
	points[index][3] = 1.0;

	normals[index][0] = 0.0;
	normals[index][1] = 0.0;
	normals[index][2] = 0.0;

	tex[index][0] = 0.5;
	tex[index][1] = 0.5;

	index++;

	int i;
	float theta;
	int tIndices = 0;


	for (i = 0; i < NumConePoints; ++i)
	{

		theta = i*10.0f*kPI / 180.0f;

		points[index][0] = cos(theta);
		points[index][1] = -1.0;
		points[index][2] = -sin(theta);
		points[index][3] = 1.0;

		normals[index][0] = 0.0;
		normals[index][1] = 0.0;
		normals[index][2] = 0.0;

		tex[index][0] = (1 + cos(theta)) / 2.0;
		tex[index][1] = (1 + sin(theta)) / 2.0;

		if (i <= (NumConePoints - 2)) {

			indices[tIndices] = 0; tIndices++;
			indices[tIndices] = index; tIndices++;
			indices[tIndices] = index + 1; tIndices++;

		}
		else {
			indices[tIndices] = 0; tIndices++;
			indices[tIndices] = index; tIndices++;
			indices[tIndices] = 1; tIndices++;
		}
		index++;
	}

	//updateVertexNormals();

	int j = 0;
	// Calculate vertex normals  
	for (j = 0; j < NumIndices; j += 3)
	{
		vec4 P1 = points[indices[j + 0]];
		vec4 P2 = points[indices[j + 1]];
		vec4 P3 = points[indices[j + 2]];
		vec3 N = normalize(cross(vec3(P2 - P1), (vec3(P3 - P1))));
		normals[indices[j + 0]] += N;
		normals[indices[j + 1]] += N;
		normals[indices[j + 2]] += N;
	}

	//normalize normals 
	int k = 0;
	for (k = 0; k < NumConePoints; k++)
		normals[k] = normalize(vec3(normals[k]));

	GLuint offset = 0;


	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);


}

void renderCone()
{
	glBindVertexArray(cone_vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}




