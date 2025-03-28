//------------------------------------------------------------------------------
//
// File Name:	Component.c
// Author(s):	Nicholas Leben
// Project:		Project 7
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "SpriteSource.h"
#include "Log.h"
#include "MEResources.h"
#include "SpriteSourceManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "MESerializer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <Shlobj_core.h>

#include "Engine.h"

/**
 * \brief SpriteSource constructor
 * \param numCols coloumns in the sprite sheet
 * \param numRows rows in the sprite sheet
 * \param texture texture id
 */
SpriteSource::SpriteSource()
{
	numCols = 1;
	numRows = 1;
	name = "nullptr";
	path = "\0";
	width = 0;
	height = 0;

	texture = NULL;
}

/**
 * \brief Sprite Source contructor with name
 * \param spriteSourceName name of the sprite source to create
 */
SpriteSource::SpriteSource(std::string spriteSourceName):name("\0"), path("\0"), numRows(0), numCols(0), width(0),height(0),texture(0)

{
	
	const SpriteSource* temp = SpriteSourceManager::Build(spriteSourceName);
	if (temp)
	{
		name = temp->name;
		path = temp->path;
		texture = temp->texture;
		numCols = temp->numCols;
		numRows = temp->numRows;
	}
}
/// <summary>
/// get the name of a spritesource
/// </summary>
/// <returns></returns>
std::string SpriteSource::GetName()
{
	return name;
}
/**
 * \brief SpriteSource deconstructor
 */
SpriteSource::~SpriteSource()
{
	if(texture)
	{
		FreeTexture(&texture);
	}
	name.clear();
	path.clear();
	numCols = NULL;
	numRows = NULL;
	width = NULL;
	height = NULL;

	texture = NULL;
}

/**
 * \brief Read in the SpriteSource
 * \param filename name of the file to open
 */
void SpriteSource::Read(const char* filename)
{

	SpriteSource newSpriteSource = MESerializer::LoadSpriteSource(filename);

	const char* texPath = newSpriteSource.path.c_str();
	CreateTextureImage(&texture, texPath, &width, &height);
	if (!texture)
	{
		ME_CORE_ERROR("METexture: %s failed to load", filename);
		assert(false);
	}

	name = newSpriteSource.name;
	path = newSpriteSource.path;
	numRows = newSpriteSource.numRows;
	numCols = newSpriteSource.numCols;
}

/**
 * \brief Save the sprite source json
 */
void SpriteSource::Save()
{
	FILE* json = NULL;
	char path[256];
	sprintf_s(path, MAYHEM_DIR("\\Assets\\Art\\%s.json"), this->name.c_str());

	 fopen_s(&json, path, "w+");
	

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}
	char buf[1]; //number is arbitrary

	rapidjson::FileWriteStream strooom(json, buf, sizeof(buf));

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(strooom);
	//start the object
	writer.StartObject();
	writer.Key("SpriteSource");
	writer.StartObject();
	//write the name
	writer.Key("name");
	writer.String(this->name.c_str());
	//write the path
	writer.Key("path");
	writer.String(this->path.c_str());
	//write the columns
	writer.Key("numCols");
	writer.Int(this->numCols);
	//write the rows
	writer.Key("numRows");
	writer.Int(this->numRows);

	writer.EndObject();

	writer.EndObject();


	fclose(json);

}

/**
 * \brief checks if the sprite source has a certain name
 * \param _name name to compare against
 * \return if the names are equal
 */
bool SpriteSource::IsNamed(const std::string _name)
{
	if (_name == name) return true;

	return false;
}

/**
 * \brief Gets a pointer to the texture
 * \return a pointer to the texture
 */
METexture* SpriteSource::GetTexture()
{
	return &texture;
}

/**
 * \brief Gets number of frames in the sprite source
 * \return frame count
 */
unsigned SpriteSource::GetFrameCount()
{
	return numRows * numCols;
}

/**
 * \brief Gets the UV info
 * \param frameIndex 
 * \param u 
 * \param v 
 */
void SpriteSource::GetUV(unsigned frameIndex, float* u, float* v)
{
	float uSize = 1.0f / static_cast<float>(numCols);
	float vSize = 1.0f / static_cast<float>(numRows);

	*u = uSize * (frameIndex % numCols);
	*v = vSize * (frameIndex / numCols);
}

/**
 * \brief sets the name of the sprite source
 * \param name string to set name to
 */
void SpriteSource::SetName(std::string _name)
{
	name = _name;
}

/**
 * \brief Sets the path to the spritesources image file
 * \param path path of the source file
 */
void SpriteSource::SetPath(std::string _path)
{
	path = _path;
}

/**
 * \brief sets the amount of rows in the sprite sheet
 * \param rows 
 */
void SpriteSource::SetNumRows(int _rows)
{
	numRows = _rows;
}

/**
 * \brief sets the amount of columns in the sprite sheet
 * \param cols 
 */
void SpriteSource::SetNumCols(int _cols)
{
	numCols = _cols;
}
/// <summary>
/// get the number of rowss in the sprite source
/// </summary>
/// <returns></returns>
int SpriteSource::GetNumRows()
{
	return numRows;
}

/**
 * \brief sets the amount of columns in the sprite sheet
 * \param cols
 */
int SpriteSource::GetNumCols()
{
	return numCols;
}

int* SpriteSource::GetWidth()
{
	return &width;
}

int* SpriteSource::GetHeight()
{
	return &height;
}
