//
// Created by antgrn-3 on 11/10/16.
//

#include "OABB.h"
using namespace CGMath;

Physics::OABB::OABB() {}

Physics::OABB::~OABB() {}

void Physics::OABB::SetUp(const CGMath::MeshResource& mesh, CGMath::matrix4D& modelMat)
{
    this->boundMin = this->boundMax = vector4D(mesh.mesh[0].pos);
    for (int i = 0; i < mesh.mesh.size(); ++i)
    {
        if(mesh.mesh[i].pos.x() < this->boundMin.x())
        {
            this->boundMin = vector4D(mesh.mesh[i].pos.x(), this->boundMin.y(),this->boundMin.z());
        }
        else if(mesh.mesh[i].pos.x() > this->boundMax.x())
        {
            this->boundMax = vector4D(mesh.mesh[i].pos.x(), this->boundMax.y(),this->boundMax.z());
        }
        else if(mesh.mesh[i].pos.y() < this->boundMin.y())
        {
            this->boundMin = vector4D(this->boundMin.x(), mesh.mesh[i].pos.y(),this->boundMin.z());
        }
        else if(mesh.mesh[i].pos.y() > this->boundMax.y())
        {
            this->boundMax = vector4D(this->boundMax.x(), mesh.mesh[i].pos.y(), this->boundMax.z());
        }
        else if(mesh.mesh[i].pos.z() < this->boundMin.z())
        {
            this->boundMin = vector4D(this->boundMin.x(), this->boundMin.y(), mesh.mesh[i].pos.z());
        }
        else if(mesh.mesh[i].pos.z() > this->boundMax.z())
        {
            this->boundMax = vector4D(this->boundMax.x(), this->boundMax.y(), mesh.mesh[i].pos.z());
        }
    }

    vector4D min = this->boundMin;
    vector4D max = this->boundMax;
    v1 = this->transform*min;
    v2 = this->transform*vector3D(min.x(), max.y(), min.z());
    v3 = this->transform*vector3D(max.x(), max.y(), min.z());
    v4 = this->transform*vector3D(max.x(), min.y(), min.z());
    v5 = this->transform*vector3D(min.x(), min.y(), max.z());
    v6 = this->transform*vector3D(min.x(), max.y(), max.z());
    v7 = this->transform*vector3D(max.x(), min.y(), max.z());
    v8 = this->transform*max;

    this->transform = modelMat;

}

CGMath::MeshResource Physics::OABB::ToMesh()
{
    MeshResource mesh;
    vector4D min = this->boundMin;
    vector4D max = this->boundMax;

    v1 = this->transform*min;
    v2 = this->transform*vector3D(min.x(), max.y(), min.z());
    v3 = this->transform*vector3D(max.x(), max.y(), min.z());
    v4 = this->transform*vector3D(max.x(), min.y(), min.z());
    v5 = this->transform*vector3D(min.x(), min.y(), max.z());
    v6 = this->transform*vector3D(min.x(), max.y(), max.z());
    v7 = this->transform*vector3D(max.x(), min.y(), max.z());
    v8 = this->transform*max;

    CGMath::Vertex v1;
    v1.pos = (this->v1).ToVec3();
    v1.uv = vector2D(0.0f,0.0f);
    v1.norm = (this->v1).ToVec3();
    mesh.mesh.push_back(v1);

    CGMath::Vertex v2;
    v2.pos = (this->v2).ToVec3();
    v2.uv = vector2D(0.0f,0.0f);
    v2.norm = (this->v2).ToVec3();
    mesh.mesh.push_back(v2);

    CGMath::Vertex v3;
    v3.pos = (this->v3).ToVec3();
    v3.uv = vector2D(0.0f,0.0f);
    v3.norm = (this->v3).ToVec3();
    mesh.mesh.push_back(v3);

    CGMath::Vertex v4;
    v4.pos = (this->v4).ToVec3();
    v4.uv = vector2D(0.0f,0.0f);
    v4.norm = (this->v4).ToVec3();
    mesh.mesh.push_back(v4);

    CGMath::Vertex v5;
    v5.pos = (this->v5).ToVec3();
    v5.uv = vector2D(0.0f,0.0f);
    v5.norm = (this->v5).ToVec3();
    mesh.mesh.push_back(v5);

    CGMath::Vertex v6;
    v6.pos = (this->v6).ToVec3();
    v6.uv = vector2D(0.0f,0.0f);
    v6.norm = (this->v6).ToVec3();
    mesh.mesh.push_back(v6);

    CGMath::Vertex v7;
    v7.pos = (this->v7).ToVec3();
    v7.uv = vector2D(0.0f,0.0f);
    v7.norm = (this->v7).ToVec3();
    mesh.mesh.push_back(v7);

    CGMath::Vertex v8;
    v8.pos = (this->v8).ToVec3();
    v8.uv = vector2D(0.0f,0.0f);
    v8.norm = (this->v8).ToVec3();
    mesh.mesh.push_back(v8);

    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);
    mesh.indices.push_back(0);
    mesh.indices.push_back(2);
    mesh.indices.push_back(3);

    mesh.indices.push_back(4);
    mesh.indices.push_back(5);
    mesh.indices.push_back(1);
    mesh.indices.push_back(4);
    mesh.indices.push_back(1);
    mesh.indices.push_back(0);

    mesh.indices.push_back(6);
    mesh.indices.push_back(7);
    mesh.indices.push_back(5);
    mesh.indices.push_back(6);
    mesh.indices.push_back(4);
    mesh.indices.push_back(5);

    mesh.indices.push_back(3);
    mesh.indices.push_back(2);
    mesh.indices.push_back(7);
    mesh.indices.push_back(3);
    mesh.indices.push_back(7);
    mesh.indices.push_back(6);

    mesh.indices.push_back(0);
    mesh.indices.push_back(4);
    mesh.indices.push_back(6);
    mesh.indices.push_back(0);
    mesh.indices.push_back(6);
    mesh.indices.push_back(3);

    mesh.indices.push_back(1);
    mesh.indices.push_back(5);
    mesh.indices.push_back(7);
    mesh.indices.push_back(1);
    mesh.indices.push_back(7);
    mesh.indices.push_back(2);

    CGMath::Face f1;
    f1.verts.push_back(0);
    f1.verts.push_back(1);
    f1.verts.push_back(2);
    f1.verts.push_back(3);

    CGMath::Face f2;
    f2.verts.push_back(4);
    f2.verts.push_back(5);
    f2.verts.push_back(1);
    f2.verts.push_back(0);

    CGMath::Face f3;
    f3.verts.push_back(6);
    f3.verts.push_back(7);
    f3.verts.push_back(5);
    f3.verts.push_back(4);

    CGMath::Face f4;
    f4.verts.push_back(3);
    f4.verts.push_back(2);
    f4.verts.push_back(7);
    f4.verts.push_back(6);

    CGMath::Face f5;
    f5.verts.push_back(1);
    f5.verts.push_back(5);
    f5.verts.push_back(7);
    f5.verts.push_back(2);

    CGMath::Face f6;
    f6.verts.push_back(0);
    f6.verts.push_back(4);
    f6.verts.push_back(6);
    f6.verts.push_back(3);

    mesh.meshFaces.push_back(f1);
    mesh.meshFaces.push_back(f2);
    mesh.meshFaces.push_back(f3);
    mesh.meshFaces.push_back(f4);
    mesh.meshFaces.push_back(f5);
    mesh.meshFaces.push_back(f6);

    return mesh;
}

