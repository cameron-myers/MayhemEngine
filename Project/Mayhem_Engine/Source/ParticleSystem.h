/*****************************************************************//**
 * \file         ParticleSystem.h
 * \author       Nate White
 * \par          Email: nate.white@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#pragma once

#define MAX_SIZE 100000
#include <string>
#include <vector>
class Transform;
class Sprite;
class Physics;
struct particle{
	//x,y
	glm::vec2 pos;
	float alpha;
	glm::mat2 scalerot;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float LifeLeft;
	unsigned FrameIndex;
	float FrameTimer;
};

class ParticlePool
{
public:
	ParticlePool();
	ParticlePool(const ParticlePool& rhs);
	~ParticlePool();
	void Spawn(int numberOfParticles);
	void Update(float dt,GameObjectPtr parent);
	void Draw();
	void SetSprite(Sprite* rhs);
	void SetTransform(TransformPtr rhs);
	void SetMoveBool(bool move);
	void SetLifeLength(float length);
	void SetAcceleration(glm::vec2 accelerate);
	void SetFade(std::string fade);
	void SetFadeTime(float fadetime);
	void SetFrameCount(int frameCount);
	void SetScaleSetting(std::string scalesetting);
	void SetScaleMultiplier(float scaleMultiplier);
	void SetScaleTime(float scaletime);
	void SethiveMind(bool hive);
	void SetUniformAnimation(bool UniformAnimation);
	void SetFrameDuration(float frameDuration);
	void SetIsLooping(bool isLooping);
	bool GetFull();

private:
	Sprite* sprite;
	Transform* transform;
	std::vector<particle> pool;
	glm::vec2 acceleration;
	bool ParticleMoveWithObject;
	bool hiveMind;
	bool uniformAnimation;
	bool IsLooping;
	float lifeLength;
	float FrameDuration;
	std::string Fade;
	float FadeTime;
	std::string ScaleSetting;
	float ScaleMultiplier;
	float ScaleTime;
	int FrameCount;

};
class Emitter : public Component
{
public:
	Emitter();
	Emitter(const Emitter& rhs);
	Emitter* Clone()const override;
	~Emitter();
	void spawn(int numOfParticles);
	void Read(const char* bufchar);
	//the min/max speed multiplier will be applied
	void SetAcceleration(glm::vec2 Acceleration);
	//the min/max speed multiplier will not be applied
	void SetAccelerationWithoutMultiplier(glm::vec2 Acceleration);
	//determines a random acceleration within the spray angle range
	glm::vec2 RandomAcceleration();
	float GetSpawnRate();
	void  SetSpawnRate(float new_rate);
	std::string GetPatternType();
	bool IsFull();
	void SetVelocity(glm::vec3 Velocity);
	void Init()override;
	void Exit();
	void Draw() const override;
	void Update(float dt) override;
	
private:
	//serialized
	std::string PatternType;//doesnt do anything at the moment
	//if true when the particles come out of the emitter they will follow the objects velocity even after being spawned. if false the particle will spawn at the parent object but not continue to follow it
	bool ParticleMoveWithObject;
	//this will make the acceleration of all particles match instead of particles using their own acceleration
	bool HiveMind;
	//if true all particles will have the same frame index
	bool UniformAnimation;
	//time in seconds of how long each particle will last
	float lifeLength;
	//how many particles to spawn 
	float SpawnRate;//doesnt do anything at the moment
	//in degrees what angle the particles spray from the emitter 0-360
	int SprayAngle;// 2 durring math so odd numbered degrees will be the same effect 1 digit lower
	//how fast each particle moves
	//the minimum speed multiplier that will be applied to acceleration
	float MinSpeed;//doesnt do anything at the moment
	//the maximum speed multiplier that will be applied to acceleration
	float MaxSpeed;//doesnt do anything at the moment
	//fade effect, can be In,Out,None for fade in/out/ no fade
	std::string Fade;
	//how long the fade effect will last
	float FadeTime;
	//current settings are not implemented
	std::string ScaleSetting;
	//the scale that will eventually be reached
	float ScaleMultiplier;
	//the scale multiplier
	float ScaleTime;
	//non serialized
	ParticlePool* pool;

};
