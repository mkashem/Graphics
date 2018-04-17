#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.1415926535897932384626433832795

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "Plane.h"
#include "box.h"
#include "pyramid.h"
#include "cone.h"
#include "circle.h"

using namespace glm;


mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;
mat4 transformation_matrix;
mat4 mvp;
mat3 normalmatrix;
static int gViewportWidth, gViewportHeight;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;
mat4 gCameraRotation;
GLuint program;
float aspect = 0.0;
GLfloat angle = 0.0;
float angle2 = 45.0*3.14 / 180; ///
float tri_look = 45.0*3.14 / 180;
float tri_angle = 45.0*3.14 / 180;
float angle_1 = acos(3.0 / sqrt(13.0));
float angle_2 = acos(sqrt(13.0 / 14.0));

GLuint planeTexID;

GLuint planeTexID_2;

static const double kPI = 3.1415926535897932384626433832795;
typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;


void Initialize();
void Display(void);
void Reshape(int width, int height);
unsigned int loadTexture(char* filename);
GLuint LoadShaders(ShaderInfo* shaders);
const GLchar* ReadShader(const char* filename);
void setMatrices();
void setMatricesBox();

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

GLuint LoadShaders(ShaderInfo* shaders) {
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

/************************************/
unsigned int loadTexture(char* filename) {

	ILboolean success;
	unsigned int imageID;


	// Load Texture Map
	ilGenImages(1, &imageID);

	ilBindImage(imageID); /* Binding of DevIL image name */
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	success = ilLoadImage((ILstring)filename);

	if (!success) {
		printf("Couldn't load the following texture file: %s", filename);
		// The operation was not sucessfull hence free image and texture 
		ilDeleteImages(1, &imageID);
		return 0;
	}

	// add information to the log
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	
	
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(1, &imageID);
	return tid;
}



/************************************/
void Initialize(void){
	// Create the program for rendering the model

	createPlane();
	createBox();
	createPyramid();
	createCone();
	createCircle();
	
	// Use our shader

	//view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	view = glm::lookAt(vec3(1.5*cos(radians(angle)), 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
	
	ShaderInfo shader[] = { { GL_VERTEX_SHADER, "texture.vs" },
	{ GL_FRAGMENT_SHADER, "texture.fs" },
	{ GL_NONE, NULL }
	};

	program = LoadShaders(shader);

	glUseProgram(program);


	vec3 light_intensity(1.0f, 1.0f, 1.0f);
	vec4 light_position(10.0f, 10.0f, 10.0f, 1.0f);
	vec3 material_ambient(0.5, 0.5, 0.5);
	vec3 material_diffuse(0.9, 0.9, 0.9);
	vec3 material_specular(0.9, 0.9, 0.9);

	GLfloat shininess = 100.0f;

	glUniform3fv(glGetUniformLocation(program, "Light.Intensity"), 1, (GLfloat*)&light_intensity);
	glUniform4fv(glGetUniformLocation(program, "Light.Position"), 1, (GLfloat*)&light_position);
	glUniform3fv(glGetUniformLocation(program, "Material.Ka"), 1, (GLfloat*)&material_ambient);
	glUniform3fv(glGetUniformLocation(program, "Material.Kd"), 1, (GLfloat*)&material_diffuse[0]);
	glUniform3fv(glGetUniformLocation(program, "Material.Ks"), 1, (GLfloat*)&material_specular[0]);
	glUniform1f(glGetUniformLocation(program, "Material.Shininess"), shininess);
	glUniform1i(glGetUniformLocation(program, "Tex1"), 0);
	planeTexID = loadTexture("planet.jpg");   // call texture function 
	planeTexID_2 = loadTexture("brick.jpg");
	glActiveTexture(GL_TEXTURE0);             // active texture
	   // bind texture 
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
/****************************************************************************/
void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(program);
	
	model = mat4(1.0f);
	
	setMatrices();
	//setMatricesBox();
	glBindTexture(GL_TEXTURE_2D, planeTexID_2);
	renderPlane();
	model = mat4(1.0f);
	mat4 box_scale, box_translate;
	box_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0));
	box_translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -0.6f, 0.0));
	model = box_translate*box_scale;
	setMatrices();
	renderBox();

	model = mat4(1.0f);
	mat4 pyra_scale, pyra_translate;
	pyra_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));
	pyra_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.6f, 0.0));
	model = pyra_translate*pyra_scale;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID);
	renderPyramid();
	// for cone 
	model = mat4(1.0f);
	mat4 cone_scale, cone_translate;
	cone_scale = glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, 0.5));
	cone_translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.6f, -1.0));
	model = cone_translate*cone_scale;
	setMatrices();
	renderCone();
	
	//for circle
	model = mat4(1.0f);
	mat4 circle_scale, circle_translate;
	//circle_scale = glm::scale(glm::mat4(1.0f), glm::vec3(.7f, .7f, .7f));
	circle_scale = glm::scale(glm::mat4(1.0f), glm::vec3(.7f, .7f, .7f));
	circle_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0));
	model = circle_translate*circle_scale;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID);
	renderCircle();

	glBindVertexArray(0);


	glutSwapBuffers();
}

/******************************************************************************/
void setMatrices(){

	normalmatrix = mat3(1.0f);
	mvp = mat4(1.0f);
	model_view = view*model;
	normalmatrix = mat3(vec3(model_view[0]), vec3(model_view[1]), vec3(model_view[2]));
	projection = glm::perspective(radians(70.0f), aspect, 0.3f, 100.0f);
	mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
}

void setMatricesBox() {

	normalmatrix = mat3(1.0f);
	mvp = mat4(1.0f);
	model_view = view*model;
	normalmatrix = mat3(vec3(model_view[0]), vec3(model_view[1]), vec3(model_view[2]));
	projection = glm::perspective(radians(70.0f), aspect, 0.3f, 100.0f);
	mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
}


/********************************************************************************/

void Reshape(int width, int height){

	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}
/*****************************************************************************/

void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	}
	glutPostRedisplay();
}

void Rotate(int n) {

	switch (n)
	{
	case 1:
		angle2 += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	case 2:
		tri_angle += 10;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 2); break;
	case 3:
		tri_look += 10;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 3);
		break;
	default:
		break;
	}


};

/*********************************************************************************************/

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Textured Plane");

	if (glewInit()){
		printf("Unable to initialize GLEW ... exiting\n");
	}

	ilInit();
	Initialize();
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Rotate, 1);
	glutTimerFunc(100, Rotate, 2);
	glutTimerFunc(100, Rotate, 3);

	glutMainLoop();
	return 0;
}


