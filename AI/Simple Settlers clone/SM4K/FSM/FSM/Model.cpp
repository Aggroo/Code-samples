#include "Model.h"
#include "TextureLoader.h"


Model::Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, GLuint nTextureID, GLuint nUVBufferID)
{
	this->width = width;
	this->height = height;
	this->vao = vao;
	this->vbo = vbo;
	this->vertexSize = vertexSize;
	this->shaderID = shaderID;
	this->TextureID = nTextureID;
	this->UVBufferID = nUVBufferID;
	this->modelLocation = glGetUniformLocation(shaderID, "ModelMatrix");
	this->ColorLocation = glGetUniformLocation(shaderID, "extraColor");
	this->textureLocation = glGetUniformLocation(shaderID, "myTextureSampler");

	glm::vec2 uv_up_left = glm::vec2(1.0f, 1.0f);
	glm::vec2 uv_up_right = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv_down_right = glm::vec2(0.0f, 0.0f);
	glm::vec2 uv_down_left = glm::vec2(0.0f, 1.0f);


	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_left);
	UVs.push_back(uv_up_right);

	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_right);
	UVs.push_back(uv_down_right);
}

Model::Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, bool line)
{
	this->width = width;
	this->height = height;
	this->vao = vao;
	this->vbo = vbo;
	this->vertexSize = vertexSize;
	this->shaderID = shaderID;
	this->modelLocation = glGetUniformLocation(shaderID, "ModelMatrix");
	this->ColorLocation = glGetUniformLocation(shaderID, "extraColor");
	this->textureLocation = glGetUniformLocation(shaderID, "myTextureSampler");
	this->line = line;

	glm::vec2 uv_up_left = glm::vec2(1.0f, 1.0f);
	glm::vec2 uv_up_right = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv_down_right = glm::vec2(0.0f, 0.0f);
	glm::vec2 uv_down_left = glm::vec2(0.0f, 1.0f);


	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_left);
	UVs.push_back(uv_up_right);

	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_right);
	UVs.push_back(uv_down_right);
}

Model::~Model()
{
}

void Model::update()
{
}

void Model::draw(const glm::mat4 &modelM, const glm::vec4 &color)
{
	glUseProgram(shaderID);

	//Send Matrix to Shader
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelM[0][0]);
	//Send Color to Shader
	glUniform4fv(ColorLocation, 1, &color[0]);
	glUniform1i(glGetUniformLocation(shaderID, "line"), 0);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureLocation, 0);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertexSize); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw(const glm::mat4 &modelM, const glm::vec4 &color, bool line)
{
	glUseProgram(shaderID);

	//Send Matrix to Shader
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelM[0][0]);
	glUniform4fv(ColorLocation, 1, &color[0]);
	glUniform1i(glGetUniformLocation(shaderID, "line"), 1);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Draw the line!
	glDrawArrays(GL_LINE_STRIP, 0, vertexSize); //Starting from vertex 0; 3 vertices total -> 1 triangle

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::SetTexture(const char* filename)
{
	TextureID = TextureLoader::loadFile(filename);
}

void Model::init()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Model::initLine(glm::vec2 startPos, glm::vec2 endPos)
{
	GLfloat vertex_data[] = {
		startPos.x, startPos.y,
		endPos.x, endPos.y
	};

	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

}

