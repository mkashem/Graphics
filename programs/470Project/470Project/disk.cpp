#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "disk.h"
#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;
unsigned  disk_vao;
unsigned   disk_vbo;
unsigned   disk_ebo;

void DiskupdateVertexNormals();
static const double kPI = 3.1415926535897932384626433832795;
int _index = 0;
GLuint _indices[NumIndices];
vec4 _points[NumConePoints + 1];
vec3 _normals[NumConePoints + 1];
vec2 _tex[NumConePoints + 1];
void createDisk() {


	_points[_index][0] = 0.0;
	_points[_index][1] = 0.0;
	_points[_index][2] = 0.0;
	_points[_index][3] = 1.0;

	_normals[_index][0] = 0.0;
	_normals[_index][1] = 0.0;
	_normals[_index][2] = 0.0;
	_tex[_index][0] = 0.5;
	_tex[_index][1] = 0.5;

	_index++;

	int i;
	float theta;
	int tIndices = 0;


	for (i = 0; i < NumConePoints; ++i) {

		theta = i * 20.0f*kPI / 180.0f;

		_points[_index][0] = cos(theta);
		_points[_index][1] =0.0 ;
		_points[_index][2] = -sin(theta); // test demo 
		_points[_index][3] = 1.0;

		_normals[_index][0] = 0.0;
		_normals[_index][1] = 0.0;
		_normals[_index][2] = 0.0;

		_tex[_index][0] = (1 + cos(theta)) / 2;
		_tex[_index][1] = (1 + sin(theta)) / 2;
		if (i <= (NumConePoints - 2)) {

			_indices[tIndices] = 0; tIndices++;
			_indices[tIndices] = _index; tIndices++;
			_indices[tIndices] = _index + 1; tIndices++;

		}
		else {
			_indices[tIndices] = 0; tIndices++;
			_indices[tIndices] = _index; tIndices++;
			_indices[tIndices] = 1; tIndices++;
		}
		_index++;
	}

	GLuint offset = 0;
	glGenVertexArrays(1, &disk_vao);
	glBindVertexArray(disk_vao);
	glGenBuffers(1, &disk_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, disk_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_points) + sizeof(_normals) + sizeof(_tex), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(_points), _points);
	offset += sizeof(_points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(_normals), _normals);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(_tex), _tex);
	glGenBuffers(1, &disk_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, disk_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), (_indices), GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(_points));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(_tex));
	glEnableVertexAttribArray(2);
	
	//glVertexAttribDivisor(2, 1); //every instance of position will change

	/************/

	/*******/

	DiskupdateVertexNormals();
	glBindVertexArray(0);
}


void DiskupdateVertexNormals() {

	vec4 P1, P2, P3;
	vec3 N; int i;
	//calculate vertex nirmals
	for (i = 0; i < NumIndices; i += 3) {
		P1 = _points[_indices[i + 0]];
		P2 = _points[_indices[i + 1]];
		P3 = _points[_indices[i + 2]];
		N = normalize(cross(vec3(P2 - P1), (vec3(P3 - P1))));
		_normals[_indices[i + 0]] += N;
		_normals[_indices[i + 1]] += N;
		_normals[_indices[i + 2]] += N;
	}
	for (i = 0; i < NumConePoints; i++)
		_normals[i] = normalize(vec3(_normals[i]));

}

void renderDisk() {
	glBindVertexArray(disk_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, disk_ebo);
	
	
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();


};
