#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Triangle.h"
#include "cone.h"
#include <math.h>
#define GLM_FORCE_RADIANS 
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;


bool change_projection;
GLuint program;
GLuint v, f;
GLboolean show_line = false;
void setMetrix();
mat4 model_matrix;
mat4 view, projection_matrix;
GLfloat angle_rotate = 0.0;
GLfloat angle_lookup = 0.0;
GLfloat tri_angle = 0.0;
static const double kPI = 3.1415926535897932384626433832795;
void Rotate(int n);

// position of the vertices in the hexagon

/*--------------------------------------------------------------------------------------*/

const GLchar* ReadShader(const char* filename);
GLuint LoadShaders(char* v_shader, char* f_shader);
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
//----------------------------------------------------------------------------
float material_shininess = 50.0;
vec4 light_position(10.0, 10.0, 9.8, 1.0); // positional light source
vec4 light_ambient(0.3, 0.3, 0.3, 1.0);
vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
vec4 light_specular(1.0, 1.0, 1.0, 1.0);

vec4 material_ambient(1.0, 1.0, 0.0, 1.0);
vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
vec4 material_specular(1.0, 0.0, 1.0, 1.0);

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

void init(void) {

	 program = LoadShaders((char*)"shader.vert", (char*)"shader.frag");
	glUseProgram(program);
	//view = glm::lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -100.0), vec3(0.0, 1.0, 0.0)); // this is the default camera positioin 
	
 //view = glm::lookAt(vec3(1.5*cos(radians(tri_look)), 0.0f, 1.5*sin(radians(tri_look))), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)); // change the camera position 

	createTriangle();
	createCone();

	glClearColor(1.0, 1.0, 1.0, 1.0);

}

/****************************************************************************************/
void display(void) {


	glClear(GL_COLOR_BUFFER_BIT);

	
	//mat4 m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 100.0f, 0.0f), vec3(2.0f, 0.0f, 0.0f));
	model_matrix = mat4(1.0f);
	//model_matrix = mat4(1, 0.5, 0, 0, 0.5, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	view = glm::lookAt(vec3(3.0f*cos(radians(angle_lookup)), 0.0f, 3.0f*sin(radians(angle_lookup))), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f); // enlarge the the view 
	//projection_matrix = glm::frustum(-2.0f, 2.0f, -2.0f, 2.0f, 0.65f, 10.0f);
	
	mat4 tri_scale, tri_translate;
	tri_scale = glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, 0.5));
	tri_translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0));
	model_matrix = tri_translate * tri_scale;


	if (show_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	setMetrix();
	renderTriangle();
	// for cone 
	model_matrix = mat4(1.0f);
	mat4 cone_scale, cone_translate;
	cone_scale = glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, .5));
	cone_translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.6f, -1.0));
	model_matrix = cone_translate * cone_scale;

	//consider material ambient
		vec4 material_ambient(1.0, 0.0, 1.0, 1.0);
	vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	vec4 material_specular(1.0, 0.0, 1.0, 1.0);


	// compute total ambient, diffuse and specular
	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;


	//vec4 ambient_product = light_ambient*material_ambient;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	
	setMetrix();
	mat3 normal = mat3(view*model_matrix);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE,
		(GLfloat*)&normal[0]);

	renderCone();

	glFlush();
}

void setMetrix() {

	projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -8.0f, 8.0f);
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

	}
	glutPostRedisplay();
}

/*********************************************************************************************/
void Rotate(int n)  //the "glutTimerFunc"
{
	switch (n) {
	case 1:
		angle_rotate += 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	case 2:
		tri_angle -= 10;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 2);
		break;
	case 3:
		angle_lookup -= 5;
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 3);
		break;
	}
}
/*******************************************************************************************************************/
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Drawing objects");

	if (glewInit()) {
		printf("Unable to initialize GLEW ... exiting\n");
	}

	init();
	printf("%s", glGetString(GL_VERSION));
	glutDisplayFunc(display);
	glutTimerFunc(100, Rotate, 1);
	glutTimerFunc(100, Rotate, 2);
	glutTimerFunc(100, Rotate, 3);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

/*****/
