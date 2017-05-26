//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#pragma once
#include "core/app.h"
#include "render/window.h"
#include "GraphicsNode.h"
#include "LightNode.h"
#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "Rasterizer.h"
#include "Skeleton.h"
#include "Animations.h"
#include <chrono>

namespace Example
{
class CGLab : public Core::App
{
public:
	/// constructor
	CGLab();
	/// destructor
	~CGLab();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	CGMath::vector4D vec;
	GLfloat rotY = 0;
	GLfloat rotX = 0;
	GLfloat oldY = 0;
	GLfloat oldX = 0;
	GLfloat movSpeed = 0.08f;
	GLfloat scrollY = 0;
	GLfloat lightScroll = 0;
	GLfloat indexScroll = 0;
	GLboolean wFlag, sFlag, dFlag, aFlag, mLeftCheck, mRightCheck;
	Display::Window* window;
	CGMath::GraphicsNode gN;
	CGMath::LightNode lNode;
	CGMath::Skeleton skel;
	Animation::Animations anim;
	int animationIndex = 0;
	int keyIndex = 0;
	
	//GLuint shaderTransMatrix;
	

};
} // namespace Example