/*****************************************************************//**
 * \file         Animation.cpp
 * \authors       Nate White,Cameron Myers, Ethan Mclaughlin
 * \par          Email: Nate.white\@digipen.edu,cameron.myers\@digipen.edu, ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "Animation.h"

#include <iostream>

#include "Sprite.h"
#include "GameObject.h"
#include "Component.h"
#include "Log.h"
#include <glfw/glfw3.h> 

#include "MESerializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "SpriteSource.h"
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.

// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Animation::Animation()
	:Component(cAnimation),
	frameIndex(0),
	frameCount(0),
	frameRate(0),
	frameDuration(0.0f),
	IsRunning(TRUE),
	IsLooping(FALSE),
	IsDone(FALSE),
	DiffAnimations(FALSE)
{
}

/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Animation::Animation(const Animation& rhs):Component(cAnimation)
{
	frameIndex = rhs.frameIndex;
	frameCount = rhs.frameCount;
	frameRate = rhs.frameRate;
	frameDuration = rhs.frameDuration;
	IsRunning = rhs.IsRunning;
	IsDone = rhs.IsDone;
	DiffAnimations = rhs.DiffAnimations;

}

/// <summary>
/// destructor
/// </summary>
Animation::~Animation()
{
	frameIndex = NULL;
	frameCount = NULL;
	frameRate = NULL;
	frameDuration = NULL;
	IsRunning = NULL;
	IsLooping = NULL;
	DiffAnimations = NULL;
	IsDone = NULL;
}

/// <summary>
/// clone function
/// </summary>
/// <returns>pointer to new clone</returns>
Component* Animation::Clone() const
{
	Animation* animation = new Animation;
	if (animation)
	{
		*animation = *this;
		
	}
	return animation;
}

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameRate and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = Pointer to the data stream used for reading.
void Animation::Read(const char* bufchar)
{
	using namespace rapidjson;

	Document d;
	d.Parse(bufchar);

	const rapidjson::Value& ghead = d["GameObject"];

	const Value& head = ghead["Animation"];
	const Value& FrameCount = head["FrameCount"];
	const Value& FrameDuration = head["FrameDuration"];
	const Value& isLooping = head["IsLooping"];
	const Value& diffAnimations = head["DiffAnimations"];
	frameCount = FrameCount.GetInt();
	frameDuration = FrameDuration.GetFloat();
	IsLooping = isLooping.GetBool();
	DiffAnimations = diffAnimations.GetBool();
}
/// <summary>
/// saves animation data
/// </summary>
/// <param name="writer"></param>
void Animation::Save()
{
	using namespace rapidjson;

	char pathName[FILENAME_MAX] = "";

	if (Parent()->HasParent())
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), "../Assets/GameObjects/Children/%s.json", Parent()->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		
		(pathName, _countof(pathName), "./Assets/GameObjects/Children/%s.json", Parent()->GetName().c_str());
#endif // _DISTRIBUTE
	}
	else
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), "../Assets/GameObjects/%s.json", Parent()->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/%s.json", Parent()->GetName().c_str());
#endif // _DISTRIBUTE
	}


	std::string jsonstr;
	std::string clearData(jsonstr);
	jsonstr = MESerializer::OpenFileRead(pathName);
	const char* bufchar = jsonstr.c_str();
	Document d;
	d.Parse(bufchar);

	/*Value* frame_index = GetValueByPointer(d, "/GameObject/Sprite/frameIndex");
	frame_index->SetInt(sprite->GetFrame());*/


	SetValueByPointer(d, "/GameObject/Animation/FrameCount", getFrameCount());

	SetValueByPointer(d, "/GameObject/Animation/FrameDuration", getFrameDuration());

	SetValueByPointer(d, "/GameObject/Animation/IsLooping",isLooping());

	SetValueByPointer(d, "/GameObject/Animation/DiffAnimations", isDiffAnimations());




	//wchar_t filename[248];
	FILE* json = NULL;

	//WCHAR path[MAX_PATH];
	//HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	//std::filesystem::remove_all(filepath / "DigiPen" / "ZeppelinRush");
	//std::filesystem::create_directories(filepath / "DigiPen" / "ZeppelinRush");

	//wsprintf(filename, TEXT("%s\\%s"), path, TEXT("DigiPen\\ZeppelinRush\\TEMPTEST_REMOVE.json"));
	//WCHAR itt[248]; //we shouldent need more then 10 didgets
	char itt[248]; //we shouldent need more then 10 didgets
	if (Parent()->HasParent())
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\Children\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, "../Assets/GameObjects/Children/%s.json", (Parent()->GetName()).c_str());
	}
	else
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, "../Assets/GameObjects/%s.json", (Parent()->GetName()).c_str());
	}

	//_wfopen_s(&json, itt, L"w+");
	fopen_s(&json, itt, "w+");

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}

	char writeBuffer[65500];
	FileWriteStream os(json, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(json);


	/*writer->Key("FrameCount");
	writer->Uint(frameCount);
	writer->Key("FrameDuration");
	writer->Double(frameDuration);
	writer->Key("IsLooping");
	writer->Bool(IsLooping);
	writer->Key("DiffAnimations");
	writer->Bool(DiffAnimations);*/
}
/// <summary>
/// returns the current row of current index
/// </summary>
/// <returns>current row</returns>
int Animation::GetRowOfCurrentIndex()
{
	SpritePtr spriteptr = Parent()->GetComponent<Sprite>(cSprite);//(get the sprite sheet col and row count)
	int Col = spriteptr->GetSpriteSource()->GetNumCols();
	return frameIndex / Col;
}
//desired row is zero indexed whereas the num rows is not

