#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createCone();
void renderCone();

#define NumConePoints  18
#define NumTriangles   18
#define NumIndices     3*NumTriangles

extern	unsigned  cone_vao;
extern	unsigned   cone_vbo;
extern	unsigned   cone_ebo;