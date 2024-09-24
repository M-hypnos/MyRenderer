#pragma once
#include "Object.h"
#include <vector>
#include "../core/Obj_Loader.h"
#include "../core/Pipeline.h"
class QuatObject :
    public Object
{
public:
	void addQuat(std::vector<Vertex> verts, const char* filename) {
		verts_ = verts;
		material_.map_Kd = filename;
		int texIdx = TexturePool::getInstance().isTextureLoaded(filename);
		if (texIdx < 0) {
			Texture texture;
			if (texture.loadFromImage(filename, "")) {
				texIdx = TexturePool::getInstance().pushToCache(texture);
			}
		}
		material_.Kd_texIdx = texIdx;
		material_.Ks = Vec3f(0.5, 0.5, 0.5);
	}

	void onRender() override {
		Pipeline::getInstance().drawVerts(verts_, material_);
	}
private:
	std::vector<Vertex> verts_;
	Material material_;
};

