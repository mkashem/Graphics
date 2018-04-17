#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "grass.h"
#include "ground.h"
#include "box.h"
#include "pyramid.h"
#include "cone.h"
#include "fan.h"

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

using namespace glm;

void Initialize(void);
void Display(void);

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;
mat4 transformation_matrix;
mat4 mvp;
mat3 normalmatrix;

GLuint prog;
GLuint render_prog;
GLuint progTexture;
float angle =0.0;
GLboolean speed_up;

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;




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

GLuint planeTexID;
GLuint planeTexID_3;
GLuint planeTexID_2;
GLuint planeTexID_4;
unsigned int loadTexture(char* filename);
//----------------------------------------------------------------------------
void setMatrices();
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


void Reshape(int width, int height) {

	glViewport(0, 0, width, height);
	aspect = float(width) / float(height);
}
/************************************** https://evals.compsci.ewu.edu***************************************/
////////////////////////////////////////////////////////////////////////////////////////


/**************************/

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

	
	

	createGround();
	glUseProgram(prog);
	createCone();

	createBox();
	createPyramid();
	createFan();
	view=glm::lookAt(glm::vec3(0.0f, 40.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	ShaderInfo common_texture[] = { { GL_VERTEX_SHADER, "texture.vs" },
	{ GL_FRAGMENT_SHADER, "texture.fs" },
	{ GL_NONE, NULL } };

	progTexture = LoadShaders(common_texture);
	glUseProgram(progTexture);
	
	vec3 light_intensity(1.0f, 1.0f, 1.0f);
	vec4 light_position(10.0f, 10.0f, 10.0f, 1.0f);
	vec3 material_ambient(0.5, 0.5, 0.5);
	vec3 material_diffuse(0.9, 0.9, 0.9);
	vec3 material_specular(1.0, 1.0, 1.0);

	GLfloat shininess = 100.0f;

	glUniform3fv(glGetUniformLocation(progTexture, "Light.Intensity"), 1, (GLfloat*)&light_intensity);
	glUniform4fv(glGetUniformLocation(progTexture, "Light.Position"), 1, (GLfloat*)&light_position);
	glUniform3fv(glGetUniformLocation(progTexture, "Material.Ka"), 1, (GLfloat*)&material_ambient);
	glUniform3fv(glGetUniformLocation(progTexture, "Material.Kd"), 1, (GLfloat*)&material_diffuse[0]);
	glUniform3fv(glGetUniformLocation(progTexture, "Material.Ks"), 1, (GLfloat*)&material_specular[0]);
	glUniform1f(glGetUniformLocation(progTexture, "Material.Shininess"), shininess);
	
	glUniform1i(glGetUniformLocation(progTexture, "Tex1"), 0);
	planeTexID = loadTexture("home.jpg");   // call texture function 
	planeTexID_2 = loadTexture("top.jpg");
	planeTexID_3 = loadTexture("wood.jpg");
	planeTexID_4 = loadTexture("white.jpg");
	glActiveTexture(GL_TEXTURE0);             // active texture
											  // bind texture 

	//glClearColor(1.0, 1.0, 1.0, 1.0);
	
	
	
	glClearColor(0.5f, 0.75f, 0.85f, 1.0f);  // sky color
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

	
	/********************************* first tree start *****************************************************************/

	

	model_matrix = mat4(1.0f);
	mat4 cone_scale, cone_translate;
	cone_scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 25.0f, 20.0f));
	cone_translate = glm::translate(glm::mat4(1.0f), glm::vec3(140.0f, 50.0f, -50.0));
	model_matrix = cone_translate*cone_scale;
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	material_ambient = vec4(0.0, 0.9, 0.104, 1.0);
	material_diffuse = vec4(0.0, 1.64, 0.104, 1.0);
	material_specular = vec4(0.1, 0.25, 0.25, 1.0);
	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;
	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(prog, "Shininess"), material_shininess);
	renderCone();

	model_matrix = mat4(1.0f);
	mat4 cone2_scale, cone2_translate;
	cone2_scale = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 35.0f, 15.0f));
	cone2_translate = glm::translate(glm::mat4(1.0f), glm::vec3(140.0f, 00.0f, -50.0));
	model_matrix = cone2_translate*cone2_scale;
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	material_ambient = vec4(.74, 0.64, 0.104, 1.0);
	material_diffuse = vec4(0.74, 0.64, 0.104, 1.0);
	material_specular = vec4(0.1, 0.25, 0.25, 1.0);
	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;
	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(prog, "Shininess"), material_shininess);

	renderCone();


	/********************* first tree end *************************************/
	

	/********************************* second tree start *****************************************************************/
	
	

	model_matrix = mat4(1.0f);
	mat4 cone4_scale, cone4_translate;
	cone4_scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 35.0f, 20.0f));    //top tree
	cone4_translate = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 70.0f, -70.0));
	model_matrix = cone4_translate*cone4_scale;
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	material_ambient = vec4(0.0, 0.9, 0.104, 1.0);
	material_diffuse = vec4(0.0, 1.64, 0.104, 1.0);
	material_specular = vec4(0.1, 0.25, 0.25, 1.0);
	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;
	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(prog, "Shininess"), material_shininess);
	renderCone();

	
	model_matrix = mat4(1.0f);
	mat4 cone3_scale, cone3_translate;
	cone3_scale = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 45.0f, 15.0f)); //bottom tree
	cone3_translate = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -70.0));
	model_matrix = cone3_translate*cone3_scale;
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	material_ambient = vec4(.74, 0.64, 0.104, 1.0);
	material_diffuse = vec4(0.74, 0.64, 0.104, 1.0);
	material_specular = vec4(0.1, 0.25, 0.25, 1.0);
	ambient_product = light_ambient*material_ambient;
	diffuse_product = light_diffuse*material_diffuse;
	specular_product = light_specular*material_specular;
	glUniform4fv(glGetUniformLocation(prog, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform4fv(glGetUniformLocation(prog, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(prog, "Shininess"), material_shininess); 
	renderCone();
	
	/********************* first tree end *************************************/
/*     Bellow this line all objects used progTexture sheder           */

	glUseProgram(progTexture);
	
	model = mat4(1.0f);
	glBindTexture(GL_TEXTURE_2D, planeTexID);
	model = mat4(1.0f);
	mat4 box_scale, box_translate;
	box_scale = glm::scale(glm::mat4(1.0f), glm::vec3(200.5f, 70.5f, 200.50));
	box_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 18.0f, -10.0f));
	model = box_translate*box_scale;
	setMatrices();
	renderBox();
	model = mat4(1.0f);
	mat4 pyra_scale, pyra_translate;
	pyra_scale = glm::scale(glm::mat4(1.0f), glm::vec3(40.f, 10.5f, 40.0));
	pyra_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 40.0f, 0.0f));
	model = pyra_translate*pyra_scale;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID_2);
	renderPyramid(); 
	/* *******************/
	if (speed_up) {
		angle += 10;
	}
	/* ********************/
	model = mat4(1.0f);
	mat4 fan_scale, fan_rotate, fan_translate;
	
	fan_rotate = glm::rotate(mat4(1.0f), radians(0.0f- angle), vec3(0.0f, 0.0f, 1.0f));
	fan_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -50.0f));
	//model = fan_scale*fan_translate*fan_rotate;
	model = fan_translate*fan_rotate;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID_3);
	renderFan();
	
	model = mat4(1.0f);
	mat4 fan_scale2, fan_rotate2, fan_translate2;
	
	fan_rotate2 = glm::rotate(mat4(1.0f), radians(90.0f-angle), vec3(0.0f, 0.0f, 1.0f));
	fan_translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -50.0f));
	
	model = fan_translate2*fan_rotate2;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID_3);
	renderFan();

	model = mat4(1.0f);
	mat4 fan_scale3, fan_rotate3, fan_translate3;
	
	fan_rotate3 = glm::rotate(mat4(1.0f), radians(180.0f- angle), vec3(0.0f, 0.0f, 1.0f));
	fan_translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -50.0f));
	model = fan_translate3*fan_rotate3;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID_3);
	renderFan();

	model = mat4(1.0f);
	mat4 fan_scale4, fan_rotate4, fan_translate4;
	
	fan_rotate4 = glm::rotate(mat4(1.0f), radians(270.0f- angle), vec3(0.0f, 0.0f, 1.0f));
	fan_translate4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -50.0f));
	//model = fan_scale2*fan_rotate2*fan_translate2;
	model = fan_translate4*fan_rotate4;
	setMatrices();
	glBindTexture(GL_TEXTURE_2D, planeTexID_3);
	renderFan();

	


	model = mat4(1.0f);
	glBindTexture(GL_TEXTURE_2D, planeTexID_3);
	model = mat4(1.0f);
	mat4 box_scale2, box_translate2;
	box_scale2 = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 480.0f, 100.0f));;
	box_translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, -100.0f));
	model = box_translate2*box_scale2;
	setMatrices();
	renderBox();
	
	glutSwapBuffers();
	

}



void Rotate(int n)  //the "glutTimerFunc"
{
	// implement this function!
	
	
		angle += 5.0f;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, n);
		//glutReshapeFunc(Reshape);
	


}

void setMatrices() {

	normalmatrix = mat3(1.0f);
	mvp = mat4(1.0f);
	model_view = view*model;
	normalmatrix = mat3(vec3(model_view[0]), vec3(model_view[1]), vec3(model_view[2]));
	projection = perspective(radians(60.0f), aspect, 1.0f, 3000.0f);
	mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(progTexture, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	glUniformMatrix3fv(glGetUniformLocation(progTexture, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(progTexture, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(progTexture, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
}

/************************************************************************/
void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	
	case 's':case 'S':
		speed_up = !speed_up;
		

		break;

	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);

	glutCreateWindow("Scenery");

	if (glewInit()) {
		printf("Unable to initialize GLEW ... exiting\n");
	}
	ilInit();
	Initialize();
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(Display);
	glutTimerFunc(100, Rotate, 0);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);// for keyboard call back function
	glutMainLoop();
	return 0;
}

/*****/



/*************/