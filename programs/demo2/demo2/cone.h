#include <stdio.h>
#include <GL/glew.h>

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

#define NumConePoints  36
#define NumTriangles   36
#define NumIndices     3*NumTriangles


void createCone();
void renderCone();

extern	unsigned int cone_vao;