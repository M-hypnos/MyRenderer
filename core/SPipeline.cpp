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

Vec3f SPipeline::getBarycentricCoordinates(Vec4f& v1, Vec4f& v2, Vec4f& v3, float x, float y) {
	float gamma = ((v1.y - v2.y) * x + (v2.x - v1.x) * y + v1.x * v2.y - v2.x * v1.y)
		/ ((v1.y - v2.y) * v3.x + (v2.x - v1.x) * v3.y + v1.x * v2.y - v2.x * v1.y);
	float beta = ((v1.y - v3.y) * x + (v3.x - v1.x) * y + v1.x * v3.y - v3.x * v1.y)
		/ ((v1.y - v3.y) * v2.x + (v3.x - v1.x) * v2.y + v1.x * v3.y - v3.x * v1.y);
	return Vec3f(1 - gamma - beta, beta, gamma);
}

bool SPipeline::is_back_facing(Vec4f& v1, Vec4f& v2, Vec4f& v3) {
	if (isBackCulling_) {
		float signed_area = v1.x * v2.y - v1.y * v2.x +
			v2.x * v3.y - v2.y * v3.x +
			v3.x * v1.y - v3.y * v1.x;
		return signed_area <= 0;
	}
	return false;
}

void SPipeline::drawObj(const SObj_Loader* obj) {
	for (auto mesh : obj->LoadedMeshes) {
		drawMesh(mesh);
	}
}

void SPipeline::clip_triangleEx(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut, Plane plane) {
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

void SPipeline::clip_triangle(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut) {
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

void SPipeline::drawVerts(std::vector<Vertex>& verts, const Material& material) {
	std::vector<shaderVert> vertIn;
	std::vector<shaderVert> vertTemp;
	if (rendererMode_ == RendererMode::Line) {
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();
			bool isNeedClip = false;
			for (int j = 0; j < 3; j++) {
				shaderVert vert;
				shader_->vertexShader(verts[i + j], vert);
				vertIn.emplace_back(vert);

				if (!isClipTriangle_ && isOutsidePlane(vert.clip_coords))
				{
					isNeedClip = true;
					break;
				}
			}

			if (isNeedClip) continue;

			clip_triangle(vertIn, vertTemp);

			if (vertIn.size() < 3) continue;
			if (vertIn.size() > 3) {
				int vv = 0;
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
				if (is_back_facing(v0, v1, v2)) continue;
				drawLine(v0, v1);
				drawLine(v1, v2);
				drawLine(v2, v0);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		Vertex tri[3];
		for (int i = 0; i < verts.size(); i += 3) {
			vertIn.clear();

			tri[0] = verts[i];
			tri[1] = verts[i + 1];
			tri[2] = verts[i + 2];

			bool isNeedClip = false;
			for (int j = 0; j < 3; j++) {
				shaderVert vert;
				shader_->vertexShader(tri[j], vert);
				vert.normal = tri[j].normal;
				vert.texcoords = tri[j].texcoords;
				vertIn.emplace_back(vert);
				if (!isClipTriangle_ && isOutsidePlane(vert.clip_coords))
				{
					isNeedClip = true;
					break;
				}
			}
			if (isNeedClip) continue;

			clip_triangle(vertIn, vertTemp);
			int count = vertIn.size() - 1;
			for (int vi = 1; vi < count; vi++) {
				drawTriangle(vertIn[0], vertIn[vi], vertIn[vi + 1], material);
			}
		}
	}
}

void SPipeline::drawMesh(Mesh& mesh) {
	std::vector<shaderVert> vertIn;
	std::vector<shaderVert> vertTemp;
	if (rendererMode_ == RendererMode::Line) {
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();
			bool isNeedClip = false;
			for (int j = 0; j < 3; j++) {
				shaderVert vert;
				shader_->vertexShader(mesh.Vertices[mesh.Indices[i + j]], vert);
				vertIn.emplace_back(vert);

				if (!isClipTriangle_ && isOutsidePlane(vert.clip_coords))
				{
					isNeedClip = true;
					break;
				}
			}

			if (isNeedClip) continue;

			clip_triangle(vertIn, vertTemp);

			if (vertIn.size() < 3) continue;
			if (vertIn.size() > 3) {
				int vv = 0;
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
				if (is_back_facing(v0, v1, v2)) continue;
				drawLine(v0, v1);
				drawLine(v1, v2);
				drawLine(v2, v0);
			}
		}
	}
	else if (rendererMode_ == RendererMode::Triangle) {
		Vertex tri[3];
		for (int i = 0; i < mesh.Indices.size(); i += 3) {
			vertIn.clear();

			tri[0] = mesh.Vertices[mesh.Indices[i]];
			tri[1] = mesh.Vertices[mesh.Indices[i + 1]];
			tri[2] = mesh.Vertices[mesh.Indices[i + 2]];


			bool isNeedClip = false;
			for (int j = 0; j < 3; j++) {
				shaderVert vert;
				shader_->vertexShader(tri[j], vert);
				vert.normal = tri[j].normal;
				vert.texcoords = tri[j].texcoords;
				vertIn.emplace_back(vert);

				if (!isClipTriangle_ && isOutsidePlane(vert.clip_coords))
				{
					isNeedClip = true;
					break;
				}
			}
			if (isNeedClip) continue;

			clip_triangle(vertIn, vertTemp);
			int count = vertIn.size() - 1;
			for (int vi = 1; vi < count; vi++) {
				drawTriangle(vertIn[0], vertIn[vi], vertIn[vi + 1], mesh.MeshMaterial);
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

void SPipeline::drawTriangle(shaderVert& v1, shaderVert& v2, shaderVert& v3, const Material& material) {
	v1.screen_coords = v1.clip_coords / v1.clip_coords.w;
	v1.screen_coords = viewport_ * v1.screen_coords;
	v2.screen_coords = v2.clip_coords / v2.clip_coords.w;
	v2.screen_coords = viewport_ * v2.screen_coords;
	v3.screen_coords = v3.clip_coords / v3.clip_coords.w;
	v3.screen_coords = viewport_ * v3.screen_coords;

	if (is_back_facing(v1.screen_coords, v2.screen_coords, v3.screen_coords)) return;

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
					SColor c = shader_->fragmentShader(v1, v2, v3, bc, material);
					frameBuffer_->writeBuffer(x, y, c);
				}
			}
		}
	}
}