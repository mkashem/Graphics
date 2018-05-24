#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sphere.h"

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;
unsigned  sphere_vao;
unsigned   sphere_vbo;
unsigned   sphere_ebo;

void sphere_updateVertexNormals();
static const double kPI = 3.1415926535897932384626433832795;
int sphere_index = 0;
GLuint sphere_indices[NumIndices];
vec4 sphere_points[NumConePoints + 1];
vec3 sphere_normals[NumConePoints + 1];
vec2 sphere_tex[NumConePoints + 1];
void createSphere() {


	sphere_points[sphere_index][0] = 0.0;
	sphere_points[sphere_index][1] = 1.0;
	sphere_points[sphere_index][2] = 0.0;
	sphere_points[sphere_index][3] = 1.0;

	sphere_normals[sphere_index][0] = 0.0;
	sphere_normals[sphere_index][1] = 0.0;
	sphere_normals[sphere_index][2] = 0.0;
	sphere_tex[sphere_index][0] = 0.5;
	sphere_tex[sphere_index][1] = 0.5;

	sphere_index++;

	int i;
	float theta;
	int tIndices = 0;


	for (i = 0; i < NumConePoints; ++i) {

		theta = i * 20.0f*kPI / 180.0f;

		sphere_points[sphere_index][0] = cos(theta);
		sphere_points[sphere_index][1] = -1.0;
		sphere_points[sphere_index][2] = -sin(theta);
		sphere_points[sphere_index][3] = 1.0;

		sphere_normals[sphere_index][0] = 0.0;
		sphere_normals[sphere_index][1] = 0.0;
		sphere_normals[sphere_index][2] = 0.0;

		sphere_tex[sphere_index][0] = (1 + cos(theta)) / 2;
		sphere_tex[sphere_index][1] = (1 + sin(theta)) / 2;
		if (i <= (NumConePoints - 2)) {

			sphere_indices[tIndices] = 0; tIndices++;
			sphere_indices[tIndices] = sphere_index; tIndices++;
			sphere_indices[tIndices] = sphere_index + 1; tIndices++;

		}
		else {
			sphere_indices[tIndices] = 0; tIndices++;
			sphere_indices[tIndices] = sphere_index; tIndices++;
			sphere_indices[tIndices] = 1; tIndices++;
		}
		sphere_index++;
	}

	GLuint offset = 0;
	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);
	glGenBuffers(1, &sphere_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_points) + sizeof(sphere_normals) + sizeof(sphere_tex), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(sphere_points), sphere_points);
	offset += sizeof(sphere_points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(sphere_normals), sphere_normals);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(sphere_tex), sphere_tex);
	glGenBuffers(1, &sphere_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_indices), (sphere_indices), GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(sphere_points));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(sphere_tex));
	glEnableVertexAttribArray(2);

	/************/

	/*******/

	sphere_updateVertexNormals();
	glBindVertexArray(0);
}


void sphere_updateVertexNormals() {

	vec4 P1, P2, P3;
	vec3 N; int i;
	//calculate vertex nirmals
	for (i = 0; i < NumIndices; i += 3) {
		P1 = sphere_points[sphere_indices[i + 0]];
		P2 = sphere_points[sphere_indices[i + 1]];
		P3 = sphere_points[sphere_indices[i + 2]];
		N = normalize(cross(vec3(P2 - P1), (vec3(P3 - P1))));
		sphere_normals[sphere_indices[i + 0]] += N;
		sphere_normals[sphere_indices[i + 1]] += N;
		sphere_normals[sphere_indices[i + 2]] += N;
	}
	for (i = 0; i < NumConePoints; i++)
		sphere_normals[i] = normalize(vec3(sphere_normals[i]));

}

void renderSphere() {
	glBindVertexArray(sphere_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);


	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();


};
