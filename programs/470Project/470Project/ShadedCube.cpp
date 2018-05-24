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
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.141592653589793238
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;
GLuint planeTexID;
mat4 view;
mat4 projection;
float positionX;
float positionY;
float positionZ;
mat4 model;
mat4 mvp;
mat4 model_view ;
mat4 _scale;
mat4 _translate;
mat4 _rotate;

mat3 normal;
void sphereOne();
void sphereTwo();
void sphreThree();
void cubeOne();

GLuint program;

float aspect = 0.0;
bool show_line = false;
bool orthographic = false;
bool forward = false;
bool upword = false;
bool left = false;
bool down = false;
void Rotate(int n);
float angle1 = 0.0;
float angle2 = -1.0;
float angle3 = 0.0;
float move = 0.0;
float sph3X = -3.5f;
float sph2X = -3.0f;
float sph1X = -4.0f;

void updateTime();
GLfloat timeUpdate = 0.0;




static const double kPI = 3.1415926535897932384626433832795;

// material properties

float material_shininess = 5.0;


unsigned int loadTexture(char* filename);
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

//unsigned int loadTexture(char* filename) {
//
//	ILboolean success;
//	unsigned int imageID;
//
//
//	// Load Texture Map
//	ilGenImages(1, &imageID);
//
//	ilBindImage(imageID); /* Binding of DevIL image name */
//	ilEnable(IL_ORIGIN_SET);
//	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
//	success = ilLoadImage((ILstring)filename);
//
//	if (!success) {
//		printf("Couldn't load the following texture file: %s", filename);
//		// The operation was not sucessfull hence free image and texture 
//		ilDeleteImages(1, &imageID);
//		return 0;
//	}
//
//	// add information to the log
//	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//
//	GLuint tid;
//	glGenTextures(1, &tid);
//	glBindTexture(GL_TEXTURE_2D, tid);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
//		GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//
//	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
//
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	/* Because we have already copied image data into texture data
//	we can release memory used by image. */
//	ilDeleteImages(1, &imageID);
//	return tid;
//}

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
	/*glUniform1i(glGetUniformLocation(program, "Tex1"), 0);
	planeTexID = loadTexture((char*)"brick.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTexID);*/

	
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

	 model = glm::translate(glm::mat4(1.0f), vec3(3.0f, 2.0f, 0.0f));

	if (orthographic)
	    projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f-angle1), aspect, 4.3f, 100.0f);
	 mvp = projection*view*model;
	 model_view = view*model;

	 normal = mat3(view*model);
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

	renderDisk();
	//****************************************
	//**********************************
	

	if (upword) {
		
		positionY -= (0.0625 / 2.0);
	}
	else
		positionY;
	if (left) {
		positionX -= 0.0625;
	}
	else
		left;
	if (down) {
		positionY += (0.0625/2.0);
	}
	else
		down;
	if (positionY == -7.0) {
		upword = false;
		left = true;
	}
	if (positionX==-1.0) {
		left = false;
		down = true;
	}
	if (positionY == 0.0) {
		down = false;
		forward = true;
	}


	if (forward) {  // start carsb
		positionZ = 0.0625;
		positionX+=positionZ;
		if (positionX==5.0) {
			forward=false;

			upword =true;
		}	
	}	
	else
		positionX;
	
	sphereOne();
	sphereTwo();
	sphreThree();
	cubeOne();
	
	//****************************************
	//**********************************
	
	glutSwapBuffers();
}

void sphereOne() {
	if (orthographic)
		projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.2, 0.2));
	_translate=glm::translate(glm::mat4(1.0f), vec3(sph1X + positionX, -2.0f , 0.0f + positionY));
	model = _translate * _scale;
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	renderSphere();
}

void sphereTwo() {
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.2, 0.2));
	_translate=glm::translate(glm::mat4(1.0f), vec3(sph2X + positionX, -2.0f , 0.0f + positionY));
	model = _translate * _scale;
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);

	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	renderSphere();
}

void sphreThree() {
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.2, 0.2));
	_translate = glm::translate(glm::mat4(1.0f), vec3(sph3X + positionX, -2.0f , 0.0f + positionY));
	model = _translate * _scale;
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);

	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	renderSphere();
}
void cubeOne() {
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5, 0.6, 0.6));
	_translate = glm::translate(glm::mat4(1.0f), vec3(-5.0f + positionX, -2.0f , 0.0f + positionY));
	model = _translate * _scale;
	mvp = projection * view*model;
	model_view = view * model;

	normal = mat3(view*model);

	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	renderCube();
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
	case 'f':
		forward = !forward;
		break;
	case 'o':case 'O':
		orthographic = !orthographic;
	}
	glutPostRedisplay();
}
void updateTime() {
	timeUpdate += 0.1f;
}
void Rotate(int n)  //the "glutTimerFunc"
{

	switch (n) {
	case 1:
		updateTime();
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
	case 4:
		move = 0.25;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 4);
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
	glutTimerFunc(100, Rotate, 2);
	glutTimerFunc(100, Rotate, 3);
	glutTimerFunc(100, Rotate, 4);
	glutMainLoop();
	return 0;
}




/*************/
