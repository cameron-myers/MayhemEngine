/*****************************************************************//**
 * \file         MERendering.cpp
 * \author       Cameron Myers, Nate White
 * \par          Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include <filesystem>
#include <glad/glad.h>
#include <MERendering.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/transform.hpp>

#include "Engine.h"
#include "Log.h"
#include "OpenGLError.h"
#include "MEEditor.h"
#include "MEResources.h"
#include "mesh.h"
#include "SpriteSource.h"
#include "Transform.h"

GLuint objectShaderID = 0;

GLuint viewportShaderID = 0;
GLuint viewportTextureID = 0;
GLuint viewportBuffer = 0;
GLuint tempBuffer = 0;
GLuint tempTextureID = 0;
GLuint viewportVAO = 0;
GLuint viewportVBO = 0;
float editorToggle = 0.5f;
glm::mat4 vTransformMatrix;


static GLFWwindow* windowPtr = nullptr;
/**
 * \brief Initializes the renderer
 * \param windowHandle pointer to the window
 */
void InitRenderer(GLFWwindow* windowHandle)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	CheckError();
	glDisable(GL_CULL_FACE);
	CheckError();


	//create the shader for sprites

#ifndef _DISTRIBUTE

	CreateShader(&objectShaderID, Engine::MAYHEM_DIR("\\Assets\\VShader.shader").c_str(), Engine::MAYHEM_DIR("\\Assets\\PShaderTexture.shader").c_str());

	CreateShader(&viewportShaderID, Engine::MAYHEM_DIR("\\Assets\\ViewportShader.shader").c_str(), Engine::MAYHEM_DIR("\\Assets\\ViewportPixelShader.shader").c_str());
	ME_CORE_INFO("shaders created")

#endif // _DEBUG



#ifdef _DISTRIBUTE

	CreateShader(&objectShaderID, "./Assets/VShader.shader", "./Assets/PShaderTexture.shader");

	CreateShader(&viewportShaderID, "./Assets/ViewportShader.shader", "./Assets/ViewportPixelShader.shader");

#endif // _DISTRIBUTE


	//////////////////////////////////////////////////////////////////////////////////////////////
	///VIEWPORT MESH
	float viewportVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &viewportVAO);
	CheckError();
	glGenBuffers(1, &viewportVBO);
	CheckError();
	glBindVertexArray(viewportVAO);
	CheckError();
	glBindBuffer(GL_ARRAY_BUFFER, viewportVBO);
	CheckError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(viewportVertices), &viewportVertices, GL_STATIC_DRAW);
	CheckError();
	glEnableVertexAttribArray(0);
	CheckError();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	CheckError();
	glEnableVertexAttribArray(1);
	CheckError();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	CheckError();
	///////////////////////////////////////////////////////////////////////

	//configure the viewportshader;
	glUseProgram(viewportShaderID);
	glUniform1i(glGetUniformLocation(viewportShaderID, "viewportTextureID"), 0);

	//create the framebuffer for the viewport
	glGenFramebuffers(1, &viewportBuffer);
	CheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, viewportBuffer);
	CheckError();

	CreateTexture(&viewportTextureID, MEWindow::GetWidth(), MEWindow::GetHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewportTextureID, 0);
	CheckError();

	//add depth attachment here

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ME_CORE_ERROR("FRAMEBUFFER NOT COMPLETE");
	}


	//temp buffer
	glGenFramebuffers(1, &tempBuffer);
	CheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, tempBuffer);
	CheckError();
	CreateTexture(&tempTextureID, 512,512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTextureID, 0);
	CheckError();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ME_CORE_ERROR("FRAMEBUFFER NOT COMPLETE");
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CheckError();





	glfwSetWindowSizeCallback(windowHandle, ResizeWindow);

	ResizeWindowInit(windowHandle, MEWindow::GetWidth(), MEWindow::GetHeight());

	windowPtr = windowHandle;
}

/**
 * \brief starts the render frame
 */
void StartRenderFrame()
{
	//frame buffer texture render size
	glBindFramebuffer(GL_FRAMEBUFFER, viewportBuffer);
	CheckError();
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	CheckError();
	glClear(GL_COLOR_BUFFER_BIT);
	CheckError();


	glUseProgram(viewportShaderID);
	glUniform1i(glGetUniformLocation(viewportShaderID, "viewportTextureID"), 0);
	//add depth stuff later
	GLuint transformLoc = glGetUniformLocation(viewportShaderID, "world");
	CheckError();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vTransformMatrix));
	CheckError();


}

