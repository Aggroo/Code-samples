//
// Created by antgrn-3 on 11/7/16.
//

#ifndef GSCEPT_LAB_ENV_PLANE_H
#define GSCEPT_LAB_ENV_PLANE_H

#include "vector4D.h"

namespace Physics
{
class Plane
{
public:
    Plane();
    Plane(CGMath::vector4D p, CGMath::vector4D n);
    //Creates normal by doing dotproduct of 2 vectors created from 3 points
    Plane(CGMath::vector4D p1, CGMath::vector4D p2, CGMath::vector4D p3);
    ~Plane();

    //Returns -1 if points is above plane, returns 1 if below plane
    int PointSide(CGMath::vec4 point);

    //Get a point in the plane
    const CGMath::vector4D &GetPoint() const;
    //Set a point in the plane
    void SetPoint(const CGMath::vector4D &point);

    //Get normal of plane
    const CGMath::vector4D &GetNormal() const;
    //Set normal of plane
    void SetNormal(const CGMath::vector4D &normal);

private:
    CGMath::vector4D point;
    CGMath::vector4D normal;
};
}
#endif //GSCEPT_LAB_ENV_PLANE_H
