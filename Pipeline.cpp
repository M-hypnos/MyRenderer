#include "Pipeline.h"

Pipeline::Pipeline()
	: rendererMode_(RendererMode::Triangle) {

}

Pipeline::~Pipeline() {

}

Pipeline& Pipeline::getInstance() {
	static Pipeline instance;
	return instance;
}

Vec3f Pipeline::getBarycentricCoordinates(Vec4f& v1, Vec4f& v2, Vec4f& v3, float x, float y) {
	float gamma = ((v1.y - v2.y) * x + (v2.x - v1.x) * y + v1.x * v2.y - v2.x * v1.y)
		/ ((v1.y - v2.y) * v3.x + (v2.x - v1.x) * v3.y + v1.x * v2.y - v2.x * v1.y);
	float beta = ((v1.y - v3.y) * x + (v3.x - v1.x) * y + v1.x * v3.y - v3.x * v1.y)
		/ ((v1.y - v3.y) * v2.x + (v3.x - v1.x) * v2.y + v1.x * v3.y - v3.x * v1.y);
	return Vec3f(1 - gamma - beta, beta, gamma);
}

bool Pipeline::is_back_facing(Vec4f& v11, Vec4f& v22, Vec4f& v33) {
	Vec4f v1 = v11 / v11.w;
	Vec4f v2 = v22 / v22.w;
	Vec4f v3 = v33 / v33.w;

	if (isBackCulling_) {
		float signed_area = v1.x * v2.y - v1.y * v2.x +
			v2.x * v3.y - v2.y * v3.x +
			v3.x * v1.y - v3.y * v1.x;
		return signed_area <= 0;
	}
	return false;
}

void Pipeline::drawObj(const Obj_Loader* obj) {
	for (auto mesh : obj->LoadedMeshes) {
		drawMesh(mesh);
	}
}

