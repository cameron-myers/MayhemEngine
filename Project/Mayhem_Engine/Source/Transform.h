/*****************************************************************//**
 * \file         Transform.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "Component.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

class Component;
class Transform;
typedef Component* ComponentPtr;
typedef Transform* TransformPtr;

class Transform :public Component
{
public:
	Transform();
	~Transform();
	//make deep copy
	Transform(const TransformPtr& rhs);
	virtual ComponentPtr Clone()const override;
	void Read(const char* path);
	glm::mat4 GetMatrix();
	glm::vec3 GetTranslation();
	glm::vec3& GetTranslationRef();
	float GetRotation();
	glm::vec3 GetScale();
	void SetTranslation(const glm::vec3 translation);
	void SetRotation(float rotation);
	void SetScale(const glm::vec3 scale);
	void SetScale(float _scale);
	void Save();
	glm::vec3 GetRelativeTran();
	void SetParticleMatrix(glm::mat2 _scalerot, glm::vec2 _pos);
	void setDirty(bool bo);


private:
	glm::vec3 translation;
	float rotation;
	glm::vec3 scale;
	glm::mat4 matrix;
	bool isDirty;
};

