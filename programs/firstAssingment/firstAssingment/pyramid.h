#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createPyramid();
void renderPyramid();

#define NumConePoints  18
#define NumTriangles   18
#define NumIndices     3*NumTriangles

extern	unsigned  pyramid_vao;
extern	unsigned   pyramid_vbo;
extern	unsigned   pyramid_ebo;