/// <summary>
/// Updates the current frame index to the start of the desired row
/// </summary>
/// <param name="DesiredRow"></param>
void Animation::SetRowForMultiAnimation(int DesiredRow)
{
	SpritePtr spriteptr = Parent()->GetComponent<Sprite>(cSprite);//(get the sprite sheet col and row count)
	int Row = spriteptr->GetSpriteSource()->GetNumRows();
	if (DesiredRow < Row)
	{
		int Col = spriteptr->GetSpriteSource()->GetNumCols();
		frameIndex = Col*DesiredRow;
		spriteptr->SetFrame(this->frameIndex);
	}
	else
	{
		ME_CORE_ERROR("Set Row For Multi Animation Failed because the desired row was greater then the number of rows");
	}
}

/// <summary>
/// advance the animation to a different frame
/// </summary>
void Animation::AnimationAdvanceDifferentFrame()
{
	if (this)
	{
		SpritePtr spriteptr = Parent()->GetComponent<Sprite>(cSprite);//(get the sprite sheet col and row count)
		int Row = spriteptr->GetSpriteSource()->GetNumRows();
		int Col = spriteptr->GetSpriteSource()->GetNumCols();
		int currentRow = frameIndex / Col;
		int currentCol = frameIndex % Col;
		this->frameIndex += 1;
		if (currentRow != (frameIndex / Col))
		{
			if (this->IsLooping == true)
			{
				frameIndex = frameIndex - Col;
				IsDone = true;
			}
			else
			{
				frameIndex = currentRow*Col;
				IsRunning = false;
			}
			IsDone = true;
		}
		if (this->IsRunning == true)
		{
			//SpritePtr spriteptr = Parent()->GetComponent<Sprite>(cSprite);
			spriteptr->SetFrame(this->frameIndex);
			this->frameRate += this->frameDuration;
		}
		else
		{
			this->frameRate = 0;
		}
	}
}

/// <summary>
/// advance the animation frame
/// </summary>
void Animation::AnimationAdvanceFrame()
{
		if (DiffAnimations == true)
		{
			this->AnimationAdvanceDifferentFrame();
			return;
		}
		if (this)
		{
			this->frameIndex += 1;
			if (this->frameIndex >= this->frameCount)
			{
				if (this->IsLooping == true)
				{
					this->frameIndex = 0;
				}
				else
				{
					frameIndex = 0;
					//this->frameIndex = this->frameCount - 1;
					this->IsRunning = false;
				}
				this->IsDone = true;


			}
			if (this->IsRunning == true)
			{
				SpritePtr spriteptr = Parent()->Has(Sprite);
				if(spriteptr)
				{
					spriteptr->SetFrame(this->frameIndex);
					this->frameRate += this->frameDuration;
				}

			}
			else
			{
				this->frameRate = 0;
			}
		}
}

/// <summary>
/// 
/// </summary>
/// <returns>frame count</returns>
int Animation::getFrameCount()
{
	return frameCount;
}


int Animation::getFrameIndex()
{
	return frameIndex;
}

/// <summary>
/// 
/// </summary>
/// <returns>frame duration</returns>
float Animation::getFrameDuration()
{
	return frameDuration;
}
/// <summary>
/// 
/// </summary>
/// <returns>if the animation is currently looping</returns>
bool Animation::isLooping()
{
	return IsLooping;
}


bool Animation::isDiffAnimations()
{
	return DiffAnimations;
}

void Animation::SetFrameCount(int count)
{
	frameCount = count;
}

void Animation::SetFrameIndex(int _index)
{
	frameIndex = _index;
}

void Animation::SetFrameDuration(float _time)
{
	frameDuration = _time;
}

void Animation::SetisLooping(bool loop)
{
	IsLooping = loop;
}

void Animation::SetisDiffAnimations(bool diff)
{
	DiffAnimations = diff;
}

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.

/// <summary>
/// play the animation
/// </summary>
/// <param name="frameCount"></param>
/// <param name="frameDuration"></param>
/// <param name="isLooping"></param>
/// <param name="startIndex"></param>
void Animation::AnimationPlay(int frameCount, float frameDuration, bool isLooping, int startIndex)
{
	if (this)
	{
		this->frameCount = frameCount;
		this->frameDuration = frameDuration;
		this->IsLooping = isLooping;
		this->frameIndex = startIndex;
		this->frameRate = frameDuration;
		this->IsDone = false;
		this->IsRunning = true;
		SpritePtr spriteptr = Parent()->GetComponent<Sprite>(cSprite);
		spriteptr->SetFrame(this->frameIndex);
	}
}

// Play a complex animation sequence (any frame/any delay).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
//void AnimationPlaySequence(AnimationPtr animation, AnimationSequencePtr sequence);

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update( float dt)
{
	if (this)
	{
		this->IsDone = false;
		if (this->IsRunning)
		{
			this->frameRate -= dt;
			if (this->frameRate <= 0)
			{
				this->AnimationAdvanceFrame();
			}
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool Animation::AnimationIsDone()
{
	if (this)
	{
		return this->IsDone;
	}
	return false;
}
/// <summary>
/// set the animation to running
/// </summary>
/// <param name="status"></param>
void Animation::SetIsRunning(bool status)
{
	IsRunning = status;
}

bool Animation::getIsRunning()
{
	return IsRunning;
}
