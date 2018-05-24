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

//void sphere_updateVertexNormals();
static const double kPI = 3.1415926535897932384626433832795;
int sphere_index = 0;
int s_index = 0;
float theta;
float phi;

vec4 vertex[NUMVERTICES];
GLuint sphere_indices[Number_of_indices];
vec3 sphere_normals[NUMVERTICES + 1];
//Number of indices = Number of triangles * 3 = STEP * (STEP - 1) * 6
void createSphere() {

		for (double b = -STEP / 2.0; b <= STEP / 2.0; b++) {
			if (b == -STEP / 2.0 || b == STEP / 2.0) { 
				theta = (1.0 * b / STEP) *kPI;          
				vertex[s_index].x = 0;
				vertex[s_index].y = sin(theta);
				vertex[s_index].z = 0;
				vertex[s_index].w = 1.0f;
				
				sphere_normals[s_index] =normalize(vec3(vertex[s_index]));
				
				s_index++;
			}
			else {
				for (int a = 0; a < STEP; a++) {   
					// lat/lon coordinates   
					phi = (1.0 * a / STEP) * 2 * kPI;    
					theta = (1.0 * b / STEP) *kPI; 

					vertex[s_index].x = cos(theta)*sin(phi);
					vertex[s_index].y = sin(theta);
					vertex[s_index].z = cos(theta)*cos(phi);
					vertex[s_index].w = 1.0f;
					
					sphere_normals[s_index] = normalize(vec3(vertex[s_index]));
					s_index++;
				}
			}
		}                                                              
		
	for (int i = 0; i < (STEP); i++) {

		if (i == STEP - 1) // does the wrap around 
		{  
			sphere_indices[sphere_index]=0;
			sphere_index++;
			sphere_indices[sphere_index] = i+1;
			sphere_index++;
			sphere_indices[sphere_index] = 1;
			sphere_index++;
		}
		else
		{
			sphere_indices[sphere_index] = i+1;
			sphere_index++;
			sphere_indices[sphere_index] = i+2;
			sphere_index++;
			sphere_indices[sphere_index] =0;
			sphere_index++;
		} 
		
	}   
///top**********************************
	for (int i = NUMVERTICES - STEP - 1; i < NUMVERTICES - 1; i++) {
		if (i == NUMVERTICES - 2) { // does the wrap around part   
									//write your code 

			sphere_indices[sphere_index] = i;
			sphere_index++;
			sphere_indices[sphere_index] = NUMVERTICES - STEP - 1;
			sphere_index++;
			sphere_indices[sphere_index] = NUMVERTICES - 1;
			sphere_index++;
		
		} 
		else   { // construct a triangle in each iteration 
			sphere_indices[sphere_index] = i;
			sphere_index++;
			sphere_indices[sphere_index] = i+1;
			sphere_index++;
			sphere_indices[sphere_index] = NUMVERTICES-1;
			sphere_index++;
		} 
	}



	for (int i = 1; i < (NUMVERTICES - STEP - 1); i += STEP) {
		for (int j = i; j < (i + STEP); j++) {
			if (j == i + STEP - 1) {// does wrap up   
				sphere_indices[sphere_index] = j;
				sphere_index++;
				sphere_indices[sphere_index] = i;
				sphere_index++;
				sphere_indices[sphere_index] = j+ STEP;
				sphere_index++;

				sphere_indices[sphere_index] = j + STEP;
				sphere_index++;
				sphere_indices[sphere_index] = i;
				sphere_index++;
				sphere_indices[sphere_index] = i+STEP;
				sphere_index++;

				}    
			else {
				sphere_indices[sphere_index] = j;
				sphere_index++;
				sphere_indices[sphere_index] = j+1;
				sphere_index++;
				sphere_indices[sphere_index] = j + STEP;
				sphere_index++;

				sphere_indices[sphere_index] = j+ STEP;
				sphere_index++;
				sphere_indices[sphere_index] = j + 1;
				sphere_index++;
				sphere_indices[sphere_index] = j + 1+STEP;
				sphere_index++;



					}   
				}
			} 


	GLuint offset = 0;
	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);
	glGenBuffers(1, &sphere_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(sphere_normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vertex), vertex);
	offset += sizeof(vertex);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(sphere_normals), sphere_normals);
	//glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(sphere_tex), sphere_tex);
	glGenBuffers(1, &sphere_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_indices), (sphere_indices), GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertex));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(sphere_tex));
	//glEnableVertexAttribArray(2);

	/************/

	/*******/
	glBindVertexArray(0);
}


void renderSphere() {
	glBindVertexArray(sphere_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);


	glDrawElements(GL_TRIANGLES, Number_of_indices, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();


};
