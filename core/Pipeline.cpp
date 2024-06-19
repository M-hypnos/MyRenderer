#include "PipeLine.h"
#include <cassert>
#include <future>
#include "TextureCache.h"
using namespace std;

PipeLine& PipeLine::getInstance() {
	static PipeLine instance;
	return instance;
}

PipeLine::PipeLine()
: rendererMode_(RendererMode::Triangle){

}

PipeLine::~PipeLine() {

}

void PipeLine::bindShader(IShader* shader) {
	shader_ = shader;
}

void PipeLine::bindFrameBuffer(FrameBuffer* frameBuffer) {
	frameBuffer_ = frameBuffer;
	frameBuffer_->clear(TColor(0));
}

void PipeLine::setRendererMode(RendererMode rendererMode) {
	rendererMode_ = rendererMode;
}

void PipeLine::drawMesh(Mesh& mesh) {
	vector<array<Vertex, 3>> triangleList;
	int vertSize = mesh.indices.size();
	for (int i = 0; i < vertSize; i += 3) {
		assert(i + 1 < vertSize && i + 2 < vertSize);
		triangleList.push_back({ mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]] });
	}
	thread th[threadNum];
	size_t idx = triangleList.size() / threadNum;
	if (rendererMode_ == RendererMode::Point) {
		auto drawFunc = [&](int i, size_t size, vector<array<Vertex, 3>>& triangleList) {
			size_t end = (i + 1) * size;
			size_t start = i * size;
			if (i == threadNum) end = max(end, triangleList.size());
			for (size_t idx = start; idx < end; idx++) {
				drawPointMode(triangleList[idx], mesh.textureIdx, i);
			}
		};
		for (int i = 0; i < threadNum; i++) {
			th[i] = thread(drawFunc, i, idx, ref(triangleList));
			//drawFunc(i, idx, triangleList);
		}
		for (int i = 0; i < threadNum; i++) {
			th[i].join();
		}
	}
	else if (rendererMode_ == RendererMode::Line) {
		auto drawFunc = [&](int i, size_t size, vector<array<Vertex, 3>>& triangleList) {
			size_t end = (i + 1) * size;
			size_t start = i * size;
			if (i == threadNum) end = max(end, triangleList.size());
			for (size_t idx = start; idx < end; idx++) {
				drawLineMode(triangleList[idx], mesh.textureIdx, i);
			}
		};
		for (int i = 0; i < threadNum; i++) {
			th[i] = thread(drawFunc, i, idx, ref(triangleList));
		}
		for (int i = 0; i < threadNum; i++) {
			th[i].join();
		}
	}
	else {
		auto drawFunc = [&](int i, size_t size, vector<array<Vertex, 3>>& triangleList) {
			size_t end = (i + 1) * size;
			size_t start = i * size;
			if (i == threadNum) end = max(end, triangleList.size());
			for (size_t idx = start; idx < end; idx++) {
				drawTriangleMode(triangleList[idx], mesh.textureIdx, i);
			}
		};
		for (int i = 0; i < threadNum; i++) {
			th[i] = thread(drawFunc, i, idx, ref(triangleList));
		}
		for (int i = 0; i < threadNum; i++) {
			th[i].join();
		}
	}
}

void PipeLine::drawPointMode(array<Vertex, 3> triangle, vector<int> textureIdx, int idx) {
	Matrix viewport = frameBuffer_->getViewport();
	int offset = idx * 3;
	for (int i = 0; i < 3; i++) {
		shader_->vertexShader(triangle[i], shaderTri_[i + offset]);
		shader_->fragmentShader(shaderTri_[i + offset]);
		shaderTri_[i + offset].screenPos = viewport * shaderTri_[i + offset].ndcPos;
		if (shaderTri_[i + offset].screenPos.x >= 0 && shaderTri_[i + offset].screenPos.x < frameBuffer_->getWidth()
			&& shaderTri_[i + offset].screenPos.y >= 0 && shaderTri_[i + offset].screenPos.y < frameBuffer_->getHeight()) {
				frameBuffer_->setColor(shaderTri_[i + offset].screenPos.x, shaderTri_[i + offset].screenPos.y, shaderTri_[i + offset].color);
		}
	}
}

void PipeLine::drawLine(const ShaderVertex& v1, const ShaderVertex& v2) {
	int x1 = v1.screenPos.x;
	int y1 = v1.screenPos.y;
	int x2 = v2.screenPos.x;
	int y2 = v2.screenPos.y;
	bool changeXY = false;
	if (abs(x2 - x1) < abs(y2 - y1)) {
		swap(x1, y1);
		swap(x2, y2);
		changeXY = true;
	}
	if (x2 < x1) {
		swap(x1, x2);
		swap(y1, y2);
	}
	int dx = x2 - x1;
	int dy = y2 - y1;
	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = y1;
	for (int x = x1; x <= x2; x++) {
		if (changeXY) {
			if (y >= 0 && y < frameBuffer_->getWidth() && x >= 0 && x < frameBuffer_->getHeight())
				frameBuffer_->setColor(y, x, TColor(255));
		}
		else {
			if (x >= 0 && x < frameBuffer_->getWidth() && y >= 0 && y < frameBuffer_->getHeight())
				frameBuffer_->setColor(x, y, TColor(255));
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y2 > y1 ? 1 : -1);
			error2 -= 2 * dx;
		}
	}
}