void Physics::OABB::DrawBoundingBox()
{
    vector4D min = this->boundMin;
    vector4D max = this->boundMax;
    v1 = this->transform*min;
    v2 = this->transform*vector3D(min.x(), max.y(), min.z());
    v3 = this->transform*vector3D(max.x(), max.y(), min.z());
    v4 = this->transform*vector3D(max.x(), min.y(), min.z());
    v5 = this->transform*vector3D(min.x(), min.y(), max.z());
    v6 = this->transform*vector3D(min.x(), max.y(), max.z());
    v7 = this->transform*vector3D(max.x(), min.y(), max.z());
    v8 = this->transform*max;

    glBegin(GL_LINES);

    glColor3f(0.0f, 0.0f, 1.0f);
    //Front
    glVertex4f(v1[0], v1[1], v1[2], v1[3]);
    glVertex4f(v2[0], v2[1], v2[2], v2[3]);

    glVertex4f(v2[0], v2[1], v2[2], v2[3]);
    glVertex4f(v3[0], v3[1], v3[2], v3[3]);

    glVertex4f(v3[0], v3[1], v3[2], v3[3]);
    glVertex4f(v4[0], v4[1], v4[2], v4[3]);

    glVertex4f(v4[0], v4[1], v4[2], v4[3]);
    glVertex4f(v1[0], v1[1], v1[2], v1[3]);

    //Left
    glVertex4f(v1[0], v1[1], v1[2], v1[3]);
    glVertex4f(v5[0], v5[1], v5[2], v5[3]);

    glVertex4f(v5[0], v5[1], v5[2], v5[3]);
    glVertex4f(v6[0], v6[1], v6[2], v6[3]);

    glVertex4f(v6[0], v6[1], v6[2], v6[3]);
    glVertex4f(v2[0], v2[1], v2[2], v2[3]);

    //Right
    glVertex4f(v7[0], v7[1], v7[2], v7[3]);
    glVertex4f(v4[0], v4[1], v4[2], v4[3]);

    glVertex4f(v8[0], v8[1], v8[2], v8[3]);
    glVertex4f(v3[0], v3[1], v3[2], v3[3]);

    glVertex4f(v7[0], v7[1], v7[2], v7[3]);
    glVertex4f(v8[0], v8[1], v8[2], v8[3]);

    //Back
    glVertex4f(v7[0], v7[1], v7[2], v7[3]);
    glVertex4f(v5[0], v5[1], v5[2], v5[3]);

    glVertex4f(v8[0], v8[1], v8[2], v8[3]);
    glVertex4f(v6[0], v6[1], v6[2], v6[3]);

    glEnd();
}

void Physics::OABB::UpdateTransform(CGMath::matrix4D modelMat)
{
    this->transform = modelMat;
}
