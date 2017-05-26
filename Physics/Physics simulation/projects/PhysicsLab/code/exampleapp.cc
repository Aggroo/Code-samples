//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <chrono>
#include "imgui.h"


#define STRING_BUFFER_SIZE 8192

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
using namespace CGMath;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
PhysicsLab::PhysicsLab() : vertexShader(0), pixelShader(0), program(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
PhysicsLab::~PhysicsLab()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
PhysicsLab::Open()
{
	App::Open();
	this->window = new Display::Window;

    window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
    {
        if (mRightCheck == true)
        {
            if (key == GLFW_KEY_W && action == GLFW_PRESS)
            {
                wFlag = true;
            }

            else if (key == GLFW_KEY_S && action == GLFW_PRESS)
            {
                sFlag = true;
            }

            else if (key == GLFW_KEY_D && action == GLFW_PRESS)
            {
                dFlag = true;
            }

            else if (key == GLFW_KEY_A && action == GLFW_PRESS)
            {
                aFlag = true;
            }

        }
        if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        {
            wFlag = false;
        }
        else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        {
            sFlag = false;
        }
        else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        {
            dFlag = false;
        }
        else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        {
            aFlag = false;
        }
        else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
        {
            Physics::RigidBody::integrate = Physics::Integrators::Euler;
            printf("Euler integrator\n");
        }
        else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
        {
            Physics::RigidBody::integrate = Physics::Integrators::Midpoint;
            printf("Midpoint integrator\n");
        }
        else if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
        {
            Physics::RigidBody::integrate = Physics::Integrators::RK4;
            printf("RK4 integrator\n");
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            this->window->Close();
        }
        else if(key == GLFW_KEY_U && action == GLFW_PRESS)
        {
            rays.clear();
        }
        else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    });

    window->SetMouseMoveFunction([this](float64 x, float64 y)
     {
         if (mRightCheck)
         {
             rotX += x - oldX;
             rotY += y - oldY;
         }

         oldX = x;
         oldY = y;

     });

    window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
    {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
      {
          mLeftCheck = true;
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
      {
          mLeftCheck = false;
      }
      else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
      {
          mRightCheck = true;
      }
      else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
      {
          mRightCheck = false;
      }


    });

    window->SetMouseScrollFunction([this](float64 x, float64 y)
   {
       if (mLeftCheck == true)
       {
           scrollY += y;
       }

   });



	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// create buffer for shader strings
		this->vsBuffer = new GLchar[STRING_BUFFER_SIZE];
		this->fsBuffer = new GLchar[STRING_BUFFER_SIZE];

		// copy the hardcoded shader into buffer
		strncpy(this->vsBuffer, vs, STRING_BUFFER_SIZE);
		strncpy(this->fsBuffer, ps, STRING_BUFFER_SIZE);

		// compile the shaders in the buffers
		//this->CompileShaders();

        glEnable(GL_DEPTH_TEST);
        //Enable backface culling
        //glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CCW);
        //glCullFace(GL_BACK);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);


		// set ui rendering function
		/*this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});

		this->window->SetNanoVGRender([this](NVGcontext * vg)
		{
			this->RenderNano(vg);
		});*/
		return true;
	}
	return false;
}




