//
// Created by antgrn-3 on 11/18/16.
//

#include "PhysicsNode.h"


namespace Physics
{

PhysicsNode::PhysicsNode()
{
    this->oabb = OABB();
    this->aabb = AABB();
    this->body = RigidBody();
}

PhysicsNode::~PhysicsNode() {}

PhysicsNode::PhysicsNode(Physics::AABB a, Physics::OABB o) : aabb(aabb), oabb(oabb)
{
    this->body = RigidBody();
}

RigidBody& PhysicsNode::GetBody()
{
    return body;
}

void PhysicsNode::SetBody(const RigidBody &body)
{
    this->body = body;
}

AABB* PhysicsNode::GetAabb()
{
    return &aabb;
}

void PhysicsNode::SetAabb(const AABB &aabb)
{
    this->aabb = aabb;
    this->aabb.SetOwner(this);
}

OABB* PhysicsNode::GetOabb()
{
    return &oabb;
}

void PhysicsNode::SetOabb(const OABB &oabb)
{
    this->oabb = oabb;
}



}