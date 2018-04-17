#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createCircle();
void renderCircle();

#define NumConePoints_cir  18
#define NumTriangles_cir   18
#define NumIndices_cir   3*NumTriangles_cir

extern	unsigned  circle_vao;
extern	unsigned   circle_vbo;
extern	unsigned   circle_ebo;
