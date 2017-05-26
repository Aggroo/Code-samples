#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "PhysicsDevice.h"
#include "LightNode.h"
#include "Ray.h"
#include "OABB.h"

namespace Example
{
class PhysicsLab : public Core::App
{
public:
	/// constructor
	PhysicsLab();
	/// destructor
	~PhysicsLab();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	/// compile shaders
	void CompileShaders();

	/// show some ui things
	void RenderUI();

	// show some nanovg stuff
	void RenderNano(NVGcontext * vg);

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;

	Display::Window* window;
	GLchar *vsBuffer;
	GLchar *fsBuffer;
	std::string compilerLog;

    //Camera movement
    GLfloat rotY = 0;
    GLfloat rotX = 0;
    GLfloat oldY = 0;
    GLfloat oldX = 0;
    GLfloat scrollY = 0;
    GLfloat moveSpeed = 0.1f;
    GLboolean wFlag, sFlag, dFlag, aFlag, mLeftCheck = false, mRightCheck = false;

    float mouseX, mouseY;

    std::shared_ptr<CGMath::ShaderObject> shaders;

    CGMath::vector4D vec;
    CGMath::LightNode lNode;

	Physics::OABB bb;

	std::vector<Physics::Ray> rays;
	std::vector<Physics::RayHit> hits;

    Physics::PhysicsDevice pd;
};
} // namespace Example