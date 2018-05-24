#pragma once
#include <stdio.h>
#include <GL/glew.h>
extern	unsigned cube_vao;
extern	unsigned cube_vbo[2];
extern	unsigned  cube_ebo;

void createCube();
void renderCube();