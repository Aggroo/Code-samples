//
// Created by antgrn-3 on 11/18/16.
//

#ifndef GSCEPT_LAB_ENV_PHYSICSNODE_H
#define GSCEPT_LAB_ENV_PHYSICSNODE_H

#include "GraphicsNode.h"
#include "AABB.h"
#include "OABB.h"
#include "RigidBody.h"

namespace Physics
{
class PhysicsNode : public CGMath::GraphicsNode
{
public:
    PhysicsNode();
    PhysicsNode(AABB a, OABB o);
    ~PhysicsNode();

    RigidBody& GetBody();
    void SetBody(const RigidBody &body);
    AABB* GetAabb();
    void SetAabb(const AABB &aabb);
    OABB* GetOabb();
    void SetOabb(const OABB &oabb);

private:
    AABB aabb;
    OABB oabb;
    RigidBody body;
};
}

#endif //GSCEPT_LAB_ENV_PHYSICSNODE_H
