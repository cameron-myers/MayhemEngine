/*****************************************************************//**
 * \file         Sprite.h
 * \author       Cameron Myers, Nate White
 * \par          Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "Component.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include <string>

class Mesh;
class SpriteSource;
class Component;

class Sprite : public Component
{
public:
	Sprite();
	Sprite(const Sprite& rhs);

	~Sprite();

	Component* Clone() const override;
	void Read(const char* bufchar);


	void Draw() const override;

	void SetAlpha(float _alpha);
	void SetFrame(unsigned _frameIndex);
	void SetMesh(Mesh* _mesh);
	void SetSpriteSource(SpriteSource* _spriteSource);
	void SetRGB(glm::vec3 _rgb);
	void SetText(const std::string NewText);
	void SetTextMode(const char* _mode);
	float GetAlpha();
	unsigned GetFrame();
	Mesh* GetMesh();
	SpriteSource* GetSpriteSource();
	glm::vec3 GetRGB();
	std::string GetText();
	char* GetTextMode();
	void Save();

private:
	unsigned frameIndex;
	float alpha;
	Mesh* mesh;
	SpriteSource* spriteSource;
	glm::vec3 rgb;
	std::string text;
	char* textMode;
	int StringLength;
};
