#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createSphere();
void renderSphere();

#define NumConePoints  18
#define NumTriangles   18
#define NumIndices     3*NumTriangles

extern	unsigned  sphere_vao;
extern	unsigned   sphere_vbo;
extern	unsigned   sphere_ebo;