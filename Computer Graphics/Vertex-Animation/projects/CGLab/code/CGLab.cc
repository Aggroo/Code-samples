//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "CGlab.h"
#include <cstring>
#include <chrono>
#include "matrix4D.h"
#include <assert.h>
#include <algorithm>
#include "quaternions.h"
#include "HalfEdgeMesh.h"
#include "Animations.h"
#include "Skinning.h"

//#define RASTERIZER
#define ANIMATION
//#define SUBDIVISION

using namespace CGMath;

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
CGLab::CGLab()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
CGLab::~CGLab()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CGLab::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		if (mLeftCheck == true)
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
		else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			this->window->Close();
		}
		else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.8f, 0.8f, 0.8f);
			lNode.setPos(50.0f, 80.0f, -80.0f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		{
			lNode.setIntensity(1.0f, 0.4f, 0.4f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.4f, 1.0f, 0.4f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.5f, 0.5f, 1.0f);
			lNode.apply();
		}
#ifdef ANIMATION
		else if(key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			animationIndex = 0;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			animationIndex = 1;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			animationIndex = 2;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_4 && action == GLFW_PRESS)
		{
			animationIndex = 3;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_5 && action == GLFW_PRESS)
		{
			animationIndex = 4;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_6 && action == GLFW_PRESS)
		{
			animationIndex = 5;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_7 && action == GLFW_PRESS)
		{
			animationIndex = 6;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
		else if(key == GLFW_KEY_8 && action == GLFW_PRESS)
		{
			animationIndex = 7;
			printf("Clip name: %s\n",anim.GetClipName(animationIndex).c_str());
		}
#endif

	});
	
	window->SetMouseMoveFunction([this](float64 x, float64 y)
	{
		if (mLeftCheck == true)
		{
			rotX += x - oldX;
			rotY += y - oldY;
		}
		if (mRightCheck == true)
		{
			lNode.setPos(x, -y, scrollY);
			lNode.apply();
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
		if (mRightCheck == true)
		{
			lightScroll += y;
			indexScroll += y;
			std::cout << indexScroll << std::endl;
			if(indexScroll > 20 || indexScroll < 0)
			{
				indexScroll = 0;
			}
		}
		
	});


	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		//Enable backface culling
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		
		return true;
	}
	return false;
}

