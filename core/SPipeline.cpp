#include "SPipeline.h"

SPipeline::SPipeline()
	: rendererMode_(RendererMode::Triangle) {

}

SPipeline::~SPipeline() {

}

SPipeline& SPipeline::getInstance() {
	static SPipeline instance;
	return instance;
}

//void SPipeline::drawMesh(Mesh& mesh) {
	//std::vector<std::vector<Vertex>> triangleList;
	/*size_t vertSize = mesh.indices.size();
	for (size_t i = 0; i < vertSize; i += 3) {
		assert(i + 1 < vertSize && i + 2 < vertSize);
		std::vector<Vertex> tri = { mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]] };
		triangleList.push_back(tri);
	}

	if (rendererMode_ == RendererMode::Line) {
		FragVert fragInput[3];
		Vec4f screenP[3];
		Matrix viewport = frameBuffer_->getViewport();
		for (size_t idx = 0; idx < triangleList.size(); idx++) {
			auto triangle = triangleList[idx];
			for (int i = 0; i < 3; i++) {
				screenP[i] = viewport * shader_->vertexShader(triangle[i], fragInput[i]);
			}
			for (int i = 0; i < 3; i++) {
				if (i == 2) {
					drawLine(screenP[2], screenP[0]);
				}
				else {
					drawLine(screenP[i], screenP[i + 1]);
				}
			}
		}
	}*/
//}

Vec3f getBarycentricCoordinates(Vec4f* v, float x, float y) {
	float gamma = ((v[0].y - v[1].y) * x + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y)
		/ ((v[0].y - v[1].y) * v[2].x + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	float beta = ((v[0].y - v[2].y) * x + (v[2].x - v[0].x) * y + v[0].x * v[2].y - v[2].x * v[0].y)
		/ ((v[0].y - v[2].y) * v[1].x + (v[2].x - v[0].x) * v[1].y + v[0].x * v[2].y - v[2].x * v[0].y);
	return Vec3f(1 - gamma - beta, beta, gamma);
}

bool is_back_facing(Vec4f* v) {
	Vec4f a = v[0];
	Vec4f b = v[1];
	Vec4f c = v[2];
	float signed_area = a.x * b.y - a.y * b.x +
		b.x * c.y - b.y * c.x +
		c.x * a.y - c.y * a.x;
	return signed_area <= 0;
}

void SPipeline::drawObj(const SObj_Loader* obj) {
	for (auto mesh : obj->LoadedMeshes) {
		drawMesh(mesh);
	}
}

void SPipeline::drawMesh(Mesh& mesh) {
	Matrix viewport = frameBuffer_->getViewport();
	if (rendererMode_ == RendererMode::Line) {
		Vec4f vert[3];
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			for (int j = 0; j < 3; j++) {
				vert[j] = shader_->vertexShader(mesh.Vertices[mesh.Indices[i+j]]);
				vert[j].homogeneousAlignment();
				vert[j] = viewport * vert[j];
			}

			if (is_back_facing(vert)) continue;

			for (int j = 0; j < 3; j++) {
				if (j == 2) {
					drawLine(vert[2], vert[0]);
				}
				else {
					drawLine(vert[j], vert[j + 1]);
				}
			}
		}
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		Vertex tri[3];
		Vec4f clip_coord[3];
		Vec4f screen_coord[3];
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			if (i == 24) {
				int ll = 0;
			}
			tri[0] = mesh.Vertices[mesh.Indices[i]];
			tri[1] = mesh.Vertices[mesh.Indices[i + 1]];
			tri[2] = mesh.Vertices[mesh.Indices[i + 2]];

			for (int j = 0; j < 3; j++) {
				clip_coord[j] = shader_->vertexShader(tri[j]);
			}

			for (int j = 0; j < 3; j++) {
				screen_coord[j] = clip_coord[j] / clip_coord[j].w;
				screen_coord[j] = viewport * screen_coord[j];
			}

			if (is_back_facing(screen_coord)) continue;

			int minX = std::min(std::min(screen_coord[0].x, screen_coord[1].x), screen_coord[2].x);
			int minY = std::min(std::min(screen_coord[0].y, screen_coord[1].y), screen_coord[2].y);
			int maxX = std::max(std::max(screen_coord[0].x, screen_coord[1].x), screen_coord[2].x);
			int maxY = std::max(std::max(screen_coord[0].y, screen_coord[1].y), screen_coord[2].y);

			if (minX >= frameBuffer_->getWidth() || maxX < 0 || minY >= frameBuffer_->getHeight() || maxY < 0) return;
			minX = std::max(0, minX);
			maxX = std::min(frameBuffer_->getWidth(), maxX);
			minY = std::max(0, minY);
			maxY = std::min(frameBuffer_->getHeight(), maxY);
			for (int x = minX; x < maxX; x++) {
				for (int y = minY; y < maxY; y++) {
					printf("%d, %d\n", x, y);
					if (x == 418 && y == 267) {
						int qq = 9;
					}
					Vec3f bc = getBarycentricCoordinates(screen_coord, x + 0.5, y + 0.5);
					if (bc.x > -1e-5f && bc.y > -1e-5f && bc.z > -1e-5f) {
						bc.x = bc.x / -clip_coord[0].w;
						bc.y = bc.y / -clip_coord[1].w;
						bc.z = bc.z / -clip_coord[2].w;

						float z = 1 / (bc.x + bc.y + bc.z);
						if (frameBuffer_->depthCheck(x, y, z)) {
							bc = bc * z;
							SColor c = shader_->fragmentShader(tri, bc, mesh.MeshMaterial);
							frameBuffer_->writeBuffer(x, y, c);
						}
					}
				}
			}
		}
	}
}

void SPipeline::drawLine(const Vec4f& v1, const Vec4f& v2) {
	int x1 = v1.x;
	int y1 = v1.y;
	int x2 = v2.x;
	int y2 = v2.y;
	bool changeXY = false;
	if (abs(x2 - x1) < abs(y2 - y1)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
		changeXY = true;
	}
	if (x2 < x1) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	int dx = x2 - x1;
	int dy = y2 - y1;
	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = y1;
	for (int x = x1; x <= x2; x++) {
		if (changeXY) {
			if (y >= 0 && y < frameBuffer_->getWidth() && x >= 0 && x < frameBuffer_->getHeight())
				frameBuffer_->writeBuffer(y, x, SColor(255));
		}
		else {
			if (x >= 0 && x < frameBuffer_->getWidth() && y >= 0 && y < frameBuffer_->getHeight())
				frameBuffer_->writeBuffer(x, y, SColor(255));
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y2 > y1 ? 1 : -1);
			error2 -= 2 * dx;
		}
	}
}