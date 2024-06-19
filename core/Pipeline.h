#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#ifdef _EXPORTING  
#define PIPELINE_EXPORT    __declspec(dllexport)  
#else  
#define PIPELINE_EXPORT    __declspec(dllimport)  
#endif  

#include "Mesh.h"
#include "../shader/IShader.h"
#include "../GlobalConst.h"
#include "FrameBuffer.h"
#include <array>
#include "ShaderVertex.h"

class PIPELINE_EXPORT PipeLine {
public:
    static PipeLine& getInstance();
    void bindFrameBuffer(FrameBuffer* frameBuffer);
    void setRendererMode(RendererMode rendererMode);
    void bindShader(IShader* shader);
    void drawMesh(Mesh& mesh);

    PipeLine(const PipeLine&) = delete;
    PipeLine(PipeLine&&) = delete;
    PipeLine& operator=(const PipeLine&) = delete;
    PipeLine& operator=(PipeLine&&) = delete;
private:
    PipeLine();
    ~PipeLine();
    void drawPointMode(std::array<Vertex, 3> triangle,std::vector<int> textureIdx, int idx = 0);
    void drawLineMode(std::array<Vertex, 3> triangle, std::vector<int> textureIdx, int idx = 0);
    void drawTriangleMode(std::array<Vertex, 3> triangle, std::vector<int> textureIdx, int idx = 0);

    void drawLine(const ShaderVertex& v1, const ShaderVertex& v2);
    IShader* shader_;
    FrameBuffer* frameBuffer_;
    enum RendererMode rendererMode_;
    std::array<ShaderVertex, threadNum * 3> shaderTri_;
};

#endif