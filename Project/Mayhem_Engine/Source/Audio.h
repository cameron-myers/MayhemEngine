/*****************************************************************//**
 * \file         Audio.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#define UNREFERENCED_PARAMETER(P) (P)
#define SOUND(s) AK::EVENTS::##s

#include <string>


#include <AK/SoundEngine/Common/AkTypes.h>
#include <SoundEngine/Common/AkCallback.h>
#include "Wwise_IDs.h"
#include "Component.h"

typedef AkUniqueID MESoundID;
typedef AkUniqueID MEParameterID;

class Component;

class Audio : public Component
{
public:
	Audio();
	Audio(const Audio& rhs);
	~Audio();


	Component* Clone()const override;

	void Update(float dt) override;
	void Save() const override;

	AkGameObjectID* GetListener();
	AkGameObjectID* GetEmitter();
	void SetListener(AkUInt64 id);
	void SetEmitter(AkUInt64 id);


	
	void SetParameter(AkRtpcID paramId, float value, AkTimeMs changeDuration = 0,
					  AkCurveInterpolation fadeCurve = AkCurveInterpolation_Linear,
					  bool bypassInternalInterpolation = false);

	void SetSoundStateByID(AkStateGroupID stateGroup, AkStateID state);
	void SetSoundState(const char* stateGroup, const char* state);

	void SetSoundSwitchByID(AkSwitchGroupID switchGroup, AkSwitchStateID switchState);
	void SetSoundSwitch(const char* switchGroup, const char* switchState);

	void PlayAudio(MESoundID sound, AkUInt32 flags = 0, AkCallbackFunc fnCallback = nullptr,
		void* pCookie = nullptr, AkUInt32 cExternals = 0,
		AkExternalSourceInfo* ExternalSources = nullptr, AkPlayingID PlayingID = AK_INVALID_PLAYING_ID);

	void PlayTrigger(AkTriggerID triggerID);

	void PlaySoundEffect(MESoundID sound);

	bool IsPlaying(MESoundID sound);

	AkPlayingID GetPlayingID(MESoundID sound);

	void StopAudioAll();


private:

	void GeneratePlaying();

	AkGameObjectID listener;
	AkGameObjectID emitter;
	AkPlayingID* playing;
	AkUInt32 active;


};