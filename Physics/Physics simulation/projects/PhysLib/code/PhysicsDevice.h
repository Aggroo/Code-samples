//
// Created by antgrn-3 on 11/30/16.
//

#ifndef GSCEPT_LAB_ENV_PHYSICSDEVICE_H
#define GSCEPT_LAB_ENV_PHYSICSDEVICE_H

#include "PhysicsNode.h"

namespace Physics
{

struct EPAInfo
{
    CGMath::vector4D contactPoint;
    CGMath::vector4D contactNormal;
    float depth;
};

struct SupportPoint
{
    //One of two points from the support function
    CGMath::vector4D pointA;

    //One of two points from the support function
    CGMath::vector4D pointB;

    //The point from minkowski difference
    CGMath::vector4D diff;

    SupportPoint(){}

    SupportPoint(CGMath::vector4D pA, CGMath::vector4D pB, CGMath::vector4D d) : pointA(pA), pointB(pB), diff(d){}

    bool operator==(const SupportPoint& s)
    {
        return diff == s.diff;
    }
};

struct Triangle
{
    SupportPoint vert1;
    SupportPoint vert2;
    SupportPoint vert3;
    CGMath::vector4D normal;

    Triangle(SupportPoint v1, SupportPoint v2, SupportPoint v3) : vert1(v1), vert2(v2), vert3(v3)
    {
        normal = CGMath::vector4D::Normalize(CGMath::vector4D::Cross((vert2.diff-vert1.diff),(vert3.diff-vert1.diff)));
    }
};

struct Edge
{
    SupportPoint start;
    SupportPoint end;

    Edge(SupportPoint start, SupportPoint end) : start(start), end(end) {}
};

class PhysicsDevice
{
public:
    PhysicsDevice();
    ~PhysicsDevice();


    void RegisterPhysicsNode(PhysicsNode* node);
    const std::vector<PhysicsNode*>& GetPhysicsNodes();

    //Sorts and checks for collision between AABBs
    bool PlaneSweep();
    //Check if AABBs overlap
    bool AABBOverlap(AABB* a, AABB* b);

    //Check for narrowphase intersection
    bool Intersection(PhysicsNode& shape1, PhysicsNode& shape2, CGMath::vector4D& dir);

    //Collects the EPA information of the GJK intersection
    void CollisionInformation(PhysicsNode& shape1, PhysicsNode& shape2, EPAInfo& contactData);

    //Fills contactData with info of the collision (Contact point, collision depth and collision normal)
    bool GetContactInfo(const Triangle* tri, EPAInfo& contactData);

    //Returns a point inside the Minkowski Difference
    SupportPoint Support(CGMath::MeshResource& shape1, CGMath::MeshResource& shape2, CGMath::vector4D& dir, CGMath::matrix4D& shape1Trans, CGMath::matrix4D& shape2Trans);

    //Get the furthest vertex position in a mesh
    CGMath::vector4D GetFurthestPoint(CGMath::MeshResource &shape, const CGMath::vector4D &dir);

    //Calculates the simplex using a switch case to find if a tetrahedron contains the origin
    bool SimplexCalculation(std::vector<SupportPoint> &list, CGMath::vector4D &dir);

    void LineCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir);
    void TriangleCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir);
    bool TetrahedronCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir);

    void QuickSort(std::vector<AABB*> &arr, int left, int right);
    //Check if sorted
    bool isSorted();

    float CalculateImpulse(PhysicsNode& a, PhysicsNode& b, float& relVel, EPAInfo& colData);

    std::vector<std::pair<AABB *, AABB *>> &GetNarrowPhaseArray();


private:
    std::vector<PhysicsNode*> nodes;
    std::vector<AABB*> AABBArray;

    std::vector<SupportPoint> simplex;

    std::vector<std::pair<AABB*,AABB*>> narrowPhaseArray;

    int axisIndex;

    CGMath::vector4D AO;
    CGMath::vector4D AB;
    CGMath::vector4D AC;
    CGMath::vector4D ABC;


};
}



#endif //GSCEPT_LAB_ENV_PHYSICSDEVICE_H