/**
 * \brief 
 * present the buffered frame
 */
void PresentFrame()
{

	glUseProgram(viewportShaderID);
	CheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CheckError();
	glClearColor(0.5f, 0.0f, 1.f, 1.f);
	CheckError();
	glClear(GL_COLOR_BUFFER_BIT);
	CheckError();
	glUseProgram(viewportShaderID);
	CheckError();

	//move viewport
	GLuint editorTog = glGetUniformLocation(viewportShaderID, "editorToggle");
	CheckError();
	glUniform2f(editorTog, editorToggle, editorToggle);

	glBindVertexArray(viewportVAO);
	CheckError();
	glBindTexture(GL_TEXTURE_2D, viewportTextureID);
	CheckError();

	GLuint transformLoc = glGetUniformLocation(viewportShaderID, "world");
	CheckError();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vTransformMatrix));
	CheckError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	CheckError();
	glUseProgram(0);



	glfwSwapBuffers(windowPtr);
}

/**
 * \brief 
 * \param spriteSource texture to render
 * \param mesh mesh to render on
 * \param transform transform data of the thing to render
 * \param frameIndex index for a sprite
 * \param rgba color value/ modifier to apply to the texture
 */
void MEDraw(SpriteSource& spriteSource, Mesh& mesh, Transform* transform, unsigned frameIndex, glm::vec4 rgba)
{

 	GLuint* textureID = spriteSource.GetTexture();
	GLuint* meshID = mesh.GetMesh();
	GLint vertexCount = mesh.GetVertexCount();
	
	glm::mat4 transformMatrix = transform->GetMatrix();

	float rows = (float)spriteSource.GetNumRows();
	float cols = (float)spriteSource.GetNumCols();

	


	glUseProgram(objectShaderID);
	CheckError();
	glBindVertexArray(*meshID);
	CheckError();
	glActiveTexture(GL_TEXTURE0);
	CheckError();
	glBindTexture(GL_TEXTURE_2D, *textureID);
	CheckError();

	

	// frames
	
	float u = 0.0f; 
	float v = 0.0f;//y offset of specified frame
	spriteSource.GetUV(frameIndex, &u, &v);
	float uSize = 1.0f/cols;//u % of spritesheet 1 frame takes up
	float vSize = 1.0f/rows;// v % of spritesheet 1 frame takes up
	GLuint u_v_loc = glGetUniformLocation(objectShaderID, "u_v");
	CheckError();
	glUniform2f(u_v_loc, u, v);
	GLuint u_v_size_loc = glGetUniformLocation(objectShaderID, "u_v_size");
	CheckError();
	glUniform2f(u_v_size_loc, uSize, vSize);
	CheckError();

	GLuint colorVal = glGetUniformLocation(objectShaderID, "colorVal");
	CheckError();
	//
	glUniform4f(colorVal, rgba.r, rgba.g, rgba.b, rgba.a);
	CheckError();
	//frames

	GLuint transformLoc = glGetUniformLocation(objectShaderID, "world");
	CheckError();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	CheckError();

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	CheckError();
	glBindVertexArray(0);
	CheckError();
	glUseProgram(0);
	CheckError();

}

void MEDrawParticle(SpriteSource& spriteSource, Mesh& mesh, glm::mat4& transformMatrix, unsigned frameIndex, glm::vec4 rgba)
{

	GLuint* textureID = spriteSource.GetTexture();
	GLuint* meshID = mesh.GetMesh();
	GLint vertexCount = mesh.GetVertexCount();
	int width = MEWindow::GetWidth();
	int height = MEWindow::GetHeight();

	
	

	float rows = (float)spriteSource.GetNumRows();
	float cols = (float)spriteSource.GetNumCols();



	glUseProgram(objectShaderID);
	CheckError();
	glBindVertexArray(*meshID);
	CheckError();
	glActiveTexture(GL_TEXTURE0);
	CheckError();
	glBindTexture(GL_TEXTURE_2D, *textureID);
	CheckError();



	// frames

	float u = 0.0f;
	float v = 0.0f;//y offset of specified frame
	spriteSource.GetUV(frameIndex, &u, &v);
	float uSize = 1.0f / cols;//u % of spritesheet 1 frame takes up
	float vSize = 1.0f / rows;// v % of spritesheet 1 frame takes up
	GLuint u_v_loc = glGetUniformLocation(objectShaderID, "u_v");
	CheckError();
	glUniform2f(u_v_loc, u, v);
	GLuint u_v_size_loc = glGetUniformLocation(objectShaderID, "u_v_size");
	CheckError();
	glUniform2f(u_v_size_loc, uSize, vSize);
	CheckError();

	GLuint colorVal = glGetUniformLocation(objectShaderID, "colorVal");
	CheckError();
	//
	glUniform4f(colorVal, rgba.r, rgba.g, rgba.b, rgba.a);
	CheckError();
	//frames

	GLuint transformLoc = glGetUniformLocation(objectShaderID, "world");
	CheckError();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	CheckError();

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	CheckError();
	glBindVertexArray(0);
	CheckError();
	glUseProgram(0);
	CheckError();

}

