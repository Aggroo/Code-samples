//
// Created by antgrn-3 on 11/30/16.
//

#include "PhysicsDevice.h"
#include <algorithm>
#include <float.h>
#include <cmath>

using namespace CGMath;

namespace Physics
{

PhysicsDevice::PhysicsDevice() : axisIndex(0) {}
PhysicsDevice::~PhysicsDevice() {}

void PhysicsDevice::RegisterPhysicsNode(PhysicsNode *node)
{
    nodes.push_back(node);
}

const std::vector<PhysicsNode *>& PhysicsDevice::GetPhysicsNodes()
{
    return this->nodes;
}

bool PhysicsDevice::PlaneSweep()
{
    bool isOverlaping = false;
    //Clears list after each update
    this->narrowPhaseArray.clear();

    //If the AABBArray is empty pushback all registered PhysicsNodes
    if(this->AABBArray.empty())
    {
        for (int i = 0; i < this->nodes.size(); ++i)
        {
            this->AABBArray.push_back(this->nodes[i]->GetAabb());
        }
    }

    //If it's not sorted quicksort the AABBArray
    if(!isSorted())
        QuickSort(this->AABBArray, 0, this->AABBArray.size()-1);

    //Sweep for collisions
    CGMath::vector3D v1,v2,v3;
    for (int i = 0; i < this->AABBArray.size(); ++i)
    {
        //Calculate center of AABBs
        CGMath::vector4D center = (this->AABBArray[i]->boundMin + this->AABBArray[i]->boundMax)*0.5f;

        //Update sums for finding varience of AABB centers
        for (int c = 0; c < 3; ++c)
        {
            v1[c] += center[c];
            v2[c] += center[c] * center[c];
        }

        for (int j = i+1; j < this->AABBArray.size(); ++j)
        {
            if(this->AABBArray[j]->boundMin[this->axisIndex] > this->AABBArray[i]->boundMax[this->axisIndex])
            {
                break;
            }
            else if(AABBOverlap(this->AABBArray[i], this->AABBArray[j]))
            {
                std::pair<AABB*, AABB*> p(this->AABBArray[i], this->AABBArray[j]);
                this->narrowPhaseArray.push_back(p);
                isOverlaping = true;
            }
        }
    }
    //Compute varience
    for (int c = 0; c < 3; ++c)
    {
        v3[c] = v2[c] - v1[c]*v1[c] / this->AABBArray.size();
    }
    axisIndex = 0;
    if(v3[1] > v3[0])
        axisIndex = 1;
    if(v3[2] > v3[axisIndex])
        axisIndex = 2;

    if(isOverlaping)
        return true;

    return false;

}

void PhysicsDevice::QuickSort(std::vector<AABB*> &arr, int left, int right)
{

    int i = left, j = right;
    AABB* tmp;
    float pivot = arr[(left + right) / 2]->boundMin[this->axisIndex];

    /* partition */
    while (i <= j)
    {
        while (arr[i]->boundMin[this->axisIndex] < pivot)
            i++;

        while (arr[j]->boundMin[this->axisIndex] > pivot)
            j--;

        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        QuickSort(arr, left, j);

    if (i < right)
        QuickSort(arr, i, right);
}

bool PhysicsDevice::isSorted()
{
    for (int i = 0; i < AABBArray.size()-1; ++i)
    {
        if(AABBArray[i]->boundMin[this->axisIndex] > AABBArray[i+1]->boundMin[this->axisIndex])
            return false;
    }
    return true;
}

bool PhysicsDevice::AABBOverlap(AABB* a, AABB* b)
{
    if(a->boundMax.x() > b->boundMin.x() && a->boundMin.x() < b->boundMax.x() && a->boundMax.y() > b->boundMin.y() &&
       a->boundMin.y() < b->boundMax.y() && a->boundMax.z() > b->boundMin.z() && a->boundMin.z() < b->boundMax.z())
    {
        return true;
    }
    return false;
}

bool PhysicsDevice::Intersection(PhysicsNode& shape1, PhysicsNode& shape2, CGMath::vector4D& dir)
{
    SupportPoint s = Support(*shape1.getMesh().get(), *shape2.getMesh().get(), dir, shape1.GetBody().GetTransform(), shape2.GetBody().GetTransform());
    std::vector<SupportPoint> simplexList;
    simplexList.push_back(s);

    dir = s.diff.Negative();

    const int iteratorMax = shape1.getMesh()->mesh.size() + shape2.getMesh()->mesh.size();

    int iteration = 0;

    while(true)
    {
        if(iteration >= iteratorMax)
            return false;

        SupportPoint a = Support(*shape1.getMesh().get(),*shape2.getMesh().get(), dir, shape1.GetBody().GetTransform(), shape2.GetBody().GetTransform());
        if(vector4D::Dot(a.diff,dir) < 0)
        {
            return false;
        }
        else
        {
            simplexList.push_back(a);
            if(SimplexCalculation(simplexList, dir))
            {
                return true;
            }
        }
        iteration++;
    }
}

SupportPoint PhysicsDevice::Support(CGMath::MeshResource& shape1, CGMath::MeshResource& shape2, CGMath::vector4D& dir, matrix4D& shape1Trans, matrix4D& shape2Trans)
{
    vector4D point1 = GetFurthestPoint(shape1, matrix3D::fromMatrix4D(shape1Trans).invert()*dir.ToVec3());
    vector4D point2 = GetFurthestPoint(shape2, matrix3D::fromMatrix4D(shape2Trans).invert()*dir.Negative().ToVec3());

    vector4D point3 = (shape1Trans*point1)-(shape2Trans*point2);

    SupportPoint p(point1, point2, point3);

    return p;
}

CGMath::vector4D PhysicsDevice::GetFurthestPoint(CGMath::MeshResource& shape, const CGMath::vector4D& dir)
{
    float max = -1000000;
    int index = 0;
    for (int i = 0; i < shape.GetMesh().size(); i++)
    {
        float dot = CGMath::vector4D::Dot(shape.GetMesh()[i].pos, dir);
        if (dot > max)
        {
            max = dot;
            index = i;
        }
    }
    return shape.GetMesh()[index].pos;
}

bool PhysicsDevice::SimplexCalculation(std::vector<SupportPoint>& list, CGMath::vector4D& dir)
{
    switch(list.size())
    {
        case 4: return TetrahedronCase(list,dir);
        case 3: TriangleCase(list,dir); break;
        case 2: LineCase(list, dir); break;
        default: assert("Invalid simplex"); break;
    }
    return false;
}

void PhysicsDevice::LineCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir)
{
    AO = list[1].diff.Negative();
    AB = (list[0].diff-list[1].diff);
    if(vector4D::Dot(AB, AO) > 0)
    {
        dir = vector4D::Cross(vector4D::Cross(AB,AO),AB);
    }
    else
    {
        list.erase(list.begin());
        dir = AO;
    }
}

void PhysicsDevice::TriangleCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir)
{
    AO = list[2].diff.Negative();
    AC = list[0].diff-list[2].diff;
    AB = list[1].diff-list[2].diff;
    ABC = vector4D::Cross(AB,AC);

    if(vector4D::Dot(vector4D::Cross(ABC,AC), AO) > 0)
    {
        if(vector4D::Dot(AC,AO) > 0)
        {
            dir = vector4D::Cross(vector4D::Cross(AC,AO),AC);
            list.erase(list.begin()+1);
        }
        else
        {
            list.erase(list.begin());
            if(vector4D::Dot(AB,AO) > 0)
            {
                dir = vector4D::Cross(vector4D::Cross(AB,AO),AB);
            }
            else
            {
                list.erase(list.begin());
                dir = AO;
            }
        }
    }
    else
    {
        if(vector4D::Dot(vector4D::Cross(AB,ABC),AO) > 0)
        {
            list.erase(list.begin());
            if(vector4D::Dot(AB,AO) > 0)
            {
                dir = vector4D::Cross(vector4D::Cross(AB,AO),AB);
            }
            else
            {
                list.erase(list.begin());
                dir = AO;
            }
        }
        else
        {
            if(vector4D::Dot(ABC,AO) > 0)
            {
                dir = ABC;
            }
            else
            {
                SupportPoint temp = list[0];
                list[0] = list[1];
                list[1] = temp;
                dir = ABC.Negative();
            }
        }
    }
}

