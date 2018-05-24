#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"hexagon.h"
#include"cube.h"
#include "pyramid.h"
#include"disk.h"
#include"cone.h"
#include"sphere.h"


#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

mat4 view;
mat4 projection;

GLuint program;

float aspect = 0.0;
bool show_line = false;
bool orthographic = false;
void Rotate(int n);
float angle1 = 0.0;
float angle2 = -1.0;
float angle3 = 0.0;






static const double kPI = 3.1415926535897932384626433832795;

// material properties

float material_shininess = 5.0;



static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void Initialize();
void Display(void);
void Reshape(int width, int height);

static const GLchar* ReadFile(const char* filename)
{
	FILE* infile;

#ifdef WIN32

	fopen_s(&infile, filename, "rb");

#else
	infile = fopen(filename, "r");
#endif
	if (!infile) {
		printf("Unable to open file %s\n", filename);
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;

	return (GLchar*)(source);
}

GLuint initShaders(const char *v_shader, const char *f_shader) {

	GLuint p = glCreateProgram();
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	free ((char*)vs);
	free ((char*) fs);

	glCompileShader(v);
	GLint compiled;
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
		glGetShaderInfoLog(v, len, &len, log);
		printf("Vertex Shader compilation failed: %s\n", log);
		free ((GLchar*)log);
	}

	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
		glGetShaderInfoLog(f, len, &len, log);
		printf("Fragment Shader compilation failed: %s\n", log);
		free((GLchar*) log);
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	GLint linked;
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*) malloc(sizeof(GLchar)*(len + 1));
		glGetProgramInfoLog(p, len, &len, log);
		printf("Shader linking failed: %s\n",  log);
		free ((char*)log);
	}
	glUseProgram(p);
	return p;
}

void Initialize(void){
	// Create the program for rendering the model

	program = initShaders("smoothshader.vert", "smoothshader.frag");

	createHexagon();
	createCone();
	createPyramid();
	createCube();
	createDisk();
	createSphere();
	

	
	view = glm::lookAt(vec3(1.0f, 1.0f, 6.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	
	// Set other lighting parameters here

	
	projection = mat4(1.0f);
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);

}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Choose whether to draw in wireframe mode or not

	if (show_line)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	vec4 light_position(10.0, 10.0, 9.8, 1.0);  // positional light source
	vec4 light_ambient(0.3, 0.3, 0.3, 1.0);
	vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 light_specular(1.0, 1.0, 1.0, 1.0);

	vec4 material_ambient(1.0, 0.0, 1.0, 1.0);
	vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	vec4 material_specular(1.0, 0.0, 1.0, 1.0);

	vec4 lightpos = view*light_position;
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&lightpos);

	// Initialize shader material and lighting parameters

	vec4 ambient_product = light_ambient*material_ambient;
	vec4 diffuse_product = light_diffuse*material_diffuse;
	vec4 specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	// Setup matrices

	mat4 model = glm::translate(glm::mat4(1.0f), vec3(3.0f, 2.0f, 0.0f));

	if (orthographic)
	    projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f-angle1), aspect, 4.3f, 100.0f);
	mat4 mvp = projection*view*model;
	mat4 model_view = view*model;

	mat3 normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);

	// You need to add normal matrix and model view matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);

	renderHexagon();
	//****************************************
	//**********************************
	model = glm::translate(glm::mat4(1.0f), vec3(-2.0f, 2.0f, 0.0f));
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	 mvp = projection * view*model;
	 model_view = view * model;

	 normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	renderCone();
	//****************************************
	//**********************************
	//****************************************
	//**********************************
	mat4 _rotate, _translate;
	_rotate = glm::rotate(mat4(1.0f), radians(270.0f - angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate = glm::translate(glm::mat4(1.0f), vec3(1.0f, 2.0f, 0.0f));
	model = _translate * _rotate;
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);

	material_ambient = vec4(1.0, 0.5, 0.0, 1.0);
	material_diffuse = vec4(0.0, 0.60, 1.0, 1.0);
	material_specular = vec4(0.80, 0.85, 0.75, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	renderPyramid();
	//*****************

	//mat4 _rotate, _translate;
	_rotate = glm::rotate(mat4(1.0f), radians(270.0f-angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate=glm::translate(glm::mat4(1.0f), vec3(-1.0f, -1.0f, 0.0f));
	model = _translate *_rotate ;
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);

	material_ambient = vec4(0.33, 0.33, 0.0, 1.0);
	material_diffuse = vec4(0.0, 0.60, 1.0, 1.0);
	material_specular = vec4(0.80, 0.85, 0.75, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	renderCube();
	//****************************************
	//**********************************
	model = glm::translate(glm::mat4(1.0f), vec3(1.0f, -1.0f, 0.0f));
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f+angle1), aspect, 4.3f, 100.0f);
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);

	material_ambient = vec4(1.0, 0.5, 0.0, 1.0);
	material_diffuse = vec4(0.0, 0.60, 1.0, 1.0);
	material_specular = vec4(0.80, 0.85, 0.75, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	renderDisk();
	//****************************************
	//**********************************
	model = glm::translate(glm::mat4(1.0f), vec3(3.0f, -1.0f, 0.0f));
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	//renderCone();
	renderSphere();
	//****************************************
	//**********************************
	
	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}


void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 's':
		show_line = !show_line;
		break;
	case 'o':case 'O':
		orthographic = !orthographic;
	}
	glutPostRedisplay();
}

void Rotate(int n)  //the "glutTimerFunc"
{

	switch (n) {
	case 1:
		angle1 += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	case 2:
		angle2 += 0.125 / 2;
		glutPostRedisplay();
		glutTimerFunc(150, Rotate, 2);
		break;
	case 3:
		angle3 += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 3);
		break;
	}

}

/*********/
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	//glutInitWindowSize(512, 512);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Shaded Cube");

	if (glewInit()){
		printf("Unable to initialize GLEW ... exiting\n");
	}

	Initialize();
	printf("%s\n",glGetString(GL_VERSION));
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Rotate, 1);
	glutMainLoop();
	return 0;
}




/*************/
