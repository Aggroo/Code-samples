#pragma once
//GLEW
#include "GL\glew.h"
//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtx\transform.hpp>

#define Camera Camera2D::Instance()

/************************************************************************/
/*						 Camera2D - Singleton							*/
/*				Handles the camera movement of the program				*/
/************************************************************************/
class Camera2D
{
public:

	static Camera2D* Instance();

	void ZoomCamera(const float& amount);
	void MoveCamera(const int& x, const int& y);

	float GetWindowHeight() const { return windowHeight; }
	void SetWindowHeight(float val) { windowHeight = val; }

	float GetWindowWidth() const { return windowWidth; }
	void SetWindowWidth(float val) { windowWidth = val; }

	float GetZoomScale() const { return zoomScale; }
	void SetZoomScale(float val) { zoomScale = val; }

	float GetOffsetX() const { return offsetX; }
	void SetOffsetX(float val) { offsetX = val; }

	float GetOffsetY() const { return offsetY; }
	void SetOffsetY(float val) { offsetY = val; }
	
	float GetMoveSpeed() const { return moveSpeed; }
	void SetMoveSpeed(float val) { moveSpeed = val; }
	
	float GetZoomStep() const { return zoomStep; }
	void SetZoomStep(float val) { zoomStep = val; }

	const glm::mat4& GetViewMatrix() const { return viewMatrix; }
	void SetViewMatrix(glm::mat4 val) { viewMatrix = val; }

	const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }
	void SetProjectionMatrix(glm::mat4 val) { projectionMatrix = val; }
	
	glm::vec2 GetPos() const { return pos; }
	void SetPos(glm::vec2 val) { pos = val; }

	void SetCameraPos(glm::vec2 val);
private:

	Camera2D(){}
	Camera2D(const Camera2D&) = delete;
	Camera2D& operator=(const Camera2D&) = delete;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec2 pos;

	float windowHeight;
	float windowWidth;

	float zoomScale = 1;
	float offsetX, offsetY;
	float moveSpeed = 10.0f;
	float zoomStep = 0.1f;
};

