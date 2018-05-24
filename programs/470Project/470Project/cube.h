#pragma once
#include <stdio.h>
#include <GL/glew.h>

void createCube();
void renderCube();
extern	unsigned  cube_vao;
extern	unsigned  cube_ebo;
extern unsigned  cube_vbo[2];