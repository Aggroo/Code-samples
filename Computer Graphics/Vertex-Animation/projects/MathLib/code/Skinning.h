//
// Created by antgrn-3 on 9/26/16.
//
#ifndef GSCEPT_LAB_ENV_SKINNING_H
#define GSCEPT_LAB_ENV_SKINNING_H
#include <string>
#include "GL/glew.h"
#include "nvx2fileformatstructs.h"
#include "MeshResource.h"
#include <vector>

using namespace CGMath;

namespace Animation
{
struct VertexComponent
{
    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)

    };

    /// stride type tells if the compoent should be per-instance or per-vertex
    enum StrideType
    {
        PerVertex,
        PerInstance
    };

    VertexComponent(SemanticName semName, uint semIndex, Format format, uint streamIndex=0, StrideType strideType=PerVertex, size_t stride=0) : semName(semName),
                                                                                                                                                semIndex(semIndex),
                                                                                                                                                format(format),
                                                                                                                                                streamIndex(streamIndex),
                                                                                                                                                byteOffset(0),
                                                                                                                                                strideType(strideType),
                                                                                                                                                stride(stride) { };

    SemanticName semName;
    uint semIndex;
    Format format;
    StrideType strideType;
    size_t stride;
    uint streamIndex;
    uint byteOffset;
};

struct PrimitiveGroup
{
    enum Code
    {
        InvalidPrimitiveTopology,

        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
    };

    uint baseVertex;
    size_t numVertices;
    uint baseIndex;
    size_t numIndices;
    Code primitiveTopology;
};

class Skinning
{
public:
    Skinning();
    ~Skinning();

    bool LoadSkinMesh(std::string filename);
    MeshResources* GenSkinningMesh();
    size_t GetByteSize(VertexComponent::Format) const;


private:

    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };

    uchar* ptr;
    uint size;
    uint attrIndex;

    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;

    size_t groupDataSize;
    size_t vertexDataSize;
    size_t indexDataSize;

    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    std::vector<Animation::PrimitiveGroup> primGroups;
    std::vector<Animation::VertexComponent> vertexComponents;

    std::vector<GLuint> offset;
    std::vector<GLuint> typeNumbers;
    std::vector<GLenum> types;
    std::vector<GLboolean> norms;
};
}

#endif //GSCEPT_LAB_ENV_SKINNING_H
