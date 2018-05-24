#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Triangle.h"
#include "cone.h"
#include "pyramid.h"
#include "hexagon.h"
#include"ground.h"
#include "cube.h"
#include"sphere.h"
#include "disk.h"
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#define GLM_FORCE_RADIANS 
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

GLuint planeTexID;
bool change_projection;
GLuint program;
GLuint v, f;
bool orthographic = false;
GLboolean show_line = false;
GLuint model_matrix_loc;
GLuint view_matrix_loc;
GLuint projection_matrix_loc;
void setMetrix();
mat4 model_matrix;
mat4 view, projection_matrix;
void Rotate(int n);
float angle1 = 0.0;
float angle2 = -1.0;
float angle3 = 0.0;
float aspect;
static const double kPI = 3.1415926535897932384626433832795;
void Reshape(int width, int height);
unsigned int loadTexture(char* filename);


// position of the vertices in the hexagon

/*--------------------------------------------------------------------------------------*/

const GLchar* ReadShader(const char* filename);
GLuint LoadShaders(char* v_shader, char* f_shader);

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
//----------------------------------------------------------------------------
float material_shininess = 80.0;
vec4 light_position(10.0, 10.0, 9.8, 1.0); // positional light source



//---------------------------------------------------------

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

GLuint LoadShaders(char* v_shader, char* f_shader) {

	GLuint p = glCreateProgram();
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadShader(v_shader);
	const char * fs = ReadShader(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	free((char*)vs);
	free((char*)fs);

	glCompileShader(v);
	GLint compiled;
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc((sizeof(GLchar))*(len + 1));
		glGetShaderInfoLog(v, len, &len, log);
		printf("Vertex Shader compilation failed: %s\n", log);
		free((GLchar*)log);
	}

	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc((sizeof(GLchar))*(len + 1));
		glGetShaderInfoLog(f, len, &len, log);
		printf("Fragment Shader compilation failed: %s\n", log);
		free((GLchar*)log);
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glBindAttribLocation(p, 0, "vPosition");


	glLinkProgram(p);

	GLint linked;
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*(len + 1));
		glGetProgramInfoLog(p, len, &len, log);
		printf("Shader linking failed: %s\n", log);
		free((GLchar*)log);
	}

	return p;

}
////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************/


void Reshape(int width, int height) {

	glViewport(0, 0, width, height);
	aspect = float(width) / float(height);
}

void init(void) {

	 program = LoadShaders((char*)"shader.vert", (char*)"shader.frag");
	glUseProgram(program);
	//view = glm::lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -100.0), vec3(0.0, 1.0, 0.0)); // this is the default camera positioin 
	
 //view = glm::lookAt(vec3(1.5*cos(radians(angle1)), 0.0f, 1.5*sin(radians(angle1))), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)); // change the camera position 
	
	
	
	createGround();
	//createTriangle();
	createCone();
	createDisk();
	createPyramid();
	createHexagon();
	createCube();
	createSphere();

	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.5f, 0.75f, 0.85f, 1.0f);

}

