#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Triangle.h"



GLuint v, f;
GLboolean show_line = false;

// position of the vertices in the hexagon

/*--------------------------------------------------------------------------------------*/

const GLchar* ReadShader(const char* filename);
GLuint LoadShaders(char* v_shader, char* f_shader);
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
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

	GLuint program = LoadShaders("shader.vert", "shader.frag");
	glUseProgram(program);

	createTriangle();

	glClearColor(1.0, 1.0, 1.0, 1.0);

}

/****************************************************************************************/
void display(void) {


	glClear(GL_COLOR_BUFFER_BIT);

	if (show_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	renderTriangle();


	glFlush();
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

/*****/
