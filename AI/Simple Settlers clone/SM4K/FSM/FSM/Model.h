#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <vector>
class Model
{
private:
	//Original dimensions
	float width;
	float height;

	//If the model is a line or not
	bool line = false;

	//openGL variables
	GLuint vbo;
	GLuint vao;
	GLuint vertexSize;
	GLuint modelLocation;
	GLuint ColorLocation;
	GLuint textureLocation;
	GLuint shaderID;
	GLuint TextureID;
	GLuint UVBufferID;

	std::vector<glm::vec2> UVs;
public:
	Model(){}
	Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, GLuint nTextureID, GLuint nUVBufferID);
	Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, bool line);
	~Model();
	void update();
	void draw(const glm::mat4 &modelM, const glm::vec4 &color);
	void draw(const glm::mat4 &modelM, const glm::vec4 &color, bool line);
	
	void SetTexture(const char* filename);
	void init();

	void initLine(glm::vec2 startPos, glm::vec2 endPos);
	//
	bool getLine(){ return line; }

	//Dimension getters
	float getWidth() { return width; }
	float getHeight() { return height; }
};

