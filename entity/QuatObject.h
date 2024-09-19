#pragma once
#include "Object.h"
#include <vector>
#include "../core/SObj_Loader.h"
#include "../core/SPipeline.h"
class QuatObject :
    public Object
{
public:
	void addQuat(std::vector<Vertex> verts, const char* filename) {
		verts_ = verts;
		material_.map_Kd = filename;
		int texIdx = STexturePool::getInstance().isTextureLoaded(filename);
		if (texIdx < 0) {
			STexture texture;
			if (texture.loadFromImage(filename, "")) {
				texIdx = STexturePool::getInstance().pushToCache(texture);
			}
		}
		material_.Kd_texIdx = texIdx;
		material_.Ks = Vec3f(0.5, 0.5, 0.5);
	}

	void onRender() override {
		SPipeline::getInstance().drawVerts(verts_, material_);
	}
private:
	std::vector<Vertex> verts_;
	Material material_;
};