//------------------------------------------------------------------------------
/**
*/
void
PhysicsLab::Run()
{

    Physics::PhysicsNode pN, pN2, pN3;
    std::shared_ptr<CGMath::MeshResource> mesh = std::make_shared<CGMath::MeshResource>();
    std::shared_ptr<CGMath::MeshResource> mesh2 = std::make_shared<CGMath::MeshResource>();
    std::shared_ptr<CGMath::TextureResource> tex = std::make_shared<CGMath::TextureResource>();
    std::shared_ptr<CGMath::TextureResource> tex2 = std::make_shared<CGMath::TextureResource>();
    shaders = std::make_shared<CGMath::ShaderObject>();

    vec = vector4D(0, 0, -4);

    matrix4D modelMat;

    mesh->loadObj("models/sphere.obj");
    mesh2->loadObj("models/cube.obj");

    modelMat = matrix4D::translationMatrix(vector4D(1.1f,1.0f,0.0f));
    matrix4D modelMat2 = matrix4D::translationMatrix(vector4D(0,-3,0));

    matrix4D mat = matrix4D::identity();
    bb.SetUp(*mesh2.get(), mat);

    mesh2->genBuffer();

    shaders->setupShaders("Blinn-phong.vert", "Blinn-phong.frag");

    lNode.setShaders(shaders);

    lNode.setPos(30.0f, 80.0f, -80.0f);
    lNode.setIntensity(0.9f, 0.9f, 0.9f);
    lNode.setColour(0.1f, 0.1f, 0.1f);

    lNode.apply();

    shaders->setupVector3f("cameraPosition", 0.0f, 0.0f, 0.0f);
    shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupUniformFloat("u_matShininess", 64.0f);
    shaders->setupMatrix4fv("transMatrix", matrix4D::identity());

    tex->LoadFromFile("textures/mesh.jpg");
    tex->bindTex();

    tex2->LoadFromFile("textures/cat_diff.tga");
    tex2->bindTex();

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();

    float deltaTime = 0.0f;
    float timestep = 0.016f;

    pN.setMesh(mesh2);
    pN.setShaders(shaders);
    pN.setTex(tex);
    pN.GetOabb()->SetUp(*mesh2.get(), mat);
    pN.GetAabb()->SetUp(pN.GetOabb()->ToMesh());
    pN.GetAabb()->SetOwner(&pN);
    pN.GetBody().Initialize(10.0f, pN.GetAabb(), mat, false);
    pd.RegisterPhysicsNode(&pN);

    pN2.setMesh(mesh2);
    pN2.setShaders(shaders);
    pN2.setTex(tex2);
    pN2.GetOabb()->SetUp(*mesh2.get(), mat);
    pN2.GetAabb()->SetUp(pN2.GetOabb()->ToMesh());
    pN2.GetAabb()->SetOwner(&pN2);
    pN2.GetBody().Initialize(10.0f, pN2.GetAabb(), modelMat, false);
    pd.RegisterPhysicsNode(&pN2);

    pN3.setMesh(mesh2);
    pN3.setShaders(shaders);
    pN3.setTex(tex2);
    pN3.GetOabb()->SetUp(*mesh2.get(), mat);
    pN3.GetAabb()->SetUp(pN3.GetOabb()->ToMesh());
    pN3.GetAabb()->SetOwner(&pN3);
    pN3.GetBody().Initialize(1000.0f, pN3.GetAabb(), modelMat2, vector4D(30.0f,1.0f,30.0f), true);
    pd.RegisterPhysicsNode(&pN3);

	while (this->window->IsOpen())
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        deltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastFrame).count() / 1000.0f);

        vector4D movement;

		this->window->Update();
        matrix4D transMat;

        if (wFlag == true)
        {
            movement[2] += moveSpeed;
        }
        if (sFlag == true)
        {
            movement[2] -= moveSpeed;
        }
        if (dFlag == true)
        {
            movement[0] -= moveSpeed;
        }
        if (aFlag == true)
        {
            movement[0] += moveSpeed;
        }
        mouseX = (oldX*2.0f)/1024 - 1;
        mouseY = 1 - (oldY*2.0f)/768;

        matrix4D rotx = matrix4D::rotX(-rotY*0.001f);
        matrix4D roty = matrix4D::rotY(rotX*0.001f);

        matrix4D rotation = (roty*rotx);

        const vector4D left = rotation.GetXAxis();
        const vector4D up = rotation.GetYAxis();
        const vector4D forward = rotation.GetZAxis();

        vec = vec + (rotation * movement);

        pN.SetProjectionMat(matrix4D::perspectiveMatrix());
        pN.SetViewMat(matrix4D::LookAt(vec, vec+forward, up));

        pN2.SetProjectionMat(matrix4D::perspectiveMatrix());
        pN2.SetViewMat(matrix4D::LookAt(vec, vec+forward, up));

        pN3.SetProjectionMat(matrix4D::perspectiveMatrix());
        pN3.SetViewMat(matrix4D::LookAt(vec, vec+forward, up));

        vector3D camPos(pN.GetViewMat()[3],pN.GetViewMat()[7],pN.GetViewMat()[11]);

        shaders->setupVector3f("cameraPosition", camPos);

        if(mLeftCheck)
        {
            Physics::RayHit hit;

            bool isHit = false;
            Physics::Ray ray = Physics::Ray(matrix4D::LookAt(vec, vec+forward, up), matrix4D::perspectiveMatrix(), CGMath::vector4D(mouseX, mouseY, -1.0f));
            rays.push_back(ray);

            for(int i = 0; i < pd.GetPhysicsNodes().size(); i++)
            {
                MeshResource m = pd.GetPhysicsNodes()[i]->GetOabb()->ToMesh();
                matrix4D imat = matrix4D::identity();

                if(ray.CollisionCheck(m, hit, imat))
                {
                    pd.GetPhysicsNodes()[i]->GetBody().AddForceAtPoint(ray.GetDir() * 5.0f, hit.contact);
                    hits.push_back(hit);
                    isHit = true;
                    break;
                }

            }
            if(!isHit)
            {
                hit.contact = ray.GetStartPoint()+(ray.GetDir()*100.0f);
                hits.push_back(hit);
            }

        }

        glUseProgram(0);
        matrix4D view = !pN.GetViewMat();
        matrix4D proj = !pN.GetProjectionMat();
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((GLfloat*)&view[0]);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((GLfloat*)&proj[0]);

        glBegin(GL_LINES);
        for (int j = 0; j < rays.size(); ++j)
        {
            vector4D v1 = rays[j].GetStartPoint();
            vector4D v2 = (hits[j].contact);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex4f(v1[0], v1[1], v1[2], v1[3]);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex4f(v2[0], v2[1], v2[2], v2[3]);

        }
        glEnd();

        pN.GetOabb()->UpdateTransform(pN.GetBody().GetTransform());
        pN.GetOabb()->DrawBoundingBox();
        pN.GetAabb()->SetUp(pN.GetOabb()->ToMesh());
        pN.GetAabb()->DrawBoundingBox();

        pN2.GetOabb()->UpdateTransform(pN2.GetBody().GetTransform());
        pN2.GetOabb()->DrawBoundingBox();
        pN2.GetAabb()->SetUp(pN2.GetOabb()->ToMesh());
        pN2.GetAabb()->DrawBoundingBox();

        pN3.GetOabb()->UpdateTransform(pN3.GetBody().GetTransform());
        pN3.GetOabb()->DrawBoundingBox();
        pN3.GetAabb()->SetUp(pN3.GetOabb()->ToMesh());
        pN3.GetAabb()->DrawBoundingBox();

        Physics::EPAInfo collisionInfo;

		//Start collision check
        if(pd.PlaneSweep())
        {
            for (int i = 0; i < pd.GetNarrowPhaseArray().size(); ++i)
            {
                vector4D vec(0,0,1);
                if(pd.Intersection(*pd.GetNarrowPhaseArray()[i].first->GetOwner(), *pd.GetNarrowPhaseArray()[i].second->GetOwner(), vec))
                {
                    pd.CollisionInformation(*pd.GetNarrowPhaseArray()[i].first->GetOwner(), *pd.GetNarrowPhaseArray()[i].second->GetOwner(), collisionInfo);
                    CGMath::vector4D deltaPa = pd.GetNarrowPhaseArray()[i].first->GetOwner()->GetBody().CalculateDeltaCollisionPoint(collisionInfo.contactPoint);
                    CGMath::vector4D deltaPb = pd.GetNarrowPhaseArray()[i].second->GetOwner()->GetBody().CalculateDeltaCollisionPoint(collisionInfo.contactPoint);

                    float relVel = vector4D::Dot(collisionInfo.contactNormal,(deltaPa-deltaPb));
                    if(relVel < 0)
                    {
                        float impulse = pd.CalculateImpulse(*pd.GetNarrowPhaseArray()[i].first->GetOwner(), *pd.GetNarrowPhaseArray()[i].second->GetOwner(),relVel, collisionInfo);

                        Physics::RigidBody* a = &pd.GetNarrowPhaseArray()[i].first->GetOwner()->GetBody();
                        Physics::RigidBody* b = &pd.GetNarrowPhaseArray()[i].second->GetOwner()->GetBody();

                        a->GetState().position += collisionInfo.contactNormal*collisionInfo.depth;
                        b->GetState().position += collisionInfo.contactNormal.Negative()*collisionInfo.depth;

                        vector4D jDirA = collisionInfo.contactNormal*impulse;
                        vector4D jDirB = collisionInfo.contactNormal.Negative()*impulse;

                        a->SetVelocity(a->GetVelocity()+(jDirA*a->inverseMass));
                        a->SetRotation(a->GetRotation()+(a->GetInverseInertiaTensorWorld()*vector4D::Cross((collisionInfo.contactPoint-a->centerOfMass),jDirA)));
                        b->SetVelocity(b->GetVelocity()+(jDirB*b->inverseMass));
                        b->SetRotation(b->GetRotation()+(b->GetInverseInertiaTensorWorld()*vector4D::Cross((collisionInfo.contactPoint-b->centerOfMass),jDirB)));
                    }
                }
            }
        }

        for (int i = 0; i < pd.GetPhysicsNodes().size(); ++i)
        {
            pd.GetPhysicsNodes()[i]->GetBody().Update(timestep);
            pd.GetPhysicsNodes()[i]->setTransMat(pd.GetPhysicsNodes()[i]->GetBody().GetTransform());
            pd.GetPhysicsNodes()[i]->draw();
        }


        lastFrame = std::chrono::high_resolution_clock::now();
		// transfer new frame to window
		this->window->SwapBuffers();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsLab::RenderUI()
{
	if (this->window->IsOpen())
	{
		bool show = true;
		// create a new window
		ImGui::Begin("Shader Sources", &show, ImGuiWindowFlags_NoSavedSettings);

		// create text editors for shader code
		ImGui::InputTextMultiline("Vertex Shader", shaders->GetVertexShader(), STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
			ImGuiInputTextFlags_AllowTabInput);
		
		ImGui::InputTextMultiline("Pixel Shader", shaders->GetFragmentShader(), STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
			ImGuiInputTextFlags_AllowTabInput);

		// apply button
		if (ImGui::Button("Apply"))
		{
			// if pressed we compile the shaders
			//this->CompileShaders();
		}
		if (this->compilerLog.length())
		{
			// if compilation produced any output we display it here
			ImGui::TextWrapped(this->compilerLog.c_str());
		}
		// close window
		ImGui::End();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsLab::CompileShaders()
{
	// check if shader already exists
	if (this->program)
	{
		glDeleteProgram(this->program);
		this->program = 0;
	}
	if (this->vertexShader)
	{
		glDeleteShader(this->vertexShader);
		this->vertexShader = 0;
	}
	if (this->pixelShader)
	{
		glDeleteShader(this->pixelShader);
		this->pixelShader = 0;
	}

	this->compilerLog.clear();

    this->vsBuffer = shaders->GetVertexShader();

	// setup vertex shader
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = std::strlen(this->vsBuffer);
	glShaderSource(this->vertexShader, 1, &this->vsBuffer, &length);
	glCompileShader(this->vertexShader);

	// get error log
	GLint shaderLogSize;
	glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// setup pixel shader
    this->fsBuffer = shaders->GetFragmentShader();
	this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	length = std::strlen(this->fsBuffer);
	glShaderSource(this->pixelShader, 1, &this->fsBuffer, &length);
	glCompileShader(this->pixelShader);

	// get error log
	shaderLogSize;
	glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[PIXEL SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// create a program object
	this->program = glCreateProgram();
	glAttachShader(this->program, this->vertexShader);
	glAttachShader(this->program, this->pixelShader);
	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsLab::RenderNano(NVGcontext * vg)
{
	
	
	nvgSave(vg);

	nvgBeginPath(vg);
	//nvgCircle(vg,600, 100, 50);
	NVGpaint paint;
	paint = nvgLinearGradient(vg, 600, 100, 650, 150, nvgRGBA(255, 0, 0, 255), nvgRGBA(0, 255, 0, 255));
	nvgFillPaint(vg, paint);
	nvgFill(vg);
	

	nvgRestore(vg);
}

} // namespace Example