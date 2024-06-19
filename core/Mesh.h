#ifndef __MESH_H__
#define __MESH_H__
#include "../utils/Geometry.h"

struct Vertex {
	Vec3f pos;
	Vec3f normal;
	Vec2f texcoords;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<size_t> indices;
	std::vector<int> textureIdx;

	Mesh(std::vector<Vertex> vertices, std::vector<size_t> indices, std::vector<int> textureIdx);
	
private:

};

#endif