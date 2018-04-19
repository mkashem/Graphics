#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "grass.h"
#include "ground.h"
#include "box.h"
#include "pyramid.h"
#include "cone.h"
#include "plane.h"
#include "Plane2.h"

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

using namespace glm;

GLuint boxTexID;
GLuint pyramidTexID;

bool speed = false;


void Initialize(void);
void Display(void);



GLuint prog;
GLuint render_prog;


typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;


float angle = 0.0;
float angle2 = 0.0;

vec4 light_position(20.0, 40.0, 60.0, 0.0);  // directional light source

float material_shininess = 50.0;

vec4 ambient_product;
vec4 diffuse_product;
vec4 specular_product;

mat4 view_matrix;
mat4 projection_matrix;
mat4 model_matrix;
GLuint model_matrix_loc;
GLuint view_matrix_loc;
GLuint projection_matrix_loc;
float aspect;

//----------------------------------------------------------------------------

const GLchar* ReadShader(const char* filename) {
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");

#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		printf("Unable to open file %s", filename);
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = (GLchar*)malloc(sizeof(GLchar)*(len + 1));

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return ((GLchar*)source);
}

//----------------------------------------------------------------------------

GLuint LoadShaders(ShaderInfo* shaders){
	if (shaders == NULL) { return 0; }

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	while (entry->type != GL_NONE) {
		GLuint shader = glCreateShader(entry->type);

		entry->shader = shader;

		const GLchar* source = ReadShader(entry->filename);

		if (source == NULL) {


			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}

		glShaderSource(shader, 1, &source, NULL);
		free((GLchar*)source);

		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
			glGetShaderInfoLog(shader, len, &len, log);
			printf("Shader compilation failed.\n");
			free(log);
#endif /* DEBUG */

			return 0;
		}

		glAttachShader(program, shader);

		++entry;
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
		glGetProgramInfoLog(program, len, &len, log);
		printf("Shader linking failed: %s\n", log);
		free(log);
#endif /* DEBUG */

		for (entry = shaders; entry->type != GL_NONE; ++entry) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}
////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************/

