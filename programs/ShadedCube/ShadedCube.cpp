#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

GLuint  cube_vao;
GLuint  cube_vbo[2];
GLuint  cube_ebo;


mat4 view;
mat4 projection;

GLuint program;

float aspect = 0.0;
bool show_line = false;
bool orthographic = false;

float vertices[] = { -0.5, -0.5, 0.5, 1.0f, //Front
0.5, -0.5, 0.5, 1.0f,
0.5, 0.5, 0.5, 1.0f,
-0.5, 0.5, 0.5, 1.0f,

0.5, -0.5, 0.5, 1.0f, //Right
0.5, -0.5, -0.5, 1.0f,
0.5, 0.5, -0.5, 1.0f,
0.5, 0.5, 0.5, 1.0f,

-0.5, -0.5, -0.5, 1.0f, //Back
-0.5, 0.5, -0.5, 1.0f,
0.5, 0.5, -0.5, 1.0f,
0.5, -0.5, -0.5, 1.0f,
//Left
-0.5, -0.5, 0.5, 1.0f,
-0.5, 0.5, 0.5, 1.0f,
-0.5, 0.5, -0.5, 1.0f,
-0.5, -0.5, -0.5, 1.0f,
//Bottom
-0.5, -0.5, 0.5, 1.0f,
-0.5, -0.5, -0.5, 1.0f,
0.5, -0.5, -0.5, 1.0f,
0.5, -0.5,  0.5, 1.0f,
//Top
-0.5, 0.5, 0.5, 1.0f,
0.5, 0.5, 0.5, 1.0f,
0.5, 0.5, -0.5, 1.0f,
-0.5, 0.5, -0.5, 1.0f
};



float normals[] = {
	// Front
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	// Right
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	// Back
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	// Left
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// Bottom
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	// Top
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

GLuint indices[] = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10, 8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
};
static const double kPI = 3.1415926535897932384626433832795;

// material properties

float material_shininess = 5.0;

vec4 light_position(10.0, 10.0, 9.8, 1.0);  // positional light source
vec4 light_ambient(0.3, 0.3, 0.3, 1.0);
vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
vec4 light_specular(1.0, 1.0, 1.0, 1.0);

vec4 material_ambient(1.0, 0.0, 1.0, 1.0);
vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
vec4 material_specular(1.0, 0.0, 1.0, 1.0);

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


	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(2, cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &cube_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (indices), GL_STATIC_DRAW);

	// attribute indices
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	

	view = glm::lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	
	// Set other lighting parameters here

	
	projection = mat4(1.0f);
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);

}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Choose whether to draw in wireframe mode or not

	if (show_line)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	mat4 model = glm::translate(glm::mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));

	if (orthographic)
	    projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 4.3f, 100.0f);
	else
		projection = glm::perspective(radians(70.0f), aspect, 4.3f, 100.0f);
	mat4 mvp = projection*view*model;
	mat4 model_view = view*model;

	mat3 normal = mat3(view*model);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normal[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);

	// You need to add normal matrix and model view matrix

	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);


	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
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

/*********/
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Shaded Cube");

	if (glewInit()){
		printf("Unable to initialize GLEW ... exiting\n");
	}

	Initialize();
	printf("%s\n",glGetString(GL_VERSION));
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}




/*************/
