//
// Created by antgrn-3 on 11/17/16.
//

#ifndef GSCEPT_LAB_ENV_RIGIDBODY_H
#define GSCEPT_LAB_ENV_RIGIDBODY_H

#include "matrix4D.h"
#include "Quaternion.h"
#include "AABB.h"

namespace Physics
{

struct State
{
    //Position values
    CGMath::vector4D position;
    CGMath::vector4D velocity;
    CGMath::vector4D acceleration;

    CGMath::vector4D lastFrameAcc;

    //Rotation values
    CGMath::Quaternion orientation;
    CGMath::vector4D rotation;
    CGMath::matrix4D inverseInertiaTensorWorld;

    CGMath::vector4D angularAcc;
};

enum Integrators
{
    RK4,
    Euler,
    Midpoint
};

class RigidBody
{
public:
    RigidBody();
    ~RigidBody();

    void Initialize(float mass, AABB* aabb, CGMath::matrix4D mat, bool staticObj);
    void Initialize(float mass, AABB* aabb, CGMath::matrix4D mat, CGMath::vector4D scale, bool staticObj);

    State Evaluate(State& init, float timestep, State& d);

    void Update(float timestep);
    void CalculateData();

    void RK4(float timestep);
    void Euler(float timestep);
    void Midpoint(float timestep);

    void AddForceAtPoint(const CGMath::vector4D& force, const CGMath::vector4D& point);
    CGMath::vector4D CalculateDeltaCollisionPoint(CGMath::vector4D colPoint);

    CGMath::matrix4D& GetInverseInertiaTensorWorld();
    void SetInverseInertiaTensor(CGMath::matrix4D &inertiaTensor);

    CGMath::vector4D &GetVelocity();
    CGMath::vector4D &GetRotation();

    void SetVelocity(const CGMath::vector4D &velocity);
    void SetRotation(const CGMath::vector4D &rotation);

    CGMath::matrix4D& GetTransform();
    State& GetState();

    float inverseMass;

    float linearDamp;
    float angularDamp;

    bool isAwake = true;

    CGMath::vector4D centerOfMass;

    static Integrators integrate;

private:

    State state;

    CGMath::matrix4D transform;

    CGMath::vector4D forces;
    CGMath::vector4D torques;

    CGMath::matrix4D inverseInertiaTensor;

    CGMath::vector4D scale;

};
}

#endif //GSCEPT_LAB_ENV_RIGIDBODY_H
