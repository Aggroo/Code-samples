#include "MeshResource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>



namespace CGMath
{
	MeshResources::MeshResources() : tSize(0), iSize(0), vertexSize(0), vertexPtr(nullptr), indexSize(0), indexPtr(nullptr) 
	{
	}

	MeshResources::MeshResources(std::vector<CGMath::Vertex> mesh, std::vector<GLuint> indices) : tSize(0), iSize(0), vertexSize(0), vertexPtr(nullptr), indexSize(0), indexPtr(nullptr)
	{
		this->mesh = mesh;
		this->indices = indices;
	}

	MeshResources::~MeshResources()
	{
		glDeleteVertexArrays(1, &vao[0]);
		glDeleteBuffers(1, &vbo[0]);
		glDeleteBuffers(1, &ibo[0]);
	}

	void MeshResources::genBuffer()
	{
		glGenVertexArrays(1, &vao[0]);
		glBindVertexArray(vao[0]);

		glGenBuffers(1, &vbo[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CGMath::Vertex)*mesh.size(), &mesh[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));

		glGenBuffers(1, &ibo[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	void MeshResources::genSkinnedBuffer(size_t vertexSize, uchar* vertexPtr, size_t indexSize, uchar* indexPtr, GLuint size)
	{
		this->vertexSize = vertexSize;
		this->vertexPtr = vertexPtr;
		this->indexSize = indexSize;
		this->indexPtr = indexPtr;
		this->iSize = size;

		glGenVertexArrays(1, &vao[0]);
		glBindVertexArray(vao[0]);

		glGenBuffers(1, &vbo[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, this->vertexSize, this->vertexPtr, GL_STATIC_DRAW);

		glGenBuffers(1, &ibo[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexSize, this->indexPtr, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	bool MeshResources::loadObj(const char* filename)
	{
		
		struct FaceVertex
		{
			int pos = 0;
			int uv = 0;
			int norm = 0;
		};
		
		typedef std::vector<FaceVertex> Faces;
		
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<vector3D> temp_vertices;
		std::vector<vector2D> temp_uvs;
		std::vector<vector3D> temp_normals;
		
		
		std::vector<Faces> faces;
		
		FILE * file = fopen(filename, "r");
		if (file == NULL){
			printf("Impossible to open the file !\n");
		}

		int32 index = 0;

		while (true){
			

			char lineHeader[128];
			// read the first word of the line
			auto res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0){
				GLfloat vertex[3];
				fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0){
				GLfloat uv[2];
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0){
				GLfloat normal[3];
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0){
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				auto matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
				if (!(matches == 9 || matches == 12))
				{
					printf("File can't be read by ourstd::vector<FaceVertex> simple parser :-( Try exporting with other options\n");
					return false;
				}
				Faces faceVertexList;
				auto verticesPerFace = 4 - (matches % 4);
				
				
				for (auto i = 0; i < verticesPerFace; i++)
				{
					FaceVertex vert;
					vert.pos = vertexIndex[i] - 1; 
					vert.uv = uvIndex[i] - 1; 
					vert.norm = normalIndex[i] - 1;
					
					faceVertexList.push_back(vert);
					
				}
				
				faces.push_back(faceVertexList);
				
			}
			else{
				// Probably a comment, eat up the rest of the line
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}
		}
		for(int i = 0; i < faces.size(); ++i)
		{
			CGMath::Face faceVertList;
			for (int j = 0; j < faces[i].size(); ++j)
			{
				int pos = faces[i][j].pos;
				int uv = faces[i][j].uv;
				int norm = faces[i][j].norm;
				std::string key = this->FaceKey(pos, uv, norm);
				if (this->vertexMap.find(key) == this->vertexMap.end())
				{
					CGMath::Vertex tempVertex;
	
					tempVertex.pos = temp_vertices[pos];
					tempVertex.uv = temp_uvs[uv];
					tempVertex.norm = temp_normals[norm];
					
					this->vertexMap[key] = this->mesh.size();
					faceVertList.verts.push_back(this->mesh.size());
					this->mesh.push_back(tempVertex);
					
				}
				else
				{
					faceVertList.verts.push_back(this->vertexMap[key]);
				}
			}
			this->meshFaces.push_back(faceVertList);
		}
		for (auto i = 0; i < this->meshFaces.size(); ++i)
		{
			for (auto j = 1; j < this->meshFaces[i].verts.size() - 1; ++j)
			{
				this->indices.push_back(this->meshFaces[i].verts[0]);
				this->indices.push_back(this->meshFaces[i].verts[j]);
				this->indices.push_back(this->meshFaces[i].verts[j + 1]);
			}
		}
		
	}

	void MeshResources::createQuad()
	{
		GLuint ibuff[6]
		{
			0, 1, 3,
			1, 2, 3
		};

		Vertex tempVertex1;
		tempVertex1.pos[0] = -1.0f; tempVertex1.pos[1] = -1.0f; tempVertex1.pos[2] = -1.0f;
		tempVertex1.uv[0] = 0.0f; tempVertex1.uv[1] = 0.0f;
		tempVertex1.norm[0] = 0.0f; tempVertex1.norm[1] = 0.0f; tempVertex1.norm[2] = 0.0f;
		mesh.push_back(tempVertex1);

		Vertex tempVertex2;
		tempVertex2.pos[0] = -1.0f; tempVertex2.pos[1] = 1.0f; tempVertex2.pos[2] = -1.0f;
		tempVertex2.uv[0] = 0.0f; tempVertex2.uv[1] = 1.0f;
		tempVertex2.norm[0] = 0.0f; tempVertex2.norm[1] = 0.0f; tempVertex2.norm[2] = 0.0f;
		mesh.push_back(tempVertex2);

		Vertex tempVertex3;
		tempVertex3.pos[0] = 1.0f; tempVertex3.pos[1] = 1.0f; tempVertex3.pos[2] = -1.0f;
		tempVertex3.uv[0] = 1.0f; tempVertex3.uv[1] = 1.0f;
		tempVertex3.norm[0] = 0.0f; tempVertex3.norm[1] = 0.0f; tempVertex3.norm[2] = 0.0f;
		mesh.push_back(tempVertex3);

		Vertex tempVertex4;
		tempVertex4.pos[0] = 1.0f; tempVertex4.pos[1] = -1.0f; tempVertex4.pos[2] = -1.0f;
		tempVertex4.uv[0] = 1.0f; tempVertex4.uv[1] = 0.0f;
		tempVertex4.norm[0] = 0.0f; tempVertex4.norm[1] = 0.0f; tempVertex4.norm[2] = 0.0f;
		mesh.push_back(tempVertex4);
		
		for (int i = 0; i < 6; i++)
		{
			indices.push_back(ibuff[i]);
		}
		
		
	}

	void MeshResources::createQuad2()
	{
		GLuint ibuff[6]
		{
			0, 1, 3,
			1, 2, 3
		};

		Vertex tempVertex1;
		tempVertex1.pos[0] = 0.0f; tempVertex1.pos[1] = 0.0f; tempVertex1.pos[2] = 0.0f;
		tempVertex1.uv[0] = 0.0f; tempVertex1.uv[1] = 0.0f;
		tempVertex1.norm[0] = 0.0f; tempVertex1.norm[1] = 0.0f; tempVertex1.norm[2] = 0.0f;
		mesh.push_back(tempVertex1);

		Vertex tempVertex2;
		tempVertex2.pos[0] = 0.0f; tempVertex2.pos[1] = 1.0f; tempVertex2.pos[2] = 0.0f;
		tempVertex2.uv[0] = 0.0f; tempVertex2.uv[1] = 1.0f;
		tempVertex2.norm[0] = 0.0f; tempVertex2.norm[1] = 0.0f; tempVertex2.norm[2] = 0.0f;
		mesh.push_back(tempVertex2);

		Vertex tempVertex3;
		tempVertex3.pos[0] = 1.0f; tempVertex3.pos[1] = 1.0f; tempVertex3.pos[2] = 0.0f;
		tempVertex3.uv[0] = 1.0f; tempVertex3.uv[1] = 1.0f;
		tempVertex3.norm[0] = 0.0f; tempVertex3.norm[1] = 0.0f; tempVertex3.norm[2] = 0.0f;
		mesh.push_back(tempVertex3);

		Vertex tempVertex4;
		tempVertex4.pos[0] = 1.0f; tempVertex4.pos[1] = 0.0f; tempVertex4.pos[2] = 0.0f;
		tempVertex4.uv[0] = 1.0f; tempVertex4.uv[1] = 0.0f;
		tempVertex4.norm[0] = 0.0f; tempVertex4.norm[1] = 0.0f; tempVertex4.norm[2] = 0.0f;
		mesh.push_back(tempVertex4);

		for (int i = 0; i < 6; i++)
		{
			indices.push_back(ibuff[i]);
		}


	}

	void MeshResources::drawMesh()
	{
		glBindVertexArray(vao[0]);

		/*glBindBuffer(GL_ARRAY_BUFFER, ibo[0]);
		glDrawArrays(GL_TRIANGLES, 0, mesh.size());*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		
	}

	void MeshResources::drawSkinnedMesh()
	{
		glBindVertexArray(vao[0]);

		/*glBindBuffer(GL_ARRAY_BUFFER, ibo[0]);
		glDrawArrays(GL_TRIANGLES, 0, mesh.size());*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, NULL);

	}
	
	std::string MeshResources::FaceKey(int pos, int uv, int norm) const
	{
		std::string posString = std::to_string(pos);
		std::string uvString = std::to_string(uv);
		std::string normString = std::to_string(norm);
		
		std::string result = posString+uvString+normString;
		return result;	
	}
	
	std::vector< CGMath::Face >& MeshResources::GetMeshFaces()
	{
		return this->meshFaces;
	}


	std::vector<Vertex> MeshResources::GetMesh()
	{
		return this->mesh;
	}
	
	std::vector<GLuint> MeshResources::getIndices()
	{
		return this->indices;
	}


}