//
// Created by antgrn-3 on 11/7/16.
//

#ifndef GSCEPT_LAB_ENV_RAY_H
#define GSCEPT_LAB_ENV_RAY_H

#include "vector4D.h"
#include "matrix4D.h"
#include "Plane.h"

namespace Physics
{

struct RayHit
{
    CGMath::vector4D contact;
    CGMath::vector4D normal;
    float dist;
};

class Ray
{
public:
    Ray();
    Ray(CGMath::vector4D p1, CGMath::vector4D p2);
    Ray(CGMath::matrix4D view, CGMath::matrix4D proj, CGMath::vector4D point);
    ~Ray();

    bool Intersect(const Plane& plane, RayHit& hit);
    bool CollisionCheck(CGMath::MeshResource &mesh, RayHit &hit, CGMath::matrix4D &modelMatrix);

    CGMath::vector4D &GetStartPoint();
    void SetStartPoint(const CGMath::vector4D &startPoint);
    CGMath::vector4D &GetDir();
    void SetDir(const CGMath::vector4D &dir);

private:
    CGMath::vector4D startPoint;
    CGMath::vector4D dir;



};
}

#endif //GSCEPT_LAB_ENV_RAY_H