void PipeLine::drawLineMode(array<Vertex, 3> triangle, vector<int> textureIdx, int idx) {
	Matrix viewport = frameBuffer_->getViewport();
	int offset = idx * 3;
	for (int i = 0; i < 3; i++) {
		shader_->vertexShader(triangle[i], shaderTri_[i + offset]);
		shader_->fragmentShader(shaderTri_[i + offset]);
		shaderTri_[i + offset].screenPos = viewport * shaderTri_[i + offset].ndcPos;
	}
	for (int i = 0; i < 3; i++) {
		if (i == 2) {
			drawLine(shaderTri_[2 + offset], shaderTri_[0 + offset]);
		}
		else {
			drawLine(shaderTri_[i + offset], shaderTri_[i+1 + offset]);
		}
	}
}

Vec3f getBarycentricCoordinates(Vec4f v1, Vec4f v2, Vec4f v3, float x, float y) {
	float gamma = ((v1.y - v2.y) * x + (v2.x - v1.x) * y + v1.x * v2.y - v2.x * v1.y)
		/ ((v1.y - v2.y) *v3.x + (v2.x - v1.x) *v3.y + v1.x * v2.y - v2.x * v1.y);
	float beta = ((v1.y -v3.y) * x + (v3.x - v1.x) * y + v1.x *v3.y -v3.x * v1.y)
		/ ((v1.y -v3.y) * v2.x + (v3.x - v1.x) * v2.y + v1.x *v3.y -v3.x * v1.y);
	return Vec3f(1 - gamma - beta, beta, gamma);
}

bool insideTriangle(Vec3f v1, Vec3f v2, Vec3f v3, float x, float y) {
	Vec3f v[3];
	v[0] = Vec3f(v1.x, v1.y, 1);
	v[1] = Vec3f(v2.x, v2.y, 1);
	v[2] = Vec3f(v3.x, v3.y, 1);
	
	Vec3f f0, f1, f2;
	f0 = cross(v[1], v[0]);
	f1 = cross(v[2], v[1]);
	f2 = cross(v[0], v[2]);
	Vec3f p(x, y, 1.);
	if ((p * f0 * (f0 * v[2]) > 0) && (p * f1 * (f1 * v[0]) > 0) && (p * f2 * (f2 * v[1]) > 0))
		return true;
	return false;
}

Vec3f interpolate(Vec3f bc, Vec3f v1,Vec3f v2, Vec3f v3) {
	return (v1 * bc.x + v2 * bc.y + v3 * bc.z);
}

Vec2f interpolate(Vec3f bc, Vec2f v1, Vec2f v2, Vec2f v3) {
	return (v1 * bc.x + v2 * bc.y + v3 * bc.z);
}

void PipeLine::drawTriangleMode(array<Vertex, 3> triangle, vector<int> textureIdx, int idx) {
	Matrix viewport = frameBuffer_->getViewport();
	int offset = idx * 3;

	int minX = frameBuffer_->getWidth();
	int maxX = 0;
	int minY = frameBuffer_->getHeight();
	int maxY = 0;

	for (int i = 0; i < 3; i++) {
		shader_->vertexShader(triangle[i], shaderTri_[i + offset]);
		Vec4f p = viewport * shaderTri_[i + offset].ndcPos;
		shaderTri_[i + offset].screenPos = p;

		minX = min(minX, int(p.x));
		maxX = max(maxX, int(p.x));
		minY = min(minY, int(p.y));
		maxY = max(maxY, int(p.y));
	}
	if (minX >= frameBuffer_->getWidth() || maxX < 0 || minY >= frameBuffer_->getHeight() || maxY < 0) return;
	minX = max(0, minX);
	maxX = min(frameBuffer_->getWidth(), maxX);
	minY = max(0, minY);
	maxY = min(frameBuffer_->getHeight(), maxY);
	Texture diffuTex;
	for (auto texId: textureIdx) {
		auto tex = TextureCache::getInstance().getTexture(texId);
		if (tex.type == "texture_diffuse") diffuTex = tex;
	}
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			Vec4f v1 = shaderTri_[offset].screenPos;
			Vec4f v2 = shaderTri_[offset+1].screenPos;
			Vec4f v3 = shaderTri_[offset+2].screenPos;
			Vec3f bc = getBarycentricCoordinates(v1, v2, v3, x+0.5, y+0.5);
			if (bc.x > -0.1 && bc.y > -0.1 && bc.z > -0.1) {
				bc.x = bc.x / shaderTri_[offset].worldPos.z;
				bc.y = bc.y / shaderTri_[offset + 1].worldPos.z;
				bc.z = bc.z / shaderTri_[offset + 2].worldPos.z;

				float z = 1 / (bc.x + bc.y + bc.z);

				Vec2f uv = interpolate(bc * z, triangle[0].texcoords, triangle[1].texcoords, triangle[2].texcoords);
				TColor c = diffuTex.getPixel(uv.x, uv.y);

				frameBuffer_->setColor(x, y, c, z);
			}
		}
	}

}