bool PhysicsDevice::TetrahedronCase(std::vector<SupportPoint> &list, CGMath::vector4D &dir)
{
    AO = list[3].diff.Negative();
    vector4D AD = list[0].diff-list[3].diff;
    vector4D AC = list[1].diff-list[3].diff;
    vector4D AB = list[2].diff-list[3].diff;

    vector4D normA = vector4D::Cross(AD,AB);
    vector4D normB = vector4D::Cross(AB,AC);
    vector4D normC = vector4D::Cross(AC,AD);

    if(vector4D::Dot(normA,AO) > 0)
    {
        list.erase(list.begin()+1);
        TriangleCase(list, dir);
        return false;
    }
    else if(vector4D::Dot(normB,AO) > 0)
    {
        list.erase(list.begin());
        TriangleCase(list, dir);
        return false;
    }
    else if(vector4D::Dot(normC,AO) > 0)
    {
        list.erase(list.begin()+2);
        TriangleCase(list, dir);
        return false;
    }
    else
    {
        this->simplex = list;
        return true;
    }

}

std::vector<std::pair<AABB *, AABB *>>& PhysicsDevice::GetNarrowPhaseArray()
{
    return this->narrowPhaseArray;
}

void PhysicsDevice::CollisionInformation(PhysicsNode& shape1, PhysicsNode& shape2, EPAInfo& contactData)
{
    const float THRESHOLD = 0.001f;
    const unsigned ITERATION_LIMIT = 50;
    unsigned ITERATION_CUR = 0;

    std::vector<Triangle> faces;
    std::vector<Edge> edges;

    Triangle tri(simplex[0], simplex[1], simplex[2]);
    faces.push_back(tri);

    tri = Triangle(simplex[0], simplex[3], simplex[2]);
    faces.push_back(tri);

    tri = Triangle(simplex[1], simplex[3], simplex[0]);
    faces.push_back(tri);

    tri = Triangle(simplex[2], simplex[3], simplex[1]);
    faces.push_back(tri);

    auto addEdge = [&](const SupportPoint &a,const SupportPoint &b)->void
    {
        for(auto it = edges.begin(); it != edges.end(); it++)
        {
            if(it->start == b && it->end == a) {
                //opposite edge found, remove it and do not add new one
                edges.erase(it);
                return;
            }
        }
        edges.emplace_back(a,b);
    };

    while(true)
    {
        if(ITERATION_CUR++ >= ITERATION_LIMIT)
            return;

        std::vector<Triangle>::iterator currTriangle = faces.begin();
        float currDst = FLT_MAX;
        for(auto it = faces.begin(); it != faces.end(); it++)
        {
            float dst = fabs(vector4D::Dot(it->normal,it->vert1.diff));
            if(dst < currDst)
            {
                currDst = dst;
                currTriangle = it;
            }
        }

        const SupportPoint s = Support(*shape1.getMesh().get(), *shape2.getMesh().get(), currTriangle->normal, shape1.GetBody().GetTransform(), shape2.GetBody().GetTransform());

        if(vector4D::Dot(currTriangle->normal, s.diff) - currDst < THRESHOLD)
        {
            GetContactInfo(&*currTriangle, contactData);
            break;
        }

        for(auto it = faces.begin(); it != faces.end();)
        {
            if(vector4D::Dot(it->normal,(s.diff - it->vert1.diff))>0)
            {
                addEdge(it->vert1,it->vert2);
                addEdge(it->vert2,it->vert3);
                addEdge(it->vert3,it->vert1);
                it = faces.erase(it);
                continue;
            }
            it++;
        }

        for (auto i = edges.begin(); i != edges.end(); i++)
        {
            Triangle t = Triangle(s,i->start, i->end);
            faces.push_back(t);
        }

        edges.clear();
    }

}