METexture* MEDrawToTexture(SpriteSource& spriteSource, Mesh& mesh, unsigned frameIndex, glm::mat4& transform_matrix)
{
	GLuint* textureID = spriteSource.GetTexture();
	GLuint* meshID = mesh.GetMesh();
	GLint vertexCount = mesh.GetVertexCount();
	
	////////////////////SHADER STUFF//////////////
	float rows = (float)spriteSource.GetNumRows();
	float cols = (float)spriteSource.GetNumCols();


	float u = 0.0f;
	float v = 0.0f;//y offset of specified frame
	spriteSource.GetUV(frameIndex, &u, &v);
	float uSize = 1.0f / cols;//u % of spritesheet 1 frame takes up
	float vSize = 1.0f / rows;// v % of spritesheet 1 frame takes up

	//width and height of the frame to use for the frame buffer texture size
	float frame_width = static_cast<float>(*spriteSource.GetWidth()) / cols;
	float frame_height = static_cast<float>(*spriteSource.GetHeight()) / rows;



	glBindFramebuffer(GL_FRAMEBUFFER, tempBuffer);
	//transparent background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


	if(tempTextureID)
	{
		FreeTexture(&tempTextureID);
		CreateTexture(&tempTextureID, frame_width, frame_height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTextureID, 0);
	}
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glUseProgram(objectShaderID);
	glBindVertexArray(*meshID);

	glBindTexture(GL_TEXTURE_2D, *textureID);


	GLuint u_v_loc = glGetUniformLocation(objectShaderID, "u_v");
	CheckError();
	glUniform2f(u_v_loc, u, v);
	GLuint u_v_size_loc = glGetUniformLocation(objectShaderID, "u_v_size");
	CheckError();
	glUniform2f(u_v_size_loc, uSize, vSize);
	CheckError();

	GLuint colorVal = glGetUniformLocation(objectShaderID, "colorVal");
	CheckError();
	
	glUniform4f(colorVal, 1.f, 1.f, 1.f, 1.f);
	CheckError();

	
	GLuint transformLoc = glGetUniformLocation(objectShaderID, "world");
	CheckError();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform_matrix));
	CheckError();
	




	//frames
	/////////////////////
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, viewportTextureID);
	glBindFramebuffer(GL_FRAMEBUFFER, viewportBuffer);
	glUseProgram(0);

	return &tempTextureID;

}
/**
 * \brief callback for resizing the window
 * \param window glfw window handle
 * \param width width in pixels of the viewport
 * \param height height in pixels of the viewport
 * \todo figure out if we actualy need to support resize and how to do it correctly
 */
