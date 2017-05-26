#pragma once
#include "GL/glew.h"
#include "config.h"
#include "vector2D.h"
#include "vector3D.h"
#include "vector4D.h"
#include <map>
#include <vector>


namespace CGMath
{
	struct Vertex;
	
	struct Face
	{
		std::vector<GLuint> verts;
	};
	
	struct Vertex
	{
		vector3D pos;
		vector2D uv;
		vector3D norm;
	};
	
	class MeshResource
	{
	public:
		MeshResource();
		MeshResource(std::vector<CGMath::Vertex>, std::vector<GLuint>);
		~MeshResource();

		void genBuffer();
		void genSkinnedBuffer(size_t vertexSize, uchar* vertexPtr, size_t indexSize, uchar* indexPtr, GLuint size);
		bool loadObj(const char* filename);
		void createQuad();
		void createQuad2();
		void drawMesh();
		void drawSkinnedMesh();

		//Used to concatenating three ints to a string
		std::string FaceKey(int pos,int uv,int norm);
		std::vector<CGMath::Vertex> GetMesh();
		std::vector<CGMath::Face>& GetMeshFaces();
		std::vector<GLuint> getIndices();
		
		std::vector<CGMath::Vertex> mesh;
		std::vector<GLuint> indices;
		std::vector<Face> meshFaces;
	private:

		GLuint vbo[1];
		GLuint vao[1];
		GLuint ibo[1];
		GLuint tSize = 0;
		GLuint iSize = 0;

		size_t vertexSize = 0;
		uchar* vertexPtr = nullptr;

		size_t indexSize = 0;
		uchar* indexPtr = nullptr;

		//Used to keep the loader from creating double vertices
		std::map<std::string,GLuint> vertexMap;
		
	};

}