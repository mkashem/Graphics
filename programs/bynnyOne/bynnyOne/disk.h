#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createDisk();
void renderDisk();

#define NumConePoints  18
#define NumTriangles   18
#define NumIndices     3*NumTriangles

extern	unsigned  disk_vao;
extern	unsigned   disk_vbo;
extern	unsigned   disk_ebo;