void ResizeWindowInit(GLFWwindow* window, int width, int height)
{

	// Create the viewport at the specified size

	//glfwGetFramebufferSize(window, &oldWidth, &oldHeight);
	glfwSetWindowAspectRatio(window, 16, 9);
	CheckError();



	glm::vec3 cameraPos(0.0f, 0.0f, 1.0f);
	// Create a view cameraPos(0.0f, 0.0f, 1.0f);
	// Create  matrix using the camer
	// We are using a constant camera position
	// If the camera moved this function would need a position
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(cameraPos.x, cameraPos.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Create a 2D orthogonal projection matrix
	//glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);

	/*glm::mat4 projection = glm::ortho(
		/*xmin, xmax#1#	(cameraPos.x - (width / 2.0f)) / scale, (cameraPos.x + (width / 2.0f)) / scale,
		/*ymin, y max#1#	(cameraPos.y - (height / 2.0f)) / scale, (cameraPos.y + (height / 2.0f)) / scale
	);*/

	glm::mat4 projection = glm::ortho(
		/*xmin, xmax*/	(cameraPos.x - (width / 2.0f)), (cameraPos.x + (width / 2.0f)),
		/*ymin, y max*/	(cameraPos.y - (height / 2.0f)), (cameraPos.y + (height / 2.0f))
	);


	glm::mat4 scaleMatrix = glm::scale(glm::vec3(static_cast<float> (MEWindow::GetWidth()) / 1920.f, static_cast<float> (MEWindow::GetHeight()) / 1080.f, 1.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::radians(0.f), glm::vec3(0, 0, 1));
	//create translation matrix from translate.x and translate.y
	glm::mat4 translateMatrix = glm::translate(glm::vec3(0.f, 0.f, 1.f));
	vTransformMatrix = translateMatrix * rotationMatrix * scaleMatrix;

	// Set the matrices on the texture shader
	glUseProgram(objectShaderID);
	CheckError();
	GLint projectionLoc = glGetUniformLocation(objectShaderID, "projection");
	CheckError();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	CheckError();
	GLint viewLoc = glGetUniformLocation(objectShaderID, "view");
	CheckError();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	CheckError();
	glUseProgram(0);
	CheckError();

}

void ResizeEditor()
{
	glm::vec3 cameraPos(0.0f, 0.0f, 1.0f);
	// Create a view cameraPos(0.0f, 0.0f, 1.0f);
	// Create  matrix using the camer
	// We are using a constant camera position
	// If the camera moved this function would need a position
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(cameraPos.x, cameraPos.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Create a 2D orthogonal projection matrix
	//glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);

	/*glm::mat4 projection = glm::ortho(
		/*xmin, xmax#1#	(cameraPos.x - (width / 2.0f)) / scale, (cameraPos.x + (width / 2.0f)) / scale,
		/*ymin, y max#1#	(cameraPos.y - (height / 2.0f)) / scale, (cameraPos.y + (height / 2.0f)) / scale
	);*/

	glm::mat4 projection = glm::ortho(
		/*xmin, xmax*/	(cameraPos.x - (MEWindow::GetWidth() / 2.0f)), (cameraPos.x + (MEWindow::GetWidth() / 2.0f)),
		/*ymin, y max*/	(cameraPos.y - (MEWindow::GetHeight() / 2.0f)), (cameraPos.y + (MEWindow::GetHeight() / 2.0f))
	);


	glm::mat4 scaleMatrix = glm::scale(glm::vec3(static_cast<float> (MEWindow::GetWidth()) / 1920.f, static_cast<float> (MEWindow::GetHeight()) / 1080.f, 1.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::radians(0.f), glm::vec3(0, 0, 1));
	//create translation matrix from translate.x and translate.y
	glm::mat4 translateMatrix = glm::translate(glm::vec3(0.f, 0.f, 1.f));
	vTransformMatrix = translateMatrix * rotationMatrix * scaleMatrix;

	vTransformMatrix *= glm::vec4(0.5f, 0.5f, 1.f, 1.f);
	// Set the matrices on the texture shader
	glUseProgram(objectShaderID);
	CheckError();
	GLint projectionLoc = glGetUniformLocation(objectShaderID, "projection");
	CheckError();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	CheckError();
	GLint viewLoc = glGetUniformLocation(objectShaderID, "view");
	CheckError();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	CheckError();
	glUseProgram(0);
	CheckError();

}

void ResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	MEEditor::SetWindowSize(glm::vec2(width, height));
	glfwSetWindowAspectRatio(window, 16, 9);
	CheckError();
	//delete the viewport texture
	FreeTexture(&viewportTextureID);
	viewportTextureID = 0;
	//grab the shader
	glUseProgram(viewportShaderID);
	CheckError();
	//get the shader ready
	glUniform1i(glGetUniformLocation(viewportShaderID, "viewportTextureID"), 0);
	CheckError();
	//grab the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, viewportBuffer);
	CheckError();
	//make the new texture at the right size

	if(width == 0 or height == 0)
	{
		return;
	}

	CreateTexture(&viewportTextureID, width , height);

	//set the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewportTextureID, 0);
	CheckError();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ME_CORE_ERROR("FRAMEBUFFER NOT COMPLETE");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CheckError();



}

GLuint GetObjectBuffer()
{
	return GL_FRAMEBUFFER;
}

GLuint GetViewportTexture()
{
	return viewportTextureID;
}

/// <summary>
/// shutdown the renderer
/// </summary>
void ShutdownRenderer()
{
	glDeleteFramebuffers(1, &viewportBuffer);
	FreeTexture(&tempTextureID);
	glDeleteFramebuffers(1, &tempBuffer);
}
