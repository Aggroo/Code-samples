//
// Created by antgrn-3 on 11/4/16.
//
#ifndef GSCEPT_LAB_ENV_AABB_H
#define GSCEPT_LAB_ENV_AABB_H

#include <MeshResource.h>
#include "matrix4D.h"

namespace Physics
{
class PhysicsNode;
class AABB
{
public:
    AABB();
    AABB(CGMath::vector3D min, CGMath::vector3D max);

    void SetUp(const CGMath::MeshResource& mesh);
    void DrawBoundingBox();
    CGMath::MeshResource ToMesh();
    void SetOwner(Physics::PhysicsNode *owner);
    Physics::PhysicsNode* GetOwner();

    CGMath::vector4D boundMin;
    CGMath::vector4D boundMax;

private:

    Physics::PhysicsNode* owner;

};
}



#endif //GSCEPT_LAB_ENV_AABB_H
