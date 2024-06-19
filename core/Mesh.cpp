#include "Mesh.h"

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<size_t> indices, vector<int> textureIdx) {
	this->vertices = vertices;
	this->indices = indices;
	this->textureIdx = textureIdx;
}