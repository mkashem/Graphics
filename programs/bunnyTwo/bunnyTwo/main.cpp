#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.141592653589793238

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "objloader.h"
#include "shader.h"

using namespace glm;
using namespace std;

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;
GLuint program;
float aspect = 0.0;
GLfloat angle = 0.0;
GLuint vao;
unsigned int vbo[1];
GLuint ebo;

GLboolean showLines = false;

vector<vec3> vertices;
vector<vec2> textures;
vector<vec3> normals; 
vector<int> indices;
int numverts;

vector<vec3> tempVertices;
vector<vec3> tempNormals;
unsigned int loadTexture(char* filename);
GLuint planeTexID;

static const double kPI = 3.1415926535897932384626433832795;
static int gViewportWidth, gViewportHeight;
vec4 light_position(10.0, 6.0, 8.0, 0.0);  // directional light source
GLfloat timeUpdate = 0.0;

void Initialize();
void Display(void);
void Reshape(int width, int height);
void updateTime();


void updateBunnyVertex(vector<vec3> &ptr, vector<vec3> verts, vector<vec3> &norms, int count);
void updateBunnyNormal(vector<vec3> &vNormals, vector<vec3> vertices, vector<int> indices);
/*******************************/
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

/***************************************************************************/
void Initialize(void){

	// Create the program for rendering the model

	objloader loader;

	bool loadfile = loader.load("bunnyCord2.obj");
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();
	numverts = normals.size();
	textures = loader.getCoordinates();
	cout << "size" << textures.size();
	tempVertices = loader.getVertices();
	tempNormals = loader.getNormals();

	// Create and compile our GLSL program from the shaders
	program = LoadShaders("shader.vs", "shader.fs");

	

	glUseProgram(program);
	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	// Initialize shader lighting parameters

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint offset = 0;
	glGenBuffers(1, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3)+ (textures.size() * sizeof(glm::vec2)), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size()*sizeof(glm::vec3), &vertices[0]);
	offset += vertices.size()*sizeof(glm::vec3);

	glBufferSubData(GL_ARRAY_BUFFER, offset, normals.size() * sizeof(glm::vec3), &normals[0]);
	offset += normals.size() * sizeof(glm::vec3);

	glBufferSubData(GL_ARRAY_BUFFER, offset, textures.size()*sizeof(glm::vec2), &textures[0]);
	
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); // for vertex

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertices.size() * sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);  // Vertex normal
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertices.size() * sizeof(glm::vec3)+(normals.size() * sizeof(glm::vec3) ))  );
	glEnableVertexAttribArray(2);  // Texture



	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	vec4 lightintensity = vec4(0.9f, 0.9f, 0.9f, 1.0f);
	vec4 material_ambient(0.9, 0.5, 0.3, 1.0f);
	vec4 material_diffuse(0.9, 0.5, 0.3, 1.0f);
	vec4 material_specular(0.8, 0.8, 0.8, 1.0f);

	float material_shininess = 50.0;
	vec4 ambient_product = lightintensity*material_ambient;
	vec4 diffuse_product = lightintensity*material_diffuse;
	vec4 specular_product = lightintensity*material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	
	glUniform1i(glGetUniformLocation(program, "Tex"), 0);
	planeTexID = loadTexture((char *)"con1.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTexID);
	

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

    glUseProgram(program);
	//mat4 _scale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0, 7.0, 7.0));
	
	model = mat4(1.0f);
	
	model_view = view*model;
	mat3 normalmatrix;
	projection = glm::perspective(radians(60.0f), aspect, 0.3f, 100.0f); 
	mat4 mvp = projection*model_view;

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (GLfloat*)&model[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (GLfloat*)&view[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	
	//updateTime();

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//insert your code here
	/*updateBunnyVertex(tempVertices, vertices, normals, numverts);
	updateBunnyNormal(tempNormals, tempVertices, indices);
	GLuint offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, tempVertices.size() * sizeof(glm::vec3), &tempVertices[0]);
	offset += vertices.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, tempNormals.size() * sizeof(glm::vec3), &tempNormals[0]);*/

	
	

	if (showLines)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindTexture(GL_TEXTURE_2D, planeTexID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	
	glBindVertexArray(0);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}

/****************************************************************************************/
void updateTime() {
	timeUpdate += 0.1f;
}
/**************************************************************************************/
void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'f':case 'F':
		// Toggles wireframe if presses 'f'
			showLines = !showLines;
		break;
		
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
     }
	glutPostRedisplay();
}

/**********************************************************************************************************************************/
void Rotate(int n)  //the "glutTimerFunc"
{

	switch (n) {
	case 1:
		//updateTime();
		glutPostRedisplay();
		glutTimerFunc(100, Rotate, 1);
		break;
	}

}

int main(int argc, char** argv){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	
	glutCreateWindow("Skinny Chubby Bunny");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}
	ilInit();
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Rotate, 1);
	glutMainLoop();
	return 0;
}

void updateBunnyNormal(vector<vec3> &vNormals, vector<vec3> vertices, vector<int> indices)
{
	// Complete this function
	// Complete this function
	normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < indices.size(); i += 3) { // each three indices represent a triangle/ face of cone

		vec3 p1 = vertices[indices[i]];
		vec3 p2 = vertices[indices[i + 1]];
		vec3 p3 = vertices[indices[i + 2]];

		vec3 normal = normalize(cross(vec3(p2 - p1), vec3(p3 - p1)));//per triangle normal
																	 // keeps on adding normals for the same index for different triangles
		normals[indices[i]] += normal;
		normals[indices[i + 1]] += normal;
		normals[indices[i + 2]] += normal;
	}
	// 
	for (int i = 0; i < normals.size(); i++) {
		normals[i] = normalize(normals[i]);
	}

}

void updateBunnyVertex(vector<vec3> &ptr, vector<vec3> verts, vector<vec3> &norms, int count){
	
	// Complete this function
	float offset =  0.06 * sin(TWOPI*1 + timeUpdate) + 0.05;
	for (int i = 0; i < verts.size();i++) {
		ptr[i].x = verts[i].x + norms[i].x* offset;
		ptr[i].y = verts[i].y + norms[i].y* offset;
		ptr[i].z = verts[i].z + norms[i].z* offset;
	}

}