/**
*/
	void
	CGLab::Run()
	{

		std::shared_ptr<CGMath::MeshResources> mesh = std::make_shared<CGMath::MeshResources>();
		std::shared_ptr<CGMath::MeshResources> mesh2 = std::make_shared<CGMath::MeshResources>();
		std::shared_ptr<CGMath::TextureResource> tex = std::make_shared<CGMath::TextureResource>();
		std::shared_ptr<CGMath::ShaderObject> shaders = std::make_shared<CGMath::ShaderObject>();

		vec = vector4D(0, 0, 0);
		Rasterizer rast;

		lNode.setShaders(shaders);

		lNode.setPos(30.0f, 80.0f, -80.0f);
		lNode.setIntensity(0.9f, 0.9f, 0.9f);
		lNode.setColour(0.1f, 0.1f, 0.1f);

		//mesh->createQuad();

#ifdef SUBDIVISION
        mesh2->loadObj("models/cube.obj");
        HalfEdges::HalfEdgeMesh heMesh(mesh2.get());
        heMesh.ConstructHalfEdgeMesh();
        heMesh.Subdivide(8);
        //exit(1);
        std::shared_ptr<CGMath::MeshResources> halfEdgeMesh(heMesh.HalfEdgeMeshToMesh());
		halfEdgeMesh->genBuffer();
		shaders->setupShaders("Blinn-phong.vert", "Blinn-phong.frag");
        gN.setMesh(halfEdgeMesh);
#endif

#ifdef ANIMATION
        Animation::Skinning skin;
        skin.LoadSkinMesh("Unit_Footman.nvx2");
        std::shared_ptr<CGMath::MeshResources> skinMesh(skin.GenSkinningMesh());

        shaders->setupShaders("Blinn-phong-Skinned.vert", "Blinn-phong-Skinned.frag");

        skel.CreateSkeleton("models/Unit_Footman.constants");
        anim.LoadAnimation("Unit_Footman.nax3");

        shaders->setupUniformInt("BoneCount", skel.jointIndex.size());

        gN.setMesh(skinMesh);
#endif

        gN.setShaders(shaders);
        lNode.apply();
        shaders->setupVector3f("cameraPosition", 0.0f, 0.0f, 0.0f);
        shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
        shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
        shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
        shaders->setupUniformFloat("u_matShininess", 64.0f);


        tex->LoadFromFile("textures/Footman_Diffuse.tga");

		vector4D vec1(-96, -86, 0);
		vector4D vec2(50, 71, 0);
		vector4D vec3(-56, 10, 0);


		vector4D vecRot1;
		vector4D vecRot2;
		vector4D vecRot3;

        tex->bindTex();
        std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

#ifdef RASTERIZER
        mesh2->createQuad2();
        rast.getMesh(mesh2);

		rast.getTexture("textures/sword7.jpg");

		auto vert = [](vector3D pos, vector3D norm, matrix4D nMat) -> vector3D
		{

			vector3D aColor(0.8f, 0.8f, 0.8f);
			vector3D ambientColor(0.05f,0.05f,0.05f);
			vector3D lightPos(-1000, 1000, 0);
			matrix3D mvMat = mvMat.fromMatrix4D(nMat);


			vector3D modelViewVertex = (mvMat*pos);
			vector3D modelViewNormal = (mvMat*norm);

			vector3D tempLightDir = (lightPos - modelViewVertex);
			vector3D lightVector = tempLightDir.vecNorm();

			float diffuse = std::max(modelViewNormal.dotProd(lightVector), 0.1f);
			float ambient = ambientColor.dotProd(vector3D(1.0f, 1.0f, 1.0f));

			vector3D v_Color = aColor*(diffuse+ambient);

			return v_Color;
		};

		rast.vertexShader(vert);

		auto frag = [](vector2D tex, vector3D norm, unsigned char* image, int w, int h, int n) -> Rasterizer::Color
		{

			int u = abs((int)(tex.x() * (w-1)) % w);
			int v = abs((int)(tex.y() * (h-1)) % h);

			int index = (u * n + v * n *w);

			Rasterizer::Color color;
			color.r = image[index];
			color.g = image[index + 1];
			color.b = image[index + 2];

			return color;
		};

		rast.fragment(frag);
#endif
        struct nk_context *ctx = this->window->ctx;
        struct nk_color background;
        background = nk_rgb(28,48,62);
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
			this->window->Update();


			matrix4D transMat;

			float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

			if (wFlag == true)
			{
				vec.y() += movSpeed;
			}
			if (sFlag == true)
			{
				vec.y() -= movSpeed;
			}
			if (dFlag == true)
			{
				vec.x() += movSpeed;
			}
			if (aFlag == true)
			{
				vec.x() -= movSpeed;
			}

			vec.z() = -5+ scrollY/3;
			transMat = matrix4D::perspectiveMatrix()*matrix4D::translationMatrix(vec)*matrix4D::rotY(rotX * 0.01)*matrix4D::rotX(rotY * 0.01)*matrix4D::vectorScaling(0.1f, 0.1f, 0.1f);


#if (defined SUBDIVISION || defined ANIMATION)
            shaders->setupMatrix4fv("VPmat", transMat);
#endif
			gN.setTex(tex);

            gN.setTransMat(matrix4D::vectorScaling(2.0f, 2.0f, 2.0f));

#ifdef SUBDIVISION
            gN.draw();
#endif

#ifdef RASTERIZER
            matrix4D tMat = matrix4D::translationMatrix(vec)*matrix4D::rotY(rotX * 0.01)*matrix4D::rotX(rotY * 0.01)*matrix4D::vectorScaling(0.1f, 0.1f, 0.1f);
            matrix4D pMat = matrix4D::perspectiveMatrix();
            rast.draw(pMat,tMat);

			tex->LoadFromRasterizer(rast);
            //gN.draw();
            //rast.clear();
#endif

#ifdef ANIMATION
            anim.PlayAnimation(animationIndex, keyIndex, &skel, timeStamp*25);

            std::vector<matrix4D> fixed;
            for(int i = 0; i < skel.jointIndex.size();++i)
            {
                fixed.push_back(skel.jointMatrices[skel.jointIndex[i]]);
            }

            shaders->setupMatrix4fv("BoneMatrixArray", fixed, fixed.size());
            gN.drawSkinned();


            {struct nk_panel layout;
                if (nk_begin(ctx, &layout, "CGLab", nk_rect(50, 50, 230, 260),
                             NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                             NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
                {
                    enum {EASY, HARD};
                    static int op = EASY;
                    static int property = 20;

                    nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
                    {
                        nk_layout_row_push(ctx, 70);
                        nk_label(ctx, "Skeleton:", NK_TEXT_LEFT);
                        nk_layout_row_push(ctx, 110);
                        nk_label(ctx, skel.GetSkelName().c_str(), NK_TEXT_LEFT);
                    }

                    nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
                    {
                        nk_layout_row_push(ctx, 80);
                        nk_label(ctx, "Animation:", NK_TEXT_LEFT);
                        nk_layout_row_push(ctx, 80);
                        nk_label(ctx, anim.GetClipName(animationIndex).c_str(), NK_TEXT_LEFT);
                    }

                    /*nk_layout_row_dynamic(ctx, 30, 2);
                    if (nk_option_label(ctx, "Subdivision", op == HARD)) op = HARD;
                    if (nk_option_label(ctx, "Animations", op == EASY)) op = EASY;*/

                    struct nk_panel combo;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_combo_begin_label(ctx, &combo, "Animations", nk_vec2(nk_widget_width(ctx),400)))
                    {
                        nk_layout_row_static(ctx, 30, 80, 2);
                        for(int i = 0; i < anim.GetClipAmount(); ++i)
                        {
                            if (nk_button_label(ctx, anim.GetClipName(i).c_str()))
                            {
                                animationIndex = i;
                                //fprintf(stdout, "button pressed\n");
                            }
                        }
                        nk_combo_end(ctx);
                    }

                    /*nk_layout_row_dynamic(ctx, 25, 1);
                    nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

                    {struct nk_panel combo;
                        nk_layout_row_dynamic(ctx, 20, 1);
                        nk_label(ctx, "background:", NK_TEXT_LEFT);
                        nk_layout_row_dynamic(ctx, 25, 1);
                        if (nk_combo_begin_color(ctx, &combo, background, nk_vec2(nk_widget_width(ctx),400))) {
                            nk_layout_row_dynamic(ctx, 120, 1);
                            background = nk_color_picker(ctx, background, NK_RGBA);
                            nk_layout_row_dynamic(ctx, 25, 1);
                            background.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, background.r, 255, 1,1);
                            background.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, background.g, 255, 1,1);
                            background.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, background.b, 255, 1,1);
                            background.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, background.a, 255, 1,1);
                            nk_combo_end(ctx);
                        }}*/
                }
                nk_end(ctx);}
#endif
            glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();
		}
	}
//------------------------------------------------------------------------------

} // namespace Example