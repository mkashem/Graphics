#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cone.h"

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;
unsigned  cone_vao;
unsigned   cone_vbo;
unsigned   cone_ebo;

void updateVertexNormals();
static const double kPI = 3.1415926535897932384626433832795;
int index = 0;
GLuint indices[NumIndices];
vec4 points[NumConePoints + 1];
vec3 normals[NumConePoints + 1];
vec2 tex[NumConePoints + 1];
void createCone() {


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


	for (i = 0; i < NumConePoints; ++i) {

		theta = i * 20.0f*kPI / 180.0f;

		points[index][0] = cos(theta);
		points[index][1] = -1.0;
		points[index][2] = -sin(theta);
		points[index][3] = 1.0;

		normals[index][0] = 0.0;
		normals[index][1] = 0.0;
		normals[index][2] = 0.0;

		tex[index][0] = (1 + cos(theta)) / 2;
		tex[index][1] = (1 + sin(theta)) / 2;
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

	GLuint offset = 0;
	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);
	glGenBuffers(1, &cone_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points);
	offset += sizeof(points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tex), tex);
	glGenBuffers(1, &cone_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (indices), GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(points));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(tex));
	glEnableVertexAttribArray(2);

	/************/

	/*******/

	updateVertexNormals();
	glBindVertexArray(0);
}


void updateVertexNormals() {

	vec4 P1, P2, P3;
	vec3 N; int i;
	//calculate vertex nirmals
	for (i = 0; i < NumIndices; i += 3) {
		P1 = points[indices[i + 0]];
		P2 = points[indices[i + 1]];
		P3 = points[indices[i + 2]];
		N = normalize(cross(vec3(P2 - P1), (vec3(P3 - P1))));
		normals[indices[i + 0]] += N;
		normals[indices[i + 1]] += N;
		normals[indices[i + 2]] += N;
	}
	for (i = 0; i < NumConePoints; i++)
		normals[i] = normalize(vec3(normals[i]));

}

void renderCone() {
	glBindVertexArray(cone_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);


	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();


};
