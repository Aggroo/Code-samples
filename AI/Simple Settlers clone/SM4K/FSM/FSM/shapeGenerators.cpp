#include "shapeGenerators.hpp"
#include "TextureLoader.h"

namespace shapeGenerator {

	Model square(float width, float height, GLuint shaderID, const char* filename) {
		GLfloat vertex_data[] = {
			0.0f, 0.0f,
			width, 0.0f,
			width, height,	//First triangle
			0.0f, 0.0f,
			width, height,
			0.0f, height	//Second triangle
		};

		// This will identify our vertex buffer
		GLuint vertexbuffer;

		GLuint textureID = TextureLoader::loadFile(filename);

		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		GLuint UVBuffer;
		glGenBuffers(1, &UVBuffer);

		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);

		// The following commands will talk about our 'vertexbuffer' buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		return Model(width, height, VertexArrayID, vertexbuffer, 6, shaderID, textureID, UVBuffer);
	}

	Model triangle(float base, float height, GLuint shaderID, const char* filename) {
		GLfloat vertex_data[] = {
			0.0f, height,
			base / 2, 0,
			base, height	//First triangle
		};

		// This will identify our vertex buffer
		GLuint vertexbuffer;

		GLuint textureID = TextureLoader::loadFile(filename);

		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		GLuint UVBuffer;
		glGenBuffers(1, &UVBuffer);

		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);

		// The following commands will talk about our 'vertexbuffer' buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		return Model(base, height, VertexArrayID, vertexbuffer, 3, shaderID, textureID, UVBuffer);
	}

	Model line(glm::vec2 startPos, glm::vec2 endPos, GLuint shaderID)
	{

		// This will identify our vertex buffer
		GLuint vertexbuffer;

		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);

		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		return Model(1, 1, VertexArrayID, vertexbuffer, 2, shaderID, true);
	}


}