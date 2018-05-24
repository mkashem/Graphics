#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "circle.h"

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;
unsigned  circle_vao;
unsigned   circle_vbo;
unsigned   circle_ebo;



void updateVertexNormals_cir();
static const double kPI_cir = 3.1415926535897932384626433832795;
int index_cir = 0;
GLuint indices_cir[NumIndices_cir];
vec4 points_cir[NumConePoints_cir + 1];
vec3 normals_cir[NumConePoints_cir + 1];
vec2 tex_cir[NumConePoints_cir + 1];
void  createCircle() {


	points_cir[index_cir][0] = 0.0;
	points_cir[index_cir][1] = 0.0;
	points_cir[index_cir][2] = 0.0;
	points_cir[index_cir][3] = 1.0;

	normals_cir[index_cir][0] = 0.0;
	normals_cir[index_cir][1] = 0.0;
	normals_cir[index_cir][2] = 0.0;
	tex_cir[index_cir][0] = 0.5;
	tex_cir[index_cir][1] = 0.5;

	index_cir++;

	int i;
	float theta;
	int tIndices = 0;


	for (i = 0; i < NumConePoints_cir; ++i) {

		theta = i*20.0f*kPI_cir / 180.0f;

		points_cir[index_cir][0] = cos(theta);
		points_cir[index_cir][1] = 0.0;
		points_cir[index_cir][2] = -sin(theta);
		points_cir[index_cir][3] = 1.0;

		normals_cir[index_cir][0] = 0.0;
		normals_cir[index_cir][1] = 0.0;
		normals_cir[index_cir][2] = 0.0;

		tex_cir[index_cir][0] = (1 + cos(theta)) / 2;
		tex_cir[index_cir][1] = (1 + sin(theta)) / 2;
		if (i <= (NumConePoints_cir - 2)) {

			indices_cir[tIndices] = 0; tIndices++;
			indices_cir[tIndices] = index_cir; tIndices++;
			indices_cir[tIndices] = index_cir + 1; tIndices++;

		}
		else {
			indices_cir[tIndices] = 0; tIndices++;
			indices_cir[tIndices] = index_cir; tIndices++;
			indices_cir[tIndices] = 1; tIndices++;
		}
		index_cir++;
	}

	GLuint offset = 0;
	glGenVertexArrays(1, &circle_vao);
	glBindVertexArray(circle_vao);
	glGenBuffers(1, &circle_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, circle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points_cir) + sizeof(normals_cir) + sizeof(tex_cir), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points_cir), points_cir);
	offset += sizeof(points_cir);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals_cir), normals_cir);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tex_cir), tex_cir);
	glGenBuffers(1, &circle_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circle_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_cir), (indices_cir), GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(points_cir));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(tex_cir));
	glEnableVertexAttribArray(2);
	/*  tux*/
	
	/*************/

	updateVertexNormals_cir();
	glBindVertexArray(0);
}


void updateVertexNormals_cir() {

	vec4 P1, P2, P3;
	vec3 N; int i;
	//calculate vertex nirmals
	for (i = 0; i < NumIndices_cir; i += 3) {
		P1 = points_cir[indices_cir[i + 0]];
		P2 = points_cir[indices_cir[i + 1]];
		P3 = points_cir[indices_cir[i + 2]];
		N = normalize(cross(vec3(P2 - P1), (vec3(P3 - P1))));
		normals_cir[indices_cir[i + 0]] += N;
		normals_cir[indices_cir[i + 1]] += N;
		normals_cir[indices_cir[i + 2]] += N;
	}
	for (i = 0; i < NumConePoints_cir; i++)
		normals_cir[i] = normalize(vec3(normals_cir[i]));


}

void renderCircle() {
	glBindVertexArray(circle_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circle_ebo);


	glDrawElements(GL_TRIANGLES, NumIndices_cir, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();


};
