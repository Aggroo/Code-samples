#include "Camera2D.h"

Camera2D* Camera2D::Instance()
{
	static Camera2D instance;
	return &instance;
}

void Camera2D::ZoomCamera(const float& amount)
{
	zoomScale += amount * zoomStep;
	zoomScale = glm::clamp(zoomScale, 0.1f, 10.0f);
	projectionMatrix = glm::ortho(-windowWidth*zoomScale*0.5f, windowWidth*zoomScale*0.5f, windowHeight*zoomScale*0.5f, -windowHeight*zoomScale*0.5f);
}

void Camera2D::MoveCamera(const int& x, const int& y)
{
	pos.x = pos.x + x * moveSpeed;
	pos.y = pos.y + (y * moveSpeed);

	viewMatrix = glm::translate(glm::vec3(pos.x, pos.y, 0.0f));
}

void Camera2D::SetCameraPos(glm::vec2 val)
{
	pos = -val;
	viewMatrix = glm::translate(glm::vec3(-val.x, -val.y, 0.0f));
}

