#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


GLuint  cube_vao;
GLuint  cube_vbo;
GLuint  cube_ebo;
GLuint loc;
GLuint normal_loc;
GLuint color_loc;
GLuint matrix_loc;
GLuint projection_matrix_loc;
GLuint view_matrix_loc;
GLuint program;
glm::mat4 view_matrix;
static const double kPI = 3.1415926535897932384626433832795;
using namespace glm;


static const GLfloat cube_vertices[] = { 1.0f, 1.0f, 1.0f, 1.0f,  // v0,v1,v2,v3 (front)
-1.0f, 1.0f, 1.0f, 1.0f,
-1.0f, -1.0f, 1.0f, 1.0f,
1.0f, -1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f, 1.0f,  // v0,v3,v4,v5 (right)
1.0f, -1.0f, 1.0f, 1.0f,
1.0f, -1.0f, -1.0f, 1.0f,
1.0f, 1.0f, -1.0f, 1.0f,
1.0f, 1.0f, 1.0f, 1.0f,  // v0,v5,v6,v1 (top)
1.0f, 1.0f, -1.0f, 1.0f,
-1.0f, 1.0f, -1.0f, 1.0f,
-1.0f, 1.0f, 1.0f, 1.0f,
-1.0f, 1.0f, 1.0f, 1.0f, // v1,v6,v7,v2 (left)
-1.0f, 1.0f, -1.0f, 1.0f,
-1.0f, -1.0f, -1.0f, 1.0f,
-1.0f, -1.0f, 1.0f, 1.0f,
-1.0f, -1.0f, -1.0f, 1.0f,// v7,v4,v3,v2 (bottom)
1.0f, -1.0f, -1.0f, 1.0f,
1.0f, -1.0f, 1.0f, 1.0f,
-1.0f, -1.0f, 1.0f, 1.0f,
1.0f, -1.0f, -1.0f, 1.0f,// v4,v7,v6,v5 (back)
-1.0f, -1.0f, -1.0f, 1.0f,
-1.0f, 1.0f, -1.0f, 1.0f,
1.0f, 1.0f, -1.0f, 1.0f };



static const GLushort cube_indices[] = { 0, 1, 2, 2, 3, 0,      // front
4, 5, 6, 6, 7, 4,      // right
8, 9, 10, 10, 11, 8,      // top
12, 13, 14, 14, 15, 12,      // left
16, 17, 18, 18, 19, 16,      // bottom
20, 21, 22, 22, 23, 20 };    // back




static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void Initialize();
void Display(void);

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

	GLchar* source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;

	return (GLchar*) (source);
}

GLuint initShaders(const char *v_shader, const char *f_shader) {

	GLuint p = glCreateProgram();
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	free((char*)vs);
	free((char*) fs);

	glCompileShader(v);
	GLint compiled;
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(len + 1));
		glGetShaderInfoLog(v, len, &len, log);
		printf("Vertex Shader compilation failed: %s\n", log);
		free(log);


	}

	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(len + 1));
		glGetShaderInfoLog(f, len, &len, log);
		printf("Fragment Shader compilation failed: %s\n", log);
		free((char*)log);
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	GLint linked;
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = (GLchar*)malloc(sizeof(len + 1));
		glGetProgramInfoLog(p, len, &len, log);
		printf("Shader linking failed: %s\n", log);
		free(log);
	}
	glUseProgram(p);
	return p;

}

void Initialize(void) {
	// Create the program for rendering the model

	program = initShaders("cube_shader.vs", "cube_shader.fs");


	// attribute indices

	loc = glGetAttribLocation(program, "position");

	// uniform indices

	matrix_loc = glGetUniformLocation(program, "model_matrix");

	projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");

	glm::mat4 projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &cube_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);


	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}


void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Setup


	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);


	glm::mat4 model_matrix;
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 0.0));

	
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);

	glFlush();
}

/*********/
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow(argv[0]);
	
	if (glewInit()) {
		printf("Unable to initialize GLEW ... exiting\n");
	}

	Initialize();
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(Display);

	glutMainLoop();
	return 0;
}




/*************/



