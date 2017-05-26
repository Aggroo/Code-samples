//
// Created by antgrn-3 on 9/26/16.
//

#ifndef GSCEPT_LAB_ENV_NVX2FILEFORMATSTRUCTS_H
#define GSCEPT_LAB_ENV_NVX2FILEFORMATSTRUCTS_H
#include "config.h"

namespace Animation
{
#pragma pack(push, 1)

#define NVX2_MAGICNUMBER 'NVX2'

//------------------------------------------------------------------------------
/**
    NVX2 file format structs.
    NOTE: keep all header-structs 4-byte aligned!
*/
    struct Nvx2Header {
        uint magic;
        uint numGroups;
        uint numVertices;
        uint vertexWidth;
        uint numIndices;
        uint numEdges;
        uint vertexComponentMask;
    };

    struct Nvx2Group {
        uint firstVertex;
        uint numVertices;
        uint firstTriangle;
        uint numTriangles;
        uint firstEdge;
        uint numEdges;
    };

#pragma pack(pop)
}
#endif //GSCEPT_LAB_ENV_NVX2FILEFORMATSTRUCTS_H
