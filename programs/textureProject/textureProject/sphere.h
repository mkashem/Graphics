#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createSphere();
void renderSphere();
//
//#define NumConePoints  18
//#define NumTriangles   18
//#define NumIndices     3*NumTriangles

#define STEP  20
#define NUMVERTICES  STEP*(STEP-1)+2
#define Number_of_triangles STEP * (STEP- 1)*2
#define Number_of_indices  STEP*(STEP - 1)*6

extern	unsigned  sphere_vao;
extern	unsigned   sphere_vbo;
extern	unsigned   sphere_ebo;