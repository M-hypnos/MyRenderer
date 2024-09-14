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
    void bindFrameBuffer(SFrameBuffer* frameBuffer) { frameBuffer_ = frameBuffer; }
    void setRendererMode(RendererMode rendererMode) { rendererMode_ = rendererMode; }
    void bindShader(IShader* shader) { shader_ = shader; }
    //void drawMesh(Mesh& mesh);
    void drawObj(const SObj_Loader* obj);

    SPipeline(const SPipeline&) = delete;
    SPipeline(SPipeline&&) = delete;
    SPipeline& operator=(const SPipeline&) = delete;
    SPipeline& operator=(SPipeline&&) = delete;
private:
    SPipeline();
    ~SPipeline();

    void drawMesh(Mesh& mesh);
    void drawLine(const Vec4f& v1, const Vec4f& v2);


    IShader* shader_;
    enum RendererMode rendererMode_;
    SFrameBuffer* frameBuffer_;
};

