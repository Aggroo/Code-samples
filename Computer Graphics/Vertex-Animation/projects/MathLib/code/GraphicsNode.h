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
		~GraphicsNode();

		void setShaders(std::shared_ptr<ShaderObject> newShader);
		std::shared_ptr<ShaderObject> getShaders();

		void setMesh(std::shared_ptr<MeshResources> newMesh);
		std::shared_ptr<MeshResources> getMesh();

		void setTex(std::shared_ptr<TextureResource> newTex);
		std::shared_ptr<TextureResource> getTex();
		
		void setTransMat(matrix4D newTransMat);
		matrix4D getTransMat();

		void draw();
		void drawSkinned();
		
	private:
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<MeshResources> mesh;
		std::shared_ptr<TextureResource> tex;
		matrix4D transMat;
		

		
	};

}