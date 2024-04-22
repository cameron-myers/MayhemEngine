/*****************************************************************//**
 * \file         Audio.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "stdafx.h"
#include "Component.h"
#include "Audio.h"
#include "Log.h"
#include "GameObject.h"
#include "MEAudio.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include <SoundEngine/Common/AkSoundEngine.h>
#include <SoundEngine/Common/AkQueryParameters.h>

/// @brief Audio ctor, Builds a new Wwise Object and initializes a active event array
Audio::Audio()
	:Component(cAudio),
	listener(NULL),
	emitter(NULL),
	playing(new AkPlayingID[50]),
	active(0)
{
	MEAudio::Build(this);
	memset(playing, 0, sizeof(AkPlayingID) * 50);
}

/// @brief audio dtor
/// @todo look into unregistering the Wwise object
Audio::~Audio()
{
	for(unsigned i = 0; i < active; ++i)
	{
		AK::SoundEngine::StopPlayingID(playing[i]);
	}
	AK::SoundEngine::UnregisterGameObj(listener);
	AK::SoundEngine::UnregisterGameObj(emitter);

	listener = NULL;
	emitter = NULL;
	active = NULL;
	delete[] playing;

}
/// @brief Audio copy ctor, 
/// @param rhs Audio component to copy from
Audio::Audio(const Audio& rhs)
	:Component(cAudio),
	listener(NULL),
	emitter(NULL),
	playing(new AkPlayingID[50]),
	active(0)
{
	//build a new wwise object
	MEAudio::Build(this);
	//deep copy the active events
	for(unsigned i = 0; i < 50; ++i)
	{
		playing[i] = rhs.playing[i];
	}

}
/// @brief shallow copy clone
/// @return clone of this
Component* Audio::Clone() const
{
	
	return new Audio(*this);
}

void Audio::Save() const
{

}

/// @brief Update the audio component
/// @param dt 
void Audio::Update(float dt)
{
	//update active
	GeneratePlaying();


	//Transform* transform = Parent()->GetComponent<Transform>(cTransform);
	//glm::vec3 
	//  = transform->GetTranslation();
	//AkVector vec;
	//vec.X = translation.x;
	//vec.Y = translation.y;
	//vec.Z = translation.z;
	//AkSoundPosition pos;
	//pos.SetPosition(vec);
	//update the emitter position
	//AK::SoundEngine::SetPosition(emitter, pos);
}
/// @brief 
/// @return id of the Wwise listener 
AkGameObjectID* Audio::GetListener()
{
	return &listener;
}

/// @brief 
/// @return id of the Wwise emitter 
AkGameObjectID* Audio::GetEmitter()
{
	return &emitter;
}

/// @brief sets the Wwise listener
/// @param id 
void Audio::SetListener(AkUInt64 id)
{
	listener = id;
}

/// @brief sets the Wwise emitter
/// @param id 
void Audio::SetEmitter(AkUInt64 id)
{
	emitter = id;
}

/// @brief A wrapper around the Wwise SetRTPC Value function
/// @param paramId RTPC ID to modify
/// @param value new RTPC value
/// @param changeDuration duration of the value change
/// @param fadeCurve type of interpolation to use
/// @param bypassInternalInterpolation switch to bypass interpolation
void Audio::SetParameter(AkRtpcID paramId, float value, AkTimeMs changeDuration,
						 AkCurveInterpolation fadeCurve,
						 bool bypassInternalInterpolation)
{
	AKRESULT result = AK::SoundEngine::SetRTPCValue(paramId, value, emitter, changeDuration, fadeCurve, bypassInternalInterpolation );
	if(result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Set Paramenter Error: %d ", result);
	}
}

/// @brief Set the SoundState of a group by ID
/// @param stateGroup group to modify
/// @param state state to change to
void Audio::SetSoundStateByID(AkStateGroupID stateGroup, AkStateID state)
{
	AKRESULT result = AK::SoundEngine::SetState(stateGroup, state);

	if (result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Set Sound State by ID Error: %d ", result);
	}
}

/// @brief Set the SoundGroups state by name
/// @param stateGroup name of the state group
/// @param state name of the state to change to
void Audio::SetSoundState(const char* stateGroup, const char* state)
{
	AKRESULT result = AK::SoundEngine::SetState(stateGroup, state);
	if (result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Set Sound State by String Error: %d ", result);
	}
}

/// @brief Set the switch of a Group
/// @param switchGroup ID of group
/// @param switchState ID of switch state
void Audio::SetSoundSwitchByID(AkSwitchGroupID switchGroup, AkSwitchStateID switchState)
{
	AKRESULT result = AK::SoundEngine::SetSwitch(switchGroup, switchState, emitter);
	if (result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Set Sound Switch by ID Error: %d ", result);
	}
}

/// @brief Set the switch group by name
/// @param switchGroup 
/// @param switchState 
void Audio::SetSoundSwitch(const char* switchGroup, const char* switchState)
{
	AKRESULT result = AK::SoundEngine::SetSwitch(switchGroup, switchState, emitter);
	if (result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Set Sound Switch by string Error: %d ", result);
	}

}

/// @brief Post a Wwise Audio Event
/// @param sound Sound to play
/// @param flags flag for the condition of the callback
/// @param fnCallback a callback for the event
/// @param pCookie cookie and other data for the callback
/// @param cExternals 
/// @param ExternalSources 
/// @param PlayingID OPTIONAL if you want to assign the event to a specific playing id
void Audio::PlayAudio(MESoundID sound, AkUInt32 flags, AkCallbackFunc fnCallback, void* pCookie, AkUInt32 cExternals,
					  AkExternalSourceInfo* ExternalSources, AkPlayingID PlayingID)
{
	if(this)
	{
		AkPlayingID result = AK::SoundEngine::PostEvent(sound, emitter, flags, fnCallback, pCookie, cExternals, ExternalSources, PlayingID);
		if (result == AK_INVALID_PLAYING_ID)
		{
			ME_CORE_ERROR("Wwise Play audio: %i ", result);
		}
	}
	
}

/// @brief Post a trigger
/// @param triggerID trigger id to post
void Audio::PlayTrigger(AkTriggerID triggerID)
{
	AKRESULT result = AK::SoundEngine::PostTrigger(triggerID, emitter);
	if (result != AK_Success)
	{
		ME_CORE_ERROR("Wwise Post Trigger Error: %d ", result);
	}
}

/// @brief Plays a sound effect, only if not already playing on a Wwise object
/// @param sound Sound to play
void Audio::PlaySoundEffect(MESoundID sound)
{
	if (!IsPlaying(sound))
	{
		PlayAudio(sound);
	}
}

/// @brief updates the active playingID list
void Audio::GeneratePlaying()
{
	AK::SoundEngine::Query::GetPlayingIDsFromGameObject(emitter, active, playing);
}

/// @brief checks if the sound is already playing on this Wwise object
/// @param sound sound to check for
/// @return playing or not
bool Audio::IsPlaying(MESoundID sound)
{
	for (int i = 0; i < 50; ++i)
	{
		if (AK::SoundEngine::Query::GetEventIDFromPlayingID(playing[i]) == sound)
		{
			return true;
		}
	}
	
	return false;
}

/// @brief Return the playingID of a sound
/// @param sound Sound to get playingid for
/// @return Playing id for the sound
AkPlayingID Audio::GetPlayingID(MESoundID sound)
{
	for (int i = 0; i < 50; ++i)
	{
		AkPlayingID _id = AK::SoundEngine::Query::GetEventIDFromPlayingID(playing[i]);
		if (_id == sound)
		{

			return _id;
		}
	}
	return 0;
}

/// <summary>
/// stop all audio
/// </summary>
void Audio::StopAudioAll()
{
	AK::SoundEngine::StopAll(emitter);
}

