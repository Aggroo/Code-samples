//
// Created by antgrn-3 on 11/10/16.
//

#ifndef GSCEPT_LAB_ENV_OABB_H
#define GSCEPT_LAB_ENV_OABB_H

#include "matrix4D.h"
#include "MeshResource.h"

namespace Physics
{
class OABB
{
public:
    OABB();
    ~OABB();

    void SetUp(const CGMath::MeshResource& mesh, CGMath::matrix4D& modelMat);
    void UpdateTransform(CGMath::matrix4D modelMat);
    void DrawBoundingBox();
    CGMath::MeshResource ToMesh();


    CGMath::matrix4D transform;
    std::vector<CGMath::vector4D> positions;

private:
    CGMath::vector4D boundMin;
    CGMath::vector4D boundMax;

    CGMath::vector4D v1;
    CGMath::vector4D v2;
    CGMath::vector4D v3;
    CGMath::vector4D v4;
    CGMath::vector4D v5;
    CGMath::vector4D v6;
    CGMath::vector4D v7;
    CGMath::vector4D v8;



};
}



#endif //GSCEPT_LAB_ENV_OABB_H