void Pipeline::clip_triangleEx(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut, Plane plane) {
	vertOut.clear();
	if (plane == Plane::XLeft) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.x >= -v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.x < -v2.clip_coords.w) {
					//相似三角形求插值
					float t = (v1.clip_coords.w + v1.clip_coords.x) / ((v1.clip_coords.w + v1.clip_coords.x) - (v2.clip_coords.w + v2.clip_coords.x));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if(v2.clip_coords.x >= -v2.clip_coords.w){
				float t = (v2.clip_coords.w + v2.clip_coords.x) / ((v2.clip_coords.w + v2.clip_coords.x) - (v1.clip_coords.w + v1.clip_coords.x));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
	else if (plane == Plane::XRight) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.x <= v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.x > v2.clip_coords.w) {
					float t = (v1.clip_coords.w - v1.clip_coords.x) / ((v1.clip_coords.w - v1.clip_coords.x) - (v2.clip_coords.w - v2.clip_coords.x));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if (v2.clip_coords.x <= v2.clip_coords.w) {
				float t = (v2.clip_coords.w - v2.clip_coords.x) / ((v2.clip_coords.w - v2.clip_coords.x) - (v1.clip_coords.w - v1.clip_coords.x));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
	else if (plane == Plane::YTOP) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.y <= v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.y > v2.clip_coords.w) {
					float t = (v1.clip_coords.w - v1.clip_coords.y) / ((v1.clip_coords.w - v1.clip_coords.y) - (v2.clip_coords.w - v2.clip_coords.y));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if (v2.clip_coords.y <= v2.clip_coords.w) {
				float t = (v2.clip_coords.w - v2.clip_coords.y) / ((v2.clip_coords.w - v2.clip_coords.y) - (v1.clip_coords.w - v1.clip_coords.y));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
	else if (plane == Plane::YBOTTOM) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.y >= -v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.y < -v2.clip_coords.w) {
					float t = (v1.clip_coords.w + v1.clip_coords.y) / ((v1.clip_coords.w + v1.clip_coords.y) - (v2.clip_coords.w + v2.clip_coords.y));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if (v2.clip_coords.y >= -v2.clip_coords.w) {
				float t = (v2.clip_coords.w + v2.clip_coords.y) / ((v2.clip_coords.w + v2.clip_coords.y) - (v1.clip_coords.w + v1.clip_coords.y));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
	else if (plane == Plane::ZNEAR) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.z <= v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.z > v2.clip_coords.w) {
					float t = (v1.clip_coords.w - v1.clip_coords.z) / ((v1.clip_coords.w - v1.clip_coords.z) - (v2.clip_coords.w - v2.clip_coords.z));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if (v2.clip_coords.z <= v2.clip_coords.w) {
				float t = (v2.clip_coords.w - v2.clip_coords.z) / ((v2.clip_coords.w - v2.clip_coords.z) - (v1.clip_coords.w - v1.clip_coords.z));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
	else if (plane == Plane::ZFAR) {
		for (int i = 0; i < vertIn.size(); i++) {
			shaderVert v1 = vertIn[i];
			shaderVert v2;
			if (i == vertIn.size() - 1)
				v2 = vertIn[0];
			else
				v2 = vertIn[i + 1];
			if (v1.clip_coords.z >= -v1.clip_coords.w) {
				vertOut.emplace_back(vertIn[i]);
				if (v2.clip_coords.z < -v2.clip_coords.w) {
					float t = (v1.clip_coords.w + v1.clip_coords.z) / ((v1.clip_coords.w + v1.clip_coords.z) - (v2.clip_coords.w + v2.clip_coords.z));
					vertOut.emplace_back(shaderVert::lerp(v1, v2, t));
				}
			}
			else if (v2.clip_coords.z >= -v2.clip_coords.w) {
				float t = (v2.clip_coords.w + v2.clip_coords.z) / ((v2.clip_coords.w + v2.clip_coords.z) - (v1.clip_coords.w + v1.clip_coords.z));
				vertOut.emplace_back(shaderVert::lerp(v2, v1, t));
			}
		}
	}
}

void Pipeline::clip_triangle(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut) {
	vertOut.clear();
	for (auto vert : vertIn) {
		if (isOutsidePlane(vert.clip_coords)) {
			clip_triangleEx(vertIn, vertOut, Plane::XLeft);
			clip_triangleEx(vertOut, vertIn, Plane::XRight);
			clip_triangleEx(vertIn, vertOut, Plane::YBOTTOM);
			clip_triangleEx(vertOut, vertIn, Plane::YTOP);
			clip_triangleEx(vertIn, vertOut, Plane::ZNEAR);
			clip_triangleEx(vertOut, vertIn, Plane::ZFAR);
			return;
		}
	}
}

void Pipeline::drawVerts(std::vector<Vertex>& verts, const Material& material) {
	std::vector<shaderVert> vertIn;
	std::vector<shaderVert> vertTemp;
	if (rendererMode_ == RendererMode::Point) {
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(verts[i], verts[i+1], verts[i+2], vertIn);
			for (auto vert : vertIn) {
				if(isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			for (auto vert : vertIn) {
				Vec4f v = vert.clip_coords;
				v.homogeneousAlignment();
				v = viewport_ * v;
				if (v.x >= 0 && v.x < frameBuffer_->getWidth() && v.y >= 0 && v.y < frameBuffer_->getHeight())
					frameBuffer_->writeBuffer(v.x, v.y, Color(255));
			}
		}
	}
	else if (rendererMode_ == RendererMode::Line) {
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(verts[i], verts[i + 1], verts[i + 2], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			Vec4f v0 = vertIn[0].clip_coords;
			v0.homogeneousAlignment();
			v0 = viewport_ * v0;
			Vec4f v1;
			Vec4f v2 = vertIn[1].clip_coords;
			v2.homogeneousAlignment();
			v2 = viewport_ * v2;
			for (int vi = 1; vi < vertIn.size() - 1; vi++) {
				v1 = v2;
				v2 = vertIn[vi + 1].clip_coords;
				v2.homogeneousAlignment();
				v2 = viewport_ * v2;
				drawLine(v0, v1);
				drawLine(v1, v2);
				drawLine(v2, v0);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(verts[i], verts[i + 1], verts[i + 2], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			int count = vertIn.size() - 1;
			for (int vi = 1; vi < count; vi++) {
				drawTriangle(vertIn[0], vertIn[vi], vertIn[vi + 1], material);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Normal) {
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(verts[i], verts[i + 1], verts[i + 2], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			{
				Vec4f v0 = vertIn[0].clip_coords;
				v0.homogeneousAlignment();
				v0 = viewport_ * v0;
				Vec4f v1;
				Vec4f v2 = vertIn[1].clip_coords;
				v2.homogeneousAlignment();
				v2 = viewport_ * v2;
				for (int vi = 1; vi < vertIn.size() - 1; vi++) {
					v1 = v2;
					v2 = vertIn[vi + 1].clip_coords;
					v2.homogeneousAlignment();
					v2 = viewport_ * v2;
					drawLine(v0, v1, Color(255, 0, 0));
					drawLine(v1, v2, Color(255, 0, 0));
					drawLine(v2, v0, Color(255, 0, 0));
				}
			}

			for (auto vert : vertIn) {
				Vec4f v = vert.clip_coords;
				v.homogeneousAlignment();
				v = viewport_ * v;
				Vec3f p = vert.world_coords + vert.normal * 0.5;
				Vec4f p1 = shader_->projectMat4 * shader_->viewMat4 * embed<4>(p);
				p1.homogeneousAlignment();
				p1 = viewport_ * p1;
				drawLine(v, p1);
			}
		}
	}
}

void Pipeline::drawMesh(Mesh& mesh) {
	std::vector<shaderVert> vertIn;
	std::vector<shaderVert> vertTemp;
	if (rendererMode_ == RendererMode::Point) {
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(mesh.Vertices[mesh.Indices[i]], mesh.Vertices[mesh.Indices[i + 1]], mesh.Vertices[mesh.Indices[i + 2]], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			for (auto vert : vertIn) {
				Vec4f v = vert.clip_coords;
				v.homogeneousAlignment();
				v = viewport_ * v;
				if (v.x >= 0 && v.x < frameBuffer_->getWidth() && v.y >= 0 && v.y < frameBuffer_->getHeight())
					frameBuffer_->writeBuffer(v.x, v.y, Color(255));
			}
		}
	}
	else if (rendererMode_ == RendererMode::Line) {
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(mesh.Vertices[mesh.Indices[i]], mesh.Vertices[mesh.Indices[i + 1]], mesh.Vertices[mesh.Indices[i + 2]], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			Vec4f v0 = vertIn[0].clip_coords;
			v0.homogeneousAlignment();
			v0 = viewport_ * v0;
			Vec4f v1;
			Vec4f v2 = vertIn[1].clip_coords;
			v2.homogeneousAlignment();
			v2 = viewport_ * v2;
			for (int vi = 1; vi < vertIn.size() - 1; vi++) {
				v1 = v2;
				v2 = vertIn[vi+1].clip_coords;
				v2.homogeneousAlignment();
				v2 = viewport_ * v2;
				drawLine(v0, v1);
				drawLine(v1, v2);
				drawLine(v2, v0);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(mesh.Vertices[mesh.Indices[i]], mesh.Vertices[mesh.Indices[i + 1]], mesh.Vertices[mesh.Indices[i + 2]], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)){
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			int count = vertIn.size() - 1;
			for (int vi = 1; vi < count; vi++) {
				drawTriangle(vertIn[0], vertIn[vi], vertIn[vi + 1], mesh.MeshMaterial);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Normal) {
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();
			int outsideCount = 0;
			shader_->vertexShader(mesh.Vertices[mesh.Indices[i]], mesh.Vertices[mesh.Indices[i + 1]], mesh.Vertices[mesh.Indices[i + 2]], vertIn);
			for (auto vert : vertIn) {
				if (isOutsidePlane(vert.clip_coords)) {
					outsideCount++;
					if (!isClipTriangle_)
						break;
				}
			}

			if (!isClipTriangle_ && outsideCount > 0) continue;
			if (isClipTriangle_ && outsideCount == 3) continue;

			if (is_back_facing(vertIn[0].clip_coords, vertIn[1].clip_coords, vertIn[2].clip_coords)) continue;

			if (outsideCount > 0 && isClipTriangle_) {
				clip_triangle(vertIn, vertTemp);
			}

			{
				Vec4f v0 = vertIn[0].clip_coords;
				v0.homogeneousAlignment();
				v0 = viewport_ * v0;
				Vec4f v1;
				Vec4f v2 = vertIn[1].clip_coords;
				v2.homogeneousAlignment();
				v2 = viewport_ * v2;
				for (int vi = 1; vi < vertIn.size() - 1; vi++) {
					v1 = v2;
					v2 = vertIn[vi + 1].clip_coords;
					v2.homogeneousAlignment();
					v2 = viewport_ * v2;
					drawLine(v0, v1, Color(255, 0, 0));
					drawLine(v1, v2, Color(255, 0, 0));
					drawLine(v2, v0, Color(255, 0, 0));
				}
			}

			for (auto vert : vertIn) {
				Vec4f v = vert.clip_coords;
				v.homogeneousAlignment();
				v = viewport_ * v;
				Vec3f p = vert.world_coords + vert.normal * 0.5;
				Vec4f p1 = shader_->projectMat4 * shader_->viewMat4 * embed<4>(p);
				p1.homogeneousAlignment();
				p1 = viewport_ * p1;
				drawLine(v, p1);
			}
		}
	}
}

void Pipeline::drawLine(const Vec4f& v1, const Vec4f& v2, Color c) {
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
				frameBuffer_->writeBuffer(y, x, c);
		}
		else {
			if (x >= 0 && x < frameBuffer_->getWidth() && y >= 0 && y < frameBuffer_->getHeight())
				frameBuffer_->writeBuffer(x, y, c);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y2 > y1 ? 1 : -1);
			error2 -= 2 * dx;
		}
	}
}

void Pipeline::drawTriangle(shaderVert& v1, shaderVert& v2, shaderVert& v3, const Material& material) {
	v1.screen_coords = v1.clip_coords / v1.clip_coords.w;
	v1.screen_coords = viewport_ * v1.screen_coords;
	v2.screen_coords = v2.clip_coords / v2.clip_coords.w;
	v2.screen_coords = viewport_ * v2.screen_coords;
	v3.screen_coords = v3.clip_coords / v3.clip_coords.w;
	v3.screen_coords = viewport_ * v3.screen_coords;

	float minX = std::min(std::min(v1.screen_coords.x, v2.screen_coords.x), v3.screen_coords.x);
	float minY = std::min(std::min(v1.screen_coords.y, v2.screen_coords.y), v3.screen_coords.y);
	float maxX = std::max(std::max(v1.screen_coords.x, v2.screen_coords.x), v3.screen_coords.x);
	float maxY = std::max(std::max(v1.screen_coords.y, v2.screen_coords.y), v3.screen_coords.y);

	/*if (minX >= frameBuffer_->getWidth() || maxX < 0 || minY >= frameBuffer_->getHeight() || maxY < 0) return;
	minX = std::max(0.f, minX);
	maxX = std::min(frameBuffer_->getWidth() - 1.f, maxX);
	minY = std::max(0.f, minY);
	maxY = std::min(frameBuffer_->getHeight() -1.f, maxY);*/
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			Vec3f bc = getBarycentricCoordinates(v1.screen_coords, v2.screen_coords, v3.screen_coords, x + 0.5, y + 0.5);
			if (bc.x > -1e-5f && bc.y > -1e-5f && bc.z > -1e-5f) {
				bc.x = bc.x / -v1.clip_coords.w;
				bc.y = bc.y / -v2.clip_coords.w;
				bc.z = bc.z / -v3.clip_coords.w;

				float z = 1 / (bc.x + bc.y + bc.z);
				if (frameBuffer_->depthCheck(x, y, z)) {
					bc = bc * z;
					Color c = shader_->fragmentShader(v1, v2, v3, bc, material);
					frameBuffer_->writeBuffer(x, y, c);
				}
			}
		}
	}
}

void Pipeline::drawRect(int x, int y, int width, int height, Color c) {
	drawLine(Vec4f(x, y, 0, 0), Vec4f(x + width, y, 0, 0), c);
	drawLine(Vec4f(x + width, y, 0, 0), Vec4f(x + width, y + height, 0, 0), c);
	drawLine(Vec4f(x + width, y + height, 0, 0), Vec4f(x, y + height, 0, 0), c);
	drawLine(Vec4f(x , y, 0, 0), Vec4f(x, y + height, 0, 0), c);
}

void Pipeline::switchRendererMode() {
	if (rendererMode_ == RendererMode::Line) {
		rendererMode_ = RendererMode::Point;
	}
	else if (rendererMode_ == RendererMode::Point) {
		rendererMode_ = RendererMode::Triangle;
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		rendererMode_ = RendererMode::Line;
	}
	else if (rendererMode_ == RendererMode::Normal) {
		rendererMode_ = RendererMode::Line;
	}
}