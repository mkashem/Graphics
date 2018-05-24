#include <iostream>    // std::cout  
#include <fstream>
#include <sstream>
#include <string>
#include "objloader.h"

using namespace glm;
void objloader:: computeNormals(std::vector<glm::vec3> const &vertices, std::vector<int> const &indices, std::vector<glm::vec3> &normals)
{
		
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


objloader::objloader() :
mVertices(0),
mNormals(0),
vIndices(0)
{
	std::cout << "Called OBJFileReader constructor" << std::endl;
}

objloader::~objloader()
{
	std::cout << "Called OBJFileReader destructor" << std::endl;
}

bool objloader::load(const char *filename)
{
	// Open OBJ file
	std::ifstream OBJFile(filename);
	if (!OBJFile.is_open()) {
		std::cerr << "Could not open " << filename << std::endl;
		return false;
	}
	
	// Extract vertices and indices
	std::string line;
	glm::vec3 vertex;
	glm::vec2 uv;
	while (!OBJFile.eof()) {
		std::getline(OBJFile, line);
		if ((line.find('#') == -1) && (line.find('m') == -1)){
			if (line.find('v') != -1) {

				if ((line.find('t') == -1) && (line.find('n') == -1)){
					std::istringstream vertexLine(line.substr(2));
					vertexLine >> vertex.x;
					vertexLine >> vertex.y;
					vertexLine >> vertex.z;
				    mVertices.push_back(vertex);
				}
			}

			else if (line.find("f ") != -1) {
				std::istringstream faceLine(line);
				std::string val1;
				faceLine >> val1;
				int val;
				for (int n = 0; n < 3; n++){
					faceLine >> val;
					
					vIndices.push_back(val- 1);
					

				}
			}
	    }
	}

	// Close OBJ file
	OBJFile.close();

	
	// Compute normals
	computeNormals(mVertices, vIndices, mNormals);
	
	
	return true;
}

std::vector<glm::vec3> const &objloader::getVertices() const
{
	return mVertices;
}

std::vector<glm::vec3> const &objloader::getNormals() const
{
	return mNormals;
}


std::vector<int> const &objloader::getVertexIndices() const
{
	return vIndices;
}



