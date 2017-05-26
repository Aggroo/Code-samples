//
// Created by antgrn-3 on 11/7/16.
//

#include "Plane.h"
#include "vector4D.h"
using namespace CGMath;

namespace Physics
{
Plane::Plane() : point(0.0f, 0.0f, 0.0f), normal(0.0f, 1.0f, 0.0f)
{
}

Plane::~Plane()
{
}

Plane::Plane(CGMath::vector4D p, CGMath::vector4D n) : point(p), normal(n)
{
}

Plane::Plane(CGMath::vector4D p1, CGMath::vector4D p2, CGMath::vector4D p3) : point(p1)
{
    this->normal = vector4D::Normalize(vec4::Cross(p2 - p1, p3 - p2));
}

int Plane:: PointSide(CGMath::vector4D point)
{
    float dot = vector4D::Dot(this->normal,(this->point - point));

    if (dot > 0.0f)
        return -1;
    else if (dot < 0.0f)
        return 1;

    return 0;
}

const vector4D &Plane::GetPoint() const
{
    return this->point;
}

void Plane::SetPoint(const vector4D &point)
{
    this->point = point;
}

const vector4D &Plane::GetNormal() const
{
    return this->normal;
}

void Plane::SetNormal(const vector4D &normal)
{
    this->normal = normal;
}
}