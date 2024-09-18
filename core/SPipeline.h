#pragma once
#include "SFrameBuffer.h"
#include "../GlobalConst.h"
//#include "SModel.h"
#include "SObj_Loader.h"
#include "../shader/IShader.h"

class SPipeline
{
public:
	static SPipeline& getInstance();
    void bindFrameBuffer(SFrameBuffer* frameBuffer) { 
        frameBuffer_ = frameBuffer; 
        viewport_ = frameBuffer_->getViewport();
    }
    void setRendererMode(RendererMode rendererMode) { rendererMode_ = rendererMode; }
    void bindShader(IShader* shader) { shader_ = shader; }
    //void drawMesh(Mesh& mesh);
    void drawObj(const SObj_Loader* obj);
    void switchBackCulling() { isBackCulling_ = !isBackCulling_; }
    void switchClipTriangle() { isClipTriangle_ = !isClipTriangle_; }

    SPipeline(const SPipeline&) = delete;
    SPipeline(SPipeline&&) = delete;
    SPipeline& operator=(const SPipeline&) = delete;
    SPipeline& operator=(SPipeline&&) = delete;
private:
    SPipeline();
    ~SPipeline();

    void drawMesh(Mesh& mesh);
    void drawLine(const Vec4f& v1, const Vec4f& v2);
    void drawTriangle(shaderVert& v1, shaderVert& v2, shaderVert& v3, const Material& material);
    Vec3f getBarycentricCoordinates(Vec4f& v1, Vec4f& v2, Vec4f& v3, float x, float y);
    bool is_back_facing(Vec4f& v1, Vec4f& v2, Vec4f& v3);
    void clip_triangle(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut);
    void clip_triangleEx(std::vector<shaderVert>& vertIn, std::vector<shaderVert>& vertOut, Plane plane);
    bool isOutsidePlane(const Vec4f& vert) {
        return vert.x > vert.w || vert.x < -vert.w
            || vert.y > vert.w || vert.y < -vert.w
            || vert.z > vert.w || vert.z < -vert.w;
    }

    IShader* shader_;
    enum RendererMode rendererMode_;
    SFrameBuffer* frameBuffer_;
    Matrix viewport_;
    bool isBackCulling_;
    bool isClipTriangle_;
};

