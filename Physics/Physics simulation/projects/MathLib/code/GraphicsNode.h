#pragma once
#include "GL/glew.h"
#include "config.h"
#include "matrix4D.h"
#include "ShaderObject.h"
#include "TextureResource.h"
#include "MeshResource.h"

#include <memory>

namespace CGMath
{
	
	class GraphicsNode
	{
	public:
		GraphicsNode();
		GraphicsNode(std::shared_ptr<MeshResource> newMesh, std::shared_ptr<TextureResource> newTex, std::shared_ptr<ShaderObject> newShader);
		~GraphicsNode();

		void setShaders(std::shared_ptr<ShaderObject> newShader);
		std::shared_ptr<ShaderObject> getShaders();

		void setMesh(std::shared_ptr<MeshResource> newMesh);
		std::shared_ptr<MeshResource> getMesh();

		void setTex(std::shared_ptr<TextureResource> newTex);
		std::shared_ptr<TextureResource> getTex();
		
		void setTransMat(matrix4D newTransMat);
		matrix4D getTransMat();

		void SetViewMat(matrix4D newViewMat);
		matrix4D GetViewMat();

		void SetProjectionMat(matrix4D newProjectionMat);
		matrix4D GetProjectionMat();

		void draw();
		void drawSkinned();

	protected:
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> tex;
		matrix4D transMat;
		matrix4D viewMat;
		matrix4D projMat;
		

		
	};

}