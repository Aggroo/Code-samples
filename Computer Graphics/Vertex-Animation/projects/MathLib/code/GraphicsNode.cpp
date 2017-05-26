#include "GraphicsNode.h"
#include <iostream>
#include <fstream>
#include <string>


namespace CGMath
{
	GraphicsNode::GraphicsNode()
	{

	}

	GraphicsNode::~GraphicsNode()
	{
		
	}


	void GraphicsNode::setShaders(std::shared_ptr<ShaderObject> newShader)
	{
		this->shader = newShader;
	}

	std::shared_ptr<ShaderObject> GraphicsNode::getShaders()
	{
		return this->shader;
	}


	void GraphicsNode::setMesh(std::shared_ptr<MeshResources> newMesh)
	{
		this->mesh = newMesh;
	}

	std::shared_ptr<MeshResources> GraphicsNode::getMesh()
	{
		return this->mesh;
	}

	void GraphicsNode::setTex(std::shared_ptr<TextureResource> newTex)
	{
		this->tex = newTex;
	}

	std::shared_ptr<TextureResource> GraphicsNode::getTex()
	{
		return this->tex;
	}

	void GraphicsNode::setTransMat(matrix4D newTransMat)
	{
		this->transMat = newTransMat;
	}

	CGMath::matrix4D GraphicsNode::getTransMat()
	{
		return this->transMat;
	}


	void GraphicsNode::draw()
	{
		this->shader->useProgram();
		this->tex->bindTex();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->mesh->drawMesh();
	}

	void GraphicsNode::drawSkinned()
	{
		this->shader->useProgram();
		this->tex->bindTex();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->mesh->drawSkinnedMesh();
	}

}