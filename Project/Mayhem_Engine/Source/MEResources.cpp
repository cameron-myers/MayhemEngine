/*****************************************************************//**
 * \file         MEResources.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#define STB_IMAGE_IMPLEMENTATION 

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "MEResources.h"
#include "MESerializer.h"
#include "Log.h"
#include "OpenGLError.h"
#include <stb_image.h>

#include "SpriteSource.h"
/**
 * \brief creates a new shader
 * \param program shader to make
 * \param vertexShader filepath to the vertex shader file
 * \param pixelShader filepath to the pixel shader file
 * \return 
 */
bool CreateShader(GLuint* program, const char* vertexShader, const char* pixelShader)
{
	int noError;
	GLuint vertexStage, pixelStage = 0;
	vertexStage = glCreateShader(GL_VERTEX_SHADER);
	CheckError();
	std::string shaderString = MESerializer::OpenFileRead(vertexShader);
	const char* shaderText = shaderString.c_str();
	GLint textLength = (GLint)shaderString.size();

	glShaderSource(vertexStage, 1, &shaderText, &textLength);
	CheckError();
	glCompileShader(vertexStage);
	CheckError();
	glGetShaderiv(vertexStage, GL_COMPILE_STATUS, &noError);
	CheckError();
	if (!noError)
	{
		PrintShaderError(vertexStage);
		assert(false);
		return false;
	}

	pixelStage = glCreateShader(GL_FRAGMENT_SHADER);
	CheckError();
	std::string pixelString = MESerializer::OpenFileRead(pixelShader);
	const char* pixelText = pixelString.c_str();
	textLength = (GLint)pixelString.size();
	glShaderSource(pixelStage, 1, &pixelText, &textLength);
	CheckError();
	glCompileShader(pixelStage);
	CheckError();
	glGetShaderiv(pixelStage, GL_COMPILE_STATUS, &noError);
	CheckError();
	if (!noError)
	{
		PrintShaderError(pixelStage);
		assert(false);
		return false;
	}

	GLuint shaderID = glCreateProgram();
	CheckError();
	glAttachShader(shaderID, vertexStage);
	CheckError();
	glAttachShader(shaderID, pixelStage);
	CheckError();
	glLinkProgram(shaderID);
	CheckError();
	glGetProgramiv(shaderID, GL_LINK_STATUS, &noError);
	CheckError();
	if (!noError)
	{
		PrintShaderError(shaderID);
		assert(false);
		return false;
	}
	glDeleteShader(vertexStage);
	CheckError();
	glDeleteShader(pixelStage);
	CheckError();
	*program = shaderID;
	return true;
}

/**
 * \brief 
 * create the texture
 * \param texture 
 */
void CreateTexture(GLuint* texture, int width, int height)
{
	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_2D, *texture);

	//game resolution
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * \brief Creates and loads a new texture
 * \param texture 
 * \param filename path to the texture to load
 */
void CreateTextureImage(GLuint* texture, const char* filename, int* width, int* height)
{
	glGenTextures(1, texture);
	CheckError();
	glBindTexture(GL_TEXTURE_2D, *texture);
	CheckError();
	int pixelnum;

	unsigned char* imgData = stbi_load(filename, width,  height, &pixelnum, 4);
	if (!imgData) ME_CORE_ERROR("Could not load %s", filename);

	GLenum format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, imgData);
	CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	CheckError();
	glBindTexture(GL_TEXTURE_2D, 0);
	CheckError();
	//free the image data
	stbi_image_free(imgData);

}

/**
 * \brief Creates a new mesh
 * \param mesh 
 * \param meshVertexBuffer 
 * \param meshVertexCount 
 * \param xHalfSize 
 * \param yHalfSize 
 */
void CreateMesh(GLuint* mesh, GLuint* meshVertexBuffer, Vertex* vertices, unsigned vertexCount)
{
	glGenVertexArrays(1, mesh);
	CheckError();
	glBindVertexArray(*mesh);
	CheckError();
	glGenBuffers(1, meshVertexBuffer);
	CheckError();
	glBindBuffer(GL_ARRAY_BUFFER, *meshVertexBuffer);
	CheckError();
	//create the mesh
	

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices, GL_STATIC_DRAW);
	CheckError();


	GLint attributePos = 0;
	GLint attributeUV = 1;
	GLint attributeColor = 2;


	glVertexAttribPointer(attributePos, 2, GL_FLOAT, false, sizeof(Vertex), 0);
	CheckError();
	// Set the texture coordinate attribute (2 float values, offset by the size of a vec2)
	glVertexAttribPointer(attributeUV, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec2)));
	CheckError();
	glVertexAttribPointer(attributeColor, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec2) * 2));
	CheckError();
	glEnableVertexAttribArray(attributePos);
	CheckError();
	glEnableVertexAttribArray(attributeUV);
	CheckError();
	glEnableVertexAttribArray(attributeColor);
	CheckError();
	const GLuint unbind = 0;
	glBindVertexArray(unbind);
	CheckError();

}

/**
 * \brief Frees a given texture
 * \param texture texture to free
 */
void FreeTexture(const GLuint* texture)
{
	glDeleteTextures(1, texture);
	CheckError();
}

/**
 * \brief Frees a given shader
 * \param program shader to free
 */
void FreeShader(const GLuint* program)
{
	glDeleteProgram(*program);
	CheckError();
}

/**
 * \brief frees the mesh and the vertex buffer
 * \param mesh mesh to free
 * \param meshVertexBuffer vertex buffer to free
 */
void FreeMesh(const GLuint* mesh, const GLuint* meshVertexBuffer)
{
	if(*mesh != 0 && *meshVertexBuffer != 0)
	{
		glDeleteBuffers(1, meshVertexBuffer);
		CheckError();
		glDeleteVertexArrays(1, mesh);
		CheckError();
	}

}
