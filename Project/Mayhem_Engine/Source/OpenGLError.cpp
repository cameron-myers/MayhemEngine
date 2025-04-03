/*********************************************************************************************************************/
/*!
  \file              OpenGL.cpp
  \par               Copyright � 2022 DigiPen, All rights reserved.

  \brief
	This file handles the opengl implementation
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "OpenGLError.h"
#include <iostream>
#include "Log.h"
/**
 * \brief This function will check if opengl experienced an error and terminate the program if it did 	ucrtbased.dll!00007fff901f12b5()	Unknown

 */
void CheckError()
{
#ifndef _DISTRIBUTE
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		ME_CORE_ERROR("There was an error. Called from the CheckError Function")
			ME_CORE_ERROR(error);
		assert(false);
	}
#endif

}

/**
 * \brief Prints a shader error
 * \param shaderID 
 */
void PrintShaderError(GLuint shaderID)
{
	// Create an array to store the message
	GLchar infoLog[256] = { 0 };
	// Store the length of the info message
	GLsizei infoLength = 0;
	// Get the info log from the shader
	glGetShaderInfoLog(shaderID, sizeof(infoLog), &infoLength, infoLog);
	// Print the info message
	ME_CORE_ERROR("Failed to compile the vertex stage");
	ME_CORE_ERROR(infoLog);
	/*std::cout << "Failed to compile the vertex stage: " << std::endl;
	std::cout << infoLog << std::endl;
	*/
}