bool PhysicsDevice::GetContactInfo(const Triangle *tri, EPAInfo& contactData)
{
    const float distFromOrigin = vector4D::Dot(tri->normal,tri->vert1.diff);

    //Barycentric coordinates
    float u,v,w;

    //Calculate the barycentric coordinates
    vector4D v0 = tri->vert2.diff - tri->vert1.diff, v1 = tri->vert3.diff - tri->vert1.diff, v2 = (tri->normal*distFromOrigin) - tri->vert1.diff;
    float d00 = vector4D::Dot(v0,v0);
    float d01 = vector4D::Dot(v0,v1);
    float d11 = vector4D::Dot(v1,v1);
    float d20 = vector4D::Dot(v2,v0);
    float d21 = vector4D::Dot(v2,v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;

    if(!std::isfinite(u) || !std::isfinite(v) || !std::isfinite(w))
        return false;

    if(fabs(u) > 1.0f || fabs(v) > 1.0f ||fabs(w) > 1.0f)
        return false;

    const vector4D colNorm = tri->normal.Negative();

    contactData.contactPoint = (tri->vert1.pointA*u)+(tri->vert2.pointA*v)+(tri->vert3.pointA*w);
    contactData.contactNormal = vector4D::Normalize(colNorm);
    contactData.depth = distFromOrigin;

    return true;
}

float PhysicsDevice::CalculateImpulse(PhysicsNode &a, PhysicsNode &b, float& relVel, EPAInfo &colData)
{
    vector4D rA = colData.contactPoint-a.GetBody().centerOfMass;
    vector4D rB = colData.contactPoint-b.GetBody().centerOfMass;
    float p1 = vector4D::Dot(colData.contactNormal,vector4D::Cross(a.GetBody().GetInverseInertiaTensorWorld()*vector4D::Cross(rA,colData.contactNormal),rA));
    float p2 = vector4D::Dot(colData.contactNormal,vector4D::Cross(b.GetBody().GetInverseInertiaTensorWorld()*vector4D::Cross(rB,colData.contactNormal),rB));
    const float restitution = 0.8f;

    return -(1+restitution)*relVel/(a.GetBody().inverseMass + b.GetBody().inverseMass + p1 + p2);
}


}