/****************************************************************************************/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);

	//glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	//view = glm::lookAt(vec3(0.0f, 2.0f, 8.5), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	view = glm::lookAt(vec3(1.0f, 1.0f, 6.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//view = glm::lookAt(vec3(0.0f, 40.0f, 200.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	
	projection_matrix = mat4(1.0f);
	model_matrix = mat4(1.0f);
	vec4 light_ambient(0.3, 0.3, 0.3, 1.0);
	vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 light_specular(1.0, 1.0, 1.0, 1.0);

	vec4 material_ambient(0.0, 1.0, 0.0, 1.0);
	vec4 material_diffuse(0.6, 0.0, 0.8, 1.0);
	vec4 material_specular(1.0, 1.0, 1.0, 1.0);

	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	
	
   // view = glm::lookAt(glm::vec3(0.0f, 40.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	

	mat3 normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	
	

	mat4 _scale, _rotate, _translate;
	
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, (GLfloat*)&view[0]);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "project_matrix"), 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, -10.0));
	model_matrix = _translate;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	setMetrix();
	renderGround();
	
	if (show_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	if (orthographic) {
		projection_matrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
		//projection_matrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	}
	else {
		projection_matrix = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	}
		
	
	

	model_matrix = mat4(1.0f);
	//mat4 _rotate, _scale, _translate;
	_rotate = glm::rotate(mat4(1.0f), radians(270.0f), vec3(0.0f, 1.0f, 0.0f));
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(angle2, 0.5, 0.5));
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0));
	model_matrix = _translate * _scale*_rotate;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);

	material_ambient = vec4(1.0, 0.5, 0.0, 1.0);
	material_diffuse = vec4(0.0, 0.60, 1.0, 1.0);
	material_specular = vec4(0.80, 0.85, 0.75, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);

	 normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	
	setMetrix();
	glBindTexture(GL_TEXTURE_2D, planeTexID);
	renderPyramid();

	model_matrix = mat4(1.0f);
	if (angle1 == 10) {
		angle1 = 5;

	}
	_rotate = glm::rotate(mat4(1.0f), radians(270.0f), vec3(0.0f, 0.0f, 1.0f));
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(angle2/angle1, angle2/ angle1, angle2/ angle1));
	if (angle2 ==1.0) {
		angle2 = -1.50;

	}
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(angle2, -angle2, angle2));
	model_matrix = _translate * _scale*_rotate;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	 light_ambient= vec4(0.5, 0.5, 0.5, 1.0);
	 light_diffuse= vec4(1.0, 1.0, 1.0, 1.0);
	 light_specular= vec4(1.0, 1.0, 1.0, 1.0);


	material_ambient = vec4(0.33, 0.15, 0.15, 1.0);
	material_diffuse = vec4(0.0, 0.8, 0.25, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);

	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	
	setMetrix();
	renderDisk();
	model_matrix = mat4(1.0f);
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));
	//_rotate = glm::rotate(mat4(1.0f), radians(180.0f-angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -2.0f, 0.0));
	model_matrix = _translate * _scale;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	light_ambient = vec4(0.5, 0.5, 0.5, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);


	material_ambient = vec4(0.33, 0.15, 0.63, 1.0);
	material_diffuse = vec4(0.0, 0.8, 0.25, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);
	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	setMetrix();
	
	renderSphere();
	//******************************
	model_matrix = mat4(1.0f);
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));
	//_rotate = glm::rotate(mat4(1.0f), radians(180.0f-angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, 0.0));
	model_matrix = _translate * _scale;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	light_ambient = vec4(0.5, 0.5, 0.5, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);


	material_ambient = vec4(0.33, 0.15, 0.63, 1.0);
	material_diffuse = vec4(0.0, 0.8, 0.25, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);
	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	setMetrix();
	renderCube();
	//*****************************
	//**********************************
	model_matrix = mat4(1.0f);
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));
	//_rotate = glm::rotate(mat4(1.0f), radians(180.0f-angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -1.0));
	model_matrix = _translate * _scale;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	light_ambient = vec4(0.5, 0.5, 0.5, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);


	material_ambient = vec4(0.33, 0.15, 0.63, 1.0);
	material_diffuse = vec4(0.0, 0.8, 0.25, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);
	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	setMetrix();
	renderHexagon();
	
	///*********************cone*******************************************************************
	model_matrix = mat4(1.0f);
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));
	//_rotate = glm::rotate(mat4(1.0f), radians(180.0f-angle1), vec3(0.0f, 1.0f, 0.0f));
	_translate = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0));
	model_matrix = _translate * _scale;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	light_ambient = vec4(0.1, 0.1, 0.1, 1.0);
	light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	light_specular = vec4(1.0, 1.0, 1.0, 1.0);


	material_ambient = vec4(0.3, 0.3, 0.3, 1.0);
	material_diffuse = vec4(0.0, 0.8, 0.25, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);
	normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);
	setMetrix();
	renderCone();
	
	///-------------
glutSwapBuffers();

	//glFlush();
}

void setMetrix() {
	//projection_matrix = perspective(radians(60.0f), aspect, 1.0f, 3000.0f);
	//projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -9.0f, 9.0f);
	//projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -11.0f, 11.0f);
	glUniformMatrix4fv(glGetUniformLocation(program, "project_matrix"), 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, (GLfloat*)&view[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
}

/*******************************************************************************************************************/

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'l':
	case 'L':
		show_line = !show_line;
		break;
	case 'o':case 'O':
		orthographic = !orthographic;

	}
	glutPostRedisplay();
}

/*********************************************************************************************/
void Rotate(int n)  //the "glutTimerFunc"
{
	
	switch (n) {
	case 1:
		angle1 += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	case 2:
		angle2 += 0.125/2;
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
/*******************************************************************************************************************/
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	//glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Drawing objects");

	if (glewInit()) {
		printf("Unable to initialize GLEW ... exiting\n");
	}

	init();
	printf("%s", glGetString(GL_VERSION));
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, Rotate, 1);
	glutTimerFunc(150, Rotate, 2);
	glutTimerFunc(100, Rotate, 3);
	glutMainLoop();
}

/*****/
