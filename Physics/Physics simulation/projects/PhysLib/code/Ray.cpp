//
// Created by antgrn-3 on 11/7/16.
//

#include <MeshResource.h>
#include "Ray.h"
using namespace CGMath;
namespace Physics
{
    Ray::Ray()
    {}

    Ray::~Ray()
    {}

    Ray::Ray(CGMath::vector4D p1, CGMath::vector4D p2)
    {
        this->startPoint = p1;
        this->dir = vector4D::Normalize(p2-p1);
    }

    Ray::Ray(CGMath::matrix4D view, CGMath::matrix4D proj, CGMath::vector4D point)
    {
        vector4D startPoint = point;
        vector4D endPoint = vector4D(point.x(),point.y(), 0.0f);

        matrix4D m = (proj*view).invert();
        this->startPoint = m * startPoint;
        this->startPoint = this->startPoint / this->startPoint.w();
        vector4D end  = m * endPoint;
        end  = end / end.w();
        this->dir = end-this->startPoint;
        this->dir = vector4D::Normalize(this->dir);
    }

    bool Ray::Intersect(const Plane &plane, RayHit& hit)
    {
        vector4D point;
        float d = vector4D::Dot(plane.GetNormal(), plane.GetPoint());

        if (vector4D::Dot(plane.GetNormal(), this->dir) == 0)
        {
            printf("No collision\n");
            return false; // avoid divide by zero
        }

        float t = (d - vector4D::Dot(plane.GetNormal(), this->startPoint)) / vector4D::Dot(plane.GetNormal(), this->dir);

        hit.contact = this->startPoint+(this->dir * t);
        hit.normal = plane.GetNormal();
        hit.dist = t;

        return true;
    }

    bool Ray::CollisionCheck(CGMath::MeshResource &mesh, RayHit &hit, CGMath::matrix4D& modelMatrix)
    {
        hit.dist = 1000000.0f;
        Plane plane;
        Plane edgePlane;
        RayHit faceHit;
        bool hasHit = false;

        for (int i = 0; i < mesh.meshFaces.size(); ++i)
        {
            vector4D p1 = vector4D(modelMatrix*mesh.mesh[mesh.meshFaces[i].verts[0]].pos);
            vector4D p2 = vector4D(modelMatrix*mesh.mesh[mesh.meshFaces[i].verts[1]].pos);
            vector4D p3 = vector4D(modelMatrix*mesh.mesh[mesh.meshFaces[i].verts[2]].pos);

            plane = Plane(p1,p2,p3);

            if(Intersect(plane, faceHit))
            {
                int numOfPlanes = 0;
                for (int j = 0; j < mesh.meshFaces[i].verts.size(); ++j)
                {
                    int nextIndex = j+1;
                    if(nextIndex >= mesh.meshFaces[i].verts.size())
                        nextIndex = nextIndex - mesh.meshFaces[i].verts.size();

                    vector4D edgeP1(modelMatrix*mesh.mesh[mesh.meshFaces[i].verts[j]].pos);
                    vector4D edgeP2(modelMatrix*mesh.mesh[mesh.meshFaces[i].verts[nextIndex]].pos);
                    vector4D edge = vector4D(edgeP2 - edgeP1);

                    edgePlane = Plane(edgeP1, vector4D::Normalize(vector4D::Cross(edge, plane.GetNormal())));
                    if(edgePlane.PointSide(faceHit.contact) < 0)
                    {
                        numOfPlanes++;
                    }
                    else
                        break;

                }
                if(numOfPlanes == mesh.meshFaces[i].verts.size())
                {
                    if(faceHit.dist < hit.dist)
                    {
                        hit = faceHit;
                        printf("CollisionCheck on point: %f %f %f\n", hit.contact.x(), hit.contact.y(), hit.contact.z());
                        hasHit = true;
                    }
                }
            }
        }
        if(hasHit)
            return true;
        return false;
    }

    vector4D &Ray::GetStartPoint()
    {
        return this->startPoint;
    }

    void Ray::SetStartPoint(const vector4D &startPoint)
    {
        this->startPoint = startPoint;
    }

    vector4D &Ray::GetDir()
    {
        return this->dir;
    }

    void Ray::SetDir(const vector4D &dir)
    {
        this->dir = dir;
    }



}
