/*****************************************************************//**
 * \file         SpriteSource.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#pragma once
#include "glfw/glfw3.h"
#include <string>


typedef GLuint METexture;
class SpriteSource;

class SpriteSource
{
public:
	SpriteSource();
	SpriteSource(std::string filename);
	~SpriteSource();
	void Read(const char*);
	void Save();
	bool IsNamed(const std::string _name);
	METexture* GetTexture();
	unsigned GetFrameCount();
	int GetNumRows();
	int GetNumCols();
	int* GetWidth();
	int* GetHeight();
	void GetUV(unsigned frameIndex, float* u, float* v);
	void SetName(std::string _name);
	std::string GetName();
	void SetPath(std::string _path);
	void SetNumRows(int _rows);
	void SetNumCols(int _cols);


private:

	std::string name;
	std::string path;
	int numRows;
	int numCols;
	int width;
	int height;

	METexture texture;

};