void Reshape(int width, int height) {

	glViewport(0, 0, width, height);
	aspect = float(width) / float(height);
}
/*****************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////




void Initialize(void)
{
	// Create the program for rendering the model
	
	// Initialize shader lighting parameters

	
	ShaderInfo shader[] = { { GL_VERTEX_SHADER, "grassOne.VERT" },
	{ GL_FRAGMENT_SHADER, "grassOne.FRAG" },
	{ GL_NONE, NULL }
	};
	
	render_prog = LoadShaders(shader);
	glUseProgram(render_prog);
	
	createGrass();


	ShaderInfo common_shaders[] = { { GL_VERTEX_SHADER, "common.vs" },
	                                { GL_FRAGMENT_SHADER, "common.fs" },
	                                { GL_NONE, NULL } };

	prog = LoadShaders(common_shaders);

	
	glUseProgram(prog);


	createGround();

	
	glClearColor(0.5f, 0.75f, 0.85f, 1.0f);  // sky color


	createBox();
	createPyramid();
	createCone();
	createPlane();
	createPlane2();

}

/**************************************************************************************************************/
void Display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(render_prog);

	vec4 light_ambient(0.5, 0.5, 1.0, 1.0);
	vec4 light_diffuse(0.5, 0.5, 1.0, 1.0);
	vec4 light_specular(1.0, 1.0, 1.0, 1.0);

	vec4 material_ambient(0.1, 1.0, 0.1, 1.0);
	vec4 material_diffuse(0.1, 1.0, 0.1, 1.0);
	vec4 material_specular(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(render_prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(render_prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(render_prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);

	glUniform4fv(glGetUniformLocation(render_prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(render_prog, "Shininess"), material_shininess);

	view_matrix_loc = glGetUniformLocation(render_prog, "view_matrix");
	projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");
	model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
	view_matrix = glm::lookAt(glm::vec3(0.0f, 40.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, (GLfloat*)&view_matrix[0]);
	projection_matrix = perspective(radians(60.0f), aspect, 1.0f, 3000.0f);
	model_matrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);

	renderGrass();
	
	glUseProgram(prog);

	material_ambient = vec4(0.25, 0.25, 0.25, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;
	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);

	glUniform4fv(glGetUniformLocation(prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(prog, "Shininess"), material_shininess);

	view_matrix_loc = glGetUniformLocation(prog, "view_matrix");
	projection_matrix_loc = glGetUniformLocation(prog, "projection_matrix");
	model_matrix_loc = glGetUniformLocation(prog, "model_matrix");
	view_matrix = glm::lookAt(glm::vec3(0.0f, 40.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection_matrix = perspective(radians(60.0f), aspect, 1.0f, 3000.0f);

	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, (GLfloat*)&view_matrix[0]);
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);
	
	
	

	renderGround();

	mat4 scale_matrix = scale(mat4(1.0f), vec3(50.0f, 40.0f, 40.0f));
	mat4 translateMat = translate(mat4(1.0f), vec3(-60.0f, 10.0f, 0.0f));
	mat4 rotateMat = rotate(mat4(1.0f), radians(35.0f), vec3(0.0, 1.0f, 0.0f));

	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(1.0, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderBox();//house

	//**********************************************//
	scale_matrix = scale(mat4(1.0f), vec3(10.0f, 20.0f, 10.0f));
	translateMat = translate(mat4(1.0f), vec3(-40.0f, 10.0f, 10.0f));
	rotateMat = rotate(mat4(1.0f), radians(35.0f), vec3(0.0, 1.0f, 0.0f));

	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.45, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderBox();//door
	//***********************************************//

	scale_matrix = scale(mat4(1.0f), vec3(10.0f, 10.0f, 10.0f));
	translateMat = translate(mat4(1.0f), vec3(-55.0f, 15.0f, 18.0f));
	rotateMat = rotate(mat4(1.0f), radians(35.0f), vec3(0.0, 1.0f, 0.0f));

	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.5f, 0.75f, 0.85f, 1.0f);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderBox();//window
				//***********************************************//

	scale_matrix = scale(mat4(1.0f), vec3(30.0f, 15.0f, 30.0f));
	translateMat = translate(mat4(1.0f), vec3(-60.0f, 45.0f, 0.0f));
	rotateMat = rotate(mat4(1.0f), radians(35.0f), vec3(0.0, 1.0f, 0.0f));

	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.45, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderPyramid();//roof

	//***********************************************//

	scale_matrix = scale(mat4(1.0f), vec3(5.0f, 10.0f, 10.0f));
	translateMat = translate(mat4(1.0f), vec3(10.0f, 10.0f, 0.0f));
	
	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.45, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderCone();//trunk

	//*******************************//

	scale_matrix = scale(mat4(1.0f), vec3(20.0f, 20.0f, 20.0f));
	translateMat = translate(mat4(1.0f), vec3(10.0f, 30.0f, 0.0f));

	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.0, 1.0, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderCone();//tree

				 //*******************************//

				 //***********************************************//

	scale_matrix = scale(mat4(1.0f), vec3(5.0f, 10.0f, 10.0f));
	translateMat = translate(mat4(1.0f), vec3(-90.0f, 10.0f, 30.0f));

	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.45, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderCone();//trunk

				 //*******************************//

	scale_matrix = scale(mat4(1.0f), vec3(20.0f, 20.0f, 20.0f));
	translateMat = translate(mat4(1.0f), vec3(-90.0f, 30.0f, 30.0f));

	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.0, 1.0, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderCone();//tree

				 //*******************************//

	scale_matrix = scale(mat4(1.0f), vec3(30.0f, 100.0f, 30.0f));
	translateMat = translate(mat4(1.0f), vec3(80.0f, 10.0f, -25.0f));


	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(1.0, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderBox();//windmill

	//***************************************//

	scale_matrix = scale(mat4(1.0f), vec3(20.0f, 10.0f, 20.0f));
	translateMat = translate(mat4(1.0f), vec3(80.0f, 70.0f, -25.0f));


	model_matrix = translateMat*scale_matrix;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.45, 0.35, 0.0, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderPyramid();//windmill roof


	//****************************************************//
	if (speed)
	{
		scale_matrix = scale(mat4(1.0f), vec3(5.0f, 5.0f, 0.0f));
		translateMat = translate(mat4(1.0f), vec3(67.0f, 50.0f, 25.0f));
		rotateMat = rotate(mat4(1.0f), radians(angle), vec3(0.0, 0.0f, 1.0f));


		model_matrix = translateMat*scale_matrix*rotateMat;

		glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

		material_ambient = vec4(0.9, 0.9, 0.3, 1.0);
		material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
		material_specular = vec4(0.25, 0.25, 0.25, 1.0);

		ambient_product = light_ambient*material_ambient;
		diffuse_product = light_diffuse*material_diffuse;
		specular_product = light_specular*material_specular;

		glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
		glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
		glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


		renderPlane();

		//****************************************************//

		scale_matrix = scale(mat4(1.0f), vec3(5.0f, 5.0f, 0.0f));
		translateMat = translate(mat4(1.0f), vec3(67.0f, 50.0f, 25.0f));
		rotateMat = rotate(mat4(1.0f), radians(angle), vec3(0.0, 0.0f, 1.0f));


		model_matrix = translateMat*scale_matrix*rotateMat;

		glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

		material_ambient = vec4(0.9, 0.9, 0.3, 1.0);
		material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
		material_specular = vec4(0.25, 0.25, 0.25, 1.0);

		ambient_product = light_ambient*material_ambient;
		diffuse_product = light_diffuse*material_diffuse;
		specular_product = light_specular*material_specular;

		glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
		glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
		glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


		renderPlane2();
	}
	else if (!speed)
	{
		scale_matrix = scale(mat4(1.0f), vec3(5.0f, 5.0f, 0.0f));
	translateMat = translate(mat4(1.0f), vec3(67.0f, 50.0f, 25.0f));
	rotateMat = rotate(mat4(1.0f), radians(angle2), vec3(0.0, 0.0f, 1.0f));


	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.9, 0.9, 0.3, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderPlane();

	//****************************************************//

	scale_matrix = scale(mat4(1.0f), vec3(5.0f, 5.0f, 0.0f));
	translateMat = translate(mat4(1.0f), vec3(67.0f, 50.0f, 25.0f));
	rotateMat = rotate(mat4(1.0f), radians(angle2), vec3(0.0, 0.0f, 1.0f));


	model_matrix = translateMat*scale_matrix*rotateMat;

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	material_ambient = vec4(0.9, 0.9, 0.3, 1.0);
	material_diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	material_specular = vec4(0.25, 0.25, 0.25, 1.0);

	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product);


	renderPlane2();

	}


	glutSwapBuffers();

}
/**************************************************************************************************************/
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 's':
		speed = !speed;
		break;
	}
	glutPostRedisplay();
}
/*************************************************************************/
void Rotate(int n)  //the "glutTimerFunc"
{
		
	switch (n) {
	case 1:
		angle += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	case 2:
		angle2 += 5;
		glutPostRedisplay();
		glutTimerFunc(25, Rotate, 2);
		break;
	}

}

/************************************************************************/
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);

	glutCreateWindow("Scenery");

	if (glewInit()) {
		printf("Unable to initialize GLEW ... exiting\n");
	}

	Initialize();
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Rotate, 1);
	glutTimerFunc(25, Rotate, 2);
	glutMainLoop();
}

/*****/



/*************/