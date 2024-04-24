/*****************************************************************//**
 * \file         Animation.h
 * \authors       Nate White, Cameron Myers, Ethan Mclaughlin
 * \par          Email: nate.white\@digipen.edu,cameron.myers\@digipen.edu, ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Component.h"


class Animation;
class Sprite;
class GameObject;

typedef GameObject* GameObjectPtr;
typedef Animation* AnimationPtr;
typedef Sprite* SpritePtr;


class Component;
class Animation : public Component
{
public:
	Animation();

	Animation(const Animation& rhs);
	~Animation();
	Component* Clone() const;
	void	Read(const char* bufchar);
	void	AnimationAdvanceFrame();
	void	SetRowForMultiAnimation(int DesiredRow);
	void	AnimationPlay(int frameCount, float frameDuration, bool isLooping, int startFrame = 0);
	void	Update(float dt);
	bool	AnimationIsDone();
	int		getFrameCount();
	int		getFrameIndex();
	int		GetRowOfCurrentIndex();
	float	getFrameDuration();
	bool	getIsRunning();
	void	SetFrameCount(int count);
	void	SetFrameIndex(int _index);
	void	SetFrameDuration(float _time);
	bool	isLooping();
	void	SetisLooping(bool loop);
	bool	isDiffAnimations();
	void	SetisDiffAnimations(bool diff);

	void	SetIsRunning(bool status);


	void Save();
private:

	// The current frame being displayed.
	int frameIndex;
	// The maximum number of frames in the sequence.
	int frameCount;
	// The time remaining for the current frame.
	float frameRate;
	// The amount of time to display each successive frame.
	float frameDuration;
	// True if the animation is running; false if the animation has stopped.
	bool IsRunning;
	// True if the animation loops back to the beginning.
	bool IsLooping;
	// True if the end of the animation has been reached, false otherwise.
	bool IsDone;
	// True if each row is a different animation as opposed to the whole spritesheet being one animation
	bool DiffAnimations;
	//enables playing animation and then hiding
	bool IsVFX;

	void AnimationAdvanceDifferentFrame();
};

