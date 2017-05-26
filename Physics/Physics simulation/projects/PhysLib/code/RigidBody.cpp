//
// Created by antgrn-3 on 11/17/16.
//

#include "RigidBody.h"
#include <math.h>

using namespace CGMath;
namespace Physics
{

RigidBody::RigidBody() {}

RigidBody::~RigidBody() {}

Integrators RigidBody::integrate = Integrators::Euler;

void RigidBody::Initialize(float mass, AABB* aabb, CGMath::matrix4D mat, bool staticObj)
{
    //Local space Center of Mass
    this->centerOfMass = (aabb->boundMin+(aabb->boundMax - aabb->boundMin)/2.0f);
    this->inverseMass = 1/mass;

    this->isAwake = !staticObj;

    this->state.position = mat.GetPosition();
    this->state.velocity = vector4D(0.0f,0.0f,0.0f);
    this->state.rotation = vector4D(0.0f,0.0f,0.0f,0.0f);
    this->forces = vector4D(0.0f,0.0f,0.0f);
    this->torques = vector4D(0.0f,0.0f,0.0f);
    this->scale = vector4D(1.0f,1.0f,1.0f);
    this->transform = mat;

    this->state.acceleration = vector4D(0.0f, -9.81f, 0.0f);
    this->state.lastFrameAcc = vector4D(0.0f,0.0f,0.0f);

    this->linearDamp = 0.95f;
    this->angularDamp = 0.95f;

    float width = aabb->boundMax[0] - aabb->boundMin[0];
    float height = aabb->boundMax[1] - aabb->boundMin[1];
    float depth = aabb->boundMax[2] - aabb->boundMin[2];

    matrix4D inertiaTensor = matrix4D().identity();

    if(staticObj)
    {
        this->inverseMass = 0;
        this->state.inverseInertiaTensorWorld = matrix4D();
        inertiaTensor[0] = 1.0f / 12.0f * (0.0f * ((height * height) + (depth * depth)));
        inertiaTensor[5] = 1.0f / 12.0f * (0.0f * ((width * width) + (depth * depth)));
        inertiaTensor[10] = 1.0f / 12.0f * (0.0f * ((width * width) + (height * height)));

        this->inverseInertiaTensor = inertiaTensor.invert();
    }
    else
    {
        this->state.inverseInertiaTensorWorld = matrix4D().identity();
        inertiaTensor[0] = 1.0f / 12.0f * (mass * ((height * height) + (depth * depth)));
        inertiaTensor[5] = 1.0f / 12.0f * (mass * ((width * width) + (depth * depth)));
        inertiaTensor[10] = 1.0f / 12.0f * (mass * ((width * width) + (height * height)));

        this->inverseInertiaTensor = inertiaTensor.invert();
    }


}

void RigidBody::Initialize(float mass, AABB* aabb, CGMath::matrix4D mat, CGMath::vector4D scale, bool staticObj)
{
    Initialize(mass, aabb, mat, staticObj);
    this->scale = scale;
    this->transform = mat*matrix4D::vectorScaling(scale);
}

State RigidBody::Evaluate(State &init, float timestep, State &d)
{

    State deriv = init;

    deriv.lastFrameAcc += d.lastFrameAcc*timestep;
	deriv.angularAcc += d.angularAcc*timestep;

    return deriv;

}

void RigidBody::Update(float timestep)
{
    if(!isAwake)
        return;

    //this->state.lastFrameAcc = this->state.acceleration;
    this->state.lastFrameAcc = this->forces*this->inverseMass;
    this->state.angularAcc = this->state.inverseInertiaTensorWorld*this->torques;

    switch(integrate)
    {
        case Integrators::Euler: Euler(timestep); break;
        case Integrators::Midpoint: Midpoint(timestep); break;
        case Integrators::RK4: RK4(timestep); break;
        default: break;
    }

    CalculateData();

    this->forces = vector4D(0.0f, 0.0f, 0.0f, 0.0f);
    this->torques = vector4D(0.0f, 0.0f, 0.0f, 0.0f);

}

void RigidBody::RK4(float timestep)
{
    State a = Evaluate(this->state, 0.0f, this->state);
    State b = Evaluate(this->state, timestep*0.5f, a);
    State c = Evaluate(this->state, timestep*0.5f, b);
    State d = Evaluate(this->state, timestep, c);

    a.lastFrameAcc = (a.lastFrameAcc+(b.lastFrameAcc + c.lastFrameAcc)*2.0f + d.lastFrameAcc)*(1.0f/6.0f);
    a.angularAcc = (a.angularAcc+(b.angularAcc + c.angularAcc)*2.0f + d.angularAcc)*(1.0f/6.0f);

    this->state.velocity += a.lastFrameAcc*timestep;
    this->state.rotation += a.angularAcc*timestep;

    this->state.velocity = this->state.velocity*pow(linearDamp,timestep);
    this->state.rotation = this->state.rotation*pow(angularDamp,timestep);

    this->state.position += this->state.velocity*timestep;
    this->state.orientation.AddScaleVector(this->state.rotation,timestep);
}

void RigidBody::Euler(float timestep)
{
    State a = Evaluate(this->state, 0.0f, this->state);

    this->state.velocity += a.lastFrameAcc*timestep;
    this->state.rotation += a.angularAcc*timestep;

    this->state.velocity = this->state.velocity*pow(linearDamp,timestep);
    this->state.rotation = this->state.rotation*pow(angularDamp,timestep);

    this->state.position += this->state.velocity*timestep;
    this->state.orientation.AddScaleVector(a.rotation, timestep);
}

void RigidBody::Midpoint(float timestep)
{
    State a = Evaluate(this->state, 0.0f, this->state);
    State b = Evaluate(this->state, timestep*0.5f, a);

    this->state.velocity += b.lastFrameAcc*timestep;
    this->state.rotation += b.angularAcc*timestep;

    this->state.velocity = this->state.velocity*pow(linearDamp,timestep);
    this->state.rotation = this->state.rotation*pow(angularDamp,timestep);

    this->state.position += b.velocity*timestep;
    this->state.orientation.AddScaleVector(b.rotation, timestep);
}

void RigidBody::SetInverseInertiaTensor(matrix4D& inertiaTensor)
{
    this->inverseInertiaTensor = inertiaTensor.invert();
}

void RigidBody::AddForceAtPoint(const CGMath::vector4D &force, const CGMath::vector4D &point)
{
    this->forces += force;
    this->torques += vector4D::Cross((point-(this->transform*this->centerOfMass)), this->forces);
}

void RigidBody::CalculateData()
{
    this->state.orientation.normalize();

    this->transform = matrix4D::translationMatrix(this->state.position) * matrix4D::RotationQuaternion(this->state.orientation) * matrix4D::vectorScaling(this->scale);

    this->state.inverseInertiaTensorWorld = matrix4D::RotationQuaternion(this->state.orientation) * this->inverseInertiaTensor * !matrix4D::RotationQuaternion(this->state.orientation);
}

CGMath::matrix4D& RigidBody::GetTransform()
{
    return this->transform;
}

CGMath::vector4D RigidBody::CalculateDeltaCollisionPoint(CGMath::vector4D colPoint)
{
    return this->state.velocity+(vector4D::Cross(this->state.rotation,(colPoint-centerOfMass)));
}

matrix4D& RigidBody::GetInverseInertiaTensorWorld()
{
    return state.inverseInertiaTensorWorld;
}

void RigidBody::SetVelocity(const vector4D &velocity)
{
    this->state.velocity = velocity;
}

void RigidBody::SetRotation(const vector4D &rotation)
{
    this->state.rotation = rotation;
}

vector4D& RigidBody::GetVelocity()
{
    return this->state.velocity;
}

vector4D& RigidBody::GetRotation()
{
    return this->state.rotation;
}

State& RigidBody::GetState()
{
    return state;
}


}


