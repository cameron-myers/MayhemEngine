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
#include <glfw/glfw3.h> 
#include "glm/gtx/rotate_vector.hpp"
#include "Transform.h"
#include "ParticleSystem.h"
#include <cmath>
#include <random>
#include <functional>
#include <random>
#include "Sprite.h"
#include "Physics.h"
#include "Component.h"
#include "MERendering.h"
#include "GameObject.h"
#include "Animation.h"
#include "ParticleSystem.h"

#include "MEFramerateController.h"
inline float randomRange(float min, float max)
{
	// Seed the random number generator with a random device
	std::random_device rd;
	std::mt19937 gen(rd());

	// Generate a random number between min and max
	std::uniform_real_distribution<float> dis(min, max);//max is actually exclusive but shouldnt be noticble given these are floats
	return dis(gen);
}
ParticlePool::ParticlePool()
	:sprite(nullptr),
	transform(nullptr),
	pool(0),
	acceleration(0),
	ParticleMoveWithObject(false),
	hiveMind(false),
	uniformAnimation(true),
	IsLooping(false),
	lifeLength(0),
	Fade(""),
	FadeTime(0),
	ScaleSetting(""),
	ScaleMultiplier(0),
	ScaleTime(0),
	FrameCount(0)
	{
	}

ParticlePool::ParticlePool(const ParticlePool& rhs)
	:sprite(rhs.sprite),
	transform(nullptr),
	pool(0),
	acceleration(rhs.acceleration),
	ParticleMoveWithObject(rhs.ParticleMoveWithObject),
	hiveMind(rhs.hiveMind),
	uniformAnimation(rhs.uniformAnimation),
	IsLooping(rhs.IsLooping),
	lifeLength(rhs.lifeLength),
	Fade(rhs.Fade),
	FadeTime(rhs.FadeTime),
	ScaleSetting(rhs.ScaleSetting),
	ScaleMultiplier(rhs.ScaleMultiplier),
	ScaleTime(rhs.ScaleTime),
	FrameCount(rhs.FrameCount)
{
}

ParticlePool::~ParticlePool()
{
	pool.clear();
	sprite = nullptr;
	transform = nullptr;
}
inline void UpdateParticlePositionHive(particle* Particle, GameObjectPtr parent, float dt, std::string Fade, float FadeTime, std::string ScaleSetting, float ScaleMultiplier, float ScaleTime, float LifeLength, glm::vec2 acceleration,float FrameDuration,int FrameCount, bool IsLooping)
{
	Particle->scalerot *= ScaleMultiplier * (Particle->LifeLeft / ScaleTime);
		Particle->velocity += acceleration * dt;
		Particle->pos += Particle->velocity * dt;
		if (ScaleSetting == "In" && LifeLength - Particle->LifeLeft <= ScaleTime)
		{
			float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((LifeLength - Particle->LifeLeft) / ScaleTime);
			Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

		}
		else if (ScaleSetting == "Pop" && LifeLength - Particle->LifeLeft >= ScaleTime)
		{
			Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix()) * ScaleMultiplier;// currently testing with particle_gold ran.json

		}
		else if (ScaleSetting == "Out" && Particle->LifeLeft <= ScaleTime)
		{
			float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((ScaleTime - Particle->LifeLeft) / ScaleTime);
			Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

		}
		if (Fade == "Out")
		{
			//Particle->alpha = Particle->LifeLeft;
			Particle->alpha = (Particle->LifeLeft / FadeTime);
		}
		else if (Fade == "In")
		{
			if (FadeTime > 1)
			{
				Particle->alpha = 1 - (Particle->LifeLeft - ((LifeLength - FadeTime) / FadeTime));

			}
			else
			{
				Particle->alpha = 1 - ((Particle->LifeLeft - (LifeLength - FadeTime)) * FadeTime);

			}
		}
		if (FrameCount > 0)
		{
			Particle->FrameTimer -= dt;
			if (Particle->FrameIndex < FrameCount && Particle->FrameTimer <= 0)
			{
				Particle->FrameIndex += 1;
				Particle->FrameTimer = FrameDuration;
			}
			else if (Particle->FrameIndex >= FrameCount && IsLooping)
			{
				Particle->FrameIndex = 0;
			}
		}
}
inline void UpdateParticlePositionHiveAndMove(particle* Particle, GameObjectPtr parent, float dt, std::string Fade, float FadeTime, std::string ScaleSetting, float ScaleMultiplier, float ScaleTime, float LifeLength,glm::vec2 acceleration,float FrameDuration,int FrameCount, bool IsLooping)
{
	
		auto ParentPhys = parent->GetParent()->Has(Physics);

		if (ParentPhys)
		{
			Particle->pos += glm::vec2(ParentPhys->GetVelocity()) * dt;
			Particle->velocity += acceleration * dt;
			Particle->pos += Particle->velocity * dt;
			if (ScaleSetting == "In" && LifeLength - Particle->LifeLeft <= ScaleTime)
			{
				float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((LifeLength - Particle->LifeLeft) / ScaleTime);
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

			}
			else if (ScaleSetting == "Pop" && LifeLength - Particle->LifeLeft >= ScaleTime)
			{
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * ScaleMultiplier;// currently testing with particle_gold ran.json

			}
			else if (ScaleSetting == "Out" && Particle->LifeLeft <= ScaleTime)
			{
				float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((ScaleTime - Particle->LifeLeft) / ScaleTime);
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

			}
			if (Fade == "Out")
			{
				//Particle->alpha = Particle->LifeLeft;
				Particle->alpha = (Particle->LifeLeft / FadeTime);
			}
			else if (Fade == "In")
			{
				if (FadeTime > 1)
				{
					Particle->alpha = 1 - (Particle->LifeLeft - ((LifeLength - FadeTime) / FadeTime));

				}
				else
				{
					Particle->alpha = 1 - ((Particle->LifeLeft - (LifeLength - FadeTime)) * FadeTime);

				}
			}
			if (FrameCount > 0)
			{
				Particle->FrameTimer -= dt;
				if (Particle->FrameIndex < FrameCount && Particle->FrameTimer <= 0)
				{
					Particle->FrameIndex += 1;
					Particle->FrameTimer = FrameDuration;
				}
				else if (Particle->FrameIndex >= FrameCount && IsLooping)
				{
					Particle->FrameIndex = 0;
				}
			}

			//poolTrans.SetRotation(poolTrans.GetRotation() + (ParentPhys->GetRotationalVelocity() * dt));
		}
}
inline void UpdateParticlePositionMove(particle* Particle, GameObjectPtr parent, float dt, std::string Fade, float FadeTime, std::string ScaleSetting, float ScaleMultiplier, float ScaleTime, float LifeLength,float FrameDuration, int FrameCount,bool IsLooping)
{
		
		auto ParentPhys = parent->GetParent()->Has(Physics);

		if (ParentPhys)
		{
			Particle->pos += glm::vec2( ParentPhys->GetVelocity()) * dt;
			Particle->velocity += Particle->acceleration * dt;
			Particle->pos += Particle->velocity * dt;
			if (ScaleSetting == "In" && LifeLength - Particle->LifeLeft <= ScaleTime)
			{
				float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((LifeLength - Particle->LifeLeft) / ScaleTime);
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

			}
			else if (ScaleSetting == "Pop" && LifeLength - Particle->LifeLeft >= ScaleTime)
			{
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * ScaleMultiplier;// currently testing with particle_gold ran.json

			}
			else if (ScaleSetting == "Out" && Particle->LifeLeft <= ScaleTime)
			{
				float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((ScaleTime - Particle->LifeLeft) / ScaleTime);
				Particle->scalerot = glm::mat2(parent->GetParent()->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

			}
			if (Fade == "Out")
			{
				//Particle->alpha = Particle->LifeLeft;
				Particle->alpha = (Particle->LifeLeft / FadeTime);
			}
			else if (Fade == "In")
			{
				if (FadeTime > 1)
				{
					Particle->alpha = 1 - (Particle->LifeLeft - ((LifeLength - FadeTime) / FadeTime));

				}
				else
				{
					Particle->alpha = 1 - ((Particle->LifeLeft - (LifeLength - FadeTime)) * FadeTime);

				}
			}
			if (FrameCount > 0)
			{
				Particle->FrameTimer -= dt;
				if (Particle->FrameIndex < FrameCount && Particle->FrameTimer <= 0)
				{
					Particle->FrameIndex += 1;
					Particle->FrameTimer = FrameDuration;
				}
				else if (Particle->FrameIndex >= FrameCount && IsLooping)
				{
					Particle->FrameIndex = 0;
				}
				
			}
			//poolTrans.SetRotation(poolTrans.GetRotation() + (ParentPhys->GetRotationalVelocity() * dt));
		}
	
}
inline void UpdateParticlePosition(particle* Particle, GameObjectPtr parent, float dt,std::string Fade, float FadeTime, std::string ScaleSetting, float ScaleMultiplier, float ScaleTime,float LifeLength,float FrameDuration,int FrameCount,bool IsLooping)
{
		Particle->velocity += Particle->acceleration * dt;
		Particle->pos += Particle->velocity * dt;
	if(ScaleSetting == "In" && LifeLength - Particle->LifeLeft<= ScaleTime)
	{
		float multiplier = 1.0f+ (ScaleMultiplier-1.0f)  *(( LifeLength - Particle->LifeLeft)/ScaleTime);
		Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix())* multiplier;// currently testing with particle_gold ran.json
		
	}
	else if (ScaleSetting == "Pop" && LifeLength - Particle->LifeLeft >= ScaleTime)
	{
		Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix()) * ScaleMultiplier;// currently testing with particle_gold ran.json

	}
	else if (ScaleSetting == "Out" && Particle->LifeLeft <= ScaleTime)
	{
		float multiplier = 1.0f + (ScaleMultiplier - 1.0f) * ((ScaleTime - Particle->LifeLeft)/ ScaleTime);
		Particle->scalerot = glm::mat2(parent->Has(Transform)->GetMatrix()) * multiplier;// currently testing with particle_gold ran.json

	}
	if (Fade == "Out")
	{
		//Particle->alpha = Particle->LifeLeft;
		Particle->alpha = Particle->LifeLeft;
	}
	else if (Fade == "In")
	{
		if (FadeTime >= 1)
		{
			Particle->alpha = 1 - (Particle->LifeLeft - ((LifeLength - FadeTime) / FadeTime));
		}
		else
		{
			Particle->alpha = 1 - ((Particle->LifeLeft - (LifeLength - FadeTime)) * FadeTime);

		}
	}
	//update animation
	if (FrameCount > 0)
	{
		Particle->FrameTimer -= dt;
		if(Particle->FrameIndex< FrameCount && Particle->FrameTimer<=0)
		{
			Particle->FrameIndex += 1;
			Particle->FrameTimer = FrameDuration;
		}
		else if (Particle->FrameIndex >= FrameCount && IsLooping)
		{

			Particle->FrameIndex = 0;
		}
	}
}

void ParticlePool::Draw()
{
	glm::mat4 par_mat(0);
	par_mat[2][2] = 1.0f;
	par_mat[3][3] = 1.0f;
	for (int i = 0; i < pool.size(); ++i)
	{
		par_mat[0][0] = pool[i].scalerot[0][0];
		par_mat[0][1] = pool[i].scalerot[0][1];
		par_mat[1][0] = pool[i].scalerot[1][0];
		par_mat[1][1] = pool[i].scalerot[1][1];
		par_mat[3][0] = pool[i].pos[0];
		par_mat[3][1] = pool[i].pos[1];
		if(uniformAnimation)
		{
			MEDrawParticle(*sprite->GetSpriteSource(), *sprite->GetMesh(), par_mat, sprite->GetFrame(), glm::vec4(sprite->GetRGB(), pool[i].alpha));
		}
		else
		{
			MEDrawParticle(*sprite->GetSpriteSource(), *sprite->GetMesh(), par_mat, pool[i].FrameIndex, glm::vec4(sprite->GetRGB(), pool[i].alpha));
		}
	}
}
void ParticlePool::Spawn(int numberOfParticles)
{
	particle Particle;
	Particle.acceleration = acceleration;
	Particle.LifeLeft = lifeLength;
	Particle.pos = glm::vec2(transform->GetTranslation());
	Particle.velocity = glm::vec3(0,0,0);
	if(Fade == "In")
	{
		Particle.alpha = 0;

	}
	else
	{
		Particle.alpha = 1;
	}
	Particle.FrameIndex = 0;
	Particle.FrameTimer = FrameDuration;
	Particle.scalerot = glm::mat2(transform->GetMatrix());

	for(int i = 0; i <numberOfParticles && pool.size() != MAX_SIZE;++i)
	{
		pool.push_back(Particle);
	}
}
void ParticlePool::SetSprite(Sprite* rhs)
{
	sprite = rhs;
}
void ParticlePool::SetTransform(TransformPtr rhs)
{
	transform = rhs;
}

void ParticlePool::SetMoveBool(bool move)
{
	ParticleMoveWithObject = move;
}

void ParticlePool::SetLifeLength(float length)
{
	lifeLength = length;
}

void ParticlePool::SetAcceleration(glm::vec2 accelerate)
{
	acceleration = accelerate;
}
void ParticlePool::SethiveMind(bool hive)
{
	hiveMind = hive;
}

void ParticlePool::SetUniformAnimation(bool UniformAnimation)
{
	uniformAnimation = UniformAnimation;
}

void ParticlePool::SetFrameDuration(float frameDuration)
{
	FrameDuration = frameDuration;
}



void ParticlePool::SetFade(std::string fade)
{
	Fade = fade;
}
void ParticlePool::SetFadeTime(float fadetime)
{
	FadeTime = fadetime;
}

void ParticlePool::SetFrameCount(int frameCount)
{
	FrameCount = frameCount;
}

void ParticlePool::SetScaleSetting(std::string scalesetting)
{
	ScaleSetting = scalesetting;
}

void ParticlePool::SetScaleMultiplier(float scaleMultiplier)
{
	ScaleMultiplier = scaleMultiplier;
}

void ParticlePool::SetScaleTime(float scaletime)
{
	ScaleTime = scaletime;
}

bool ParticlePool::GetFull()
{
	return pool.size() == MAX_SIZE;
}
void ParticlePool::SetIsLooping(bool isLooping)
{
	IsLooping = isLooping;
}
void ParticlePool::Update(float dt, GameObjectPtr parent)
{
	for(int i = 0; i < pool.size();++i)
	{
		pool[i].LifeLeft -= dt;
		if (pool[i].LifeLeft <= 0.0f)
		{
			pool.erase(pool.begin() + i);
			--i;
		}
		
	}
	if(!hiveMind && !ParticleMoveWithObject)
	{
		for (int i = 0; i < pool.size(); ++i)
		{
			UpdateParticlePosition(&pool[i], parent, dt, Fade, FadeTime, ScaleSetting, ScaleMultiplier, ScaleTime, lifeLength, FrameDuration, FrameCount, IsLooping);
		}
		
	}
	else if (hiveMind && !ParticleMoveWithObject)
	{
		for (int i = 0; i < pool.size(); ++i)
		{
			UpdateParticlePositionHive(&pool[i], parent, dt, Fade, FadeTime, ScaleSetting, ScaleMultiplier, ScaleTime, lifeLength, acceleration,FrameDuration,FrameCount, IsLooping);
			
		}
	}
	else if (!hiveMind && ParticleMoveWithObject)
	{
		for (int i = 0; i < pool.size(); ++i)
		{
			UpdateParticlePositionMove(&pool[i], parent, dt, Fade, FadeTime, ScaleSetting, ScaleMultiplier, ScaleTime, lifeLength,FrameDuration,FrameCount, IsLooping);
		}
	}
	else
	{
		for (int i = 0; i < pool.size(); ++i)
		{
			UpdateParticlePositionHiveAndMove(&pool[i], parent, dt, Fade, FadeTime, ScaleSetting, ScaleMultiplier, ScaleTime, lifeLength, acceleration,FrameDuration,FrameCount,IsLooping);
			
		}
	}
	
	
}

Emitter::Emitter()
	:Component(cEmitter),
	 PatternType(""),
	 ParticleMoveWithObject(false),
	 HiveMind(false),
	 UniformAnimation(true),
	 lifeLength(0),
	 SpawnRate(0),
	 SprayAngle(0),
	 MinSpeed(0),
	 MaxSpeed(0),
	 Fade(""),
	 FadeTime(0),
	 ScaleSetting(""),
	 ScaleMultiplier(0),
	 ScaleTime(0),
	 pool(new ParticlePool)
	
{
}
Emitter::Emitter(const Emitter& rhs)
	:Component(cEmitter),
	PatternType(rhs.PatternType),
	ParticleMoveWithObject(rhs.ParticleMoveWithObject),
	HiveMind(rhs.HiveMind),
	UniformAnimation(rhs.UniformAnimation),
	lifeLength(rhs.lifeLength),
	SpawnRate(rhs.SpawnRate),
	SprayAngle(rhs.SprayAngle),
	MinSpeed(rhs.MinSpeed),
	MaxSpeed(rhs.MaxSpeed),
	Fade(rhs.Fade),
	FadeTime(rhs.FadeTime),
	ScaleSetting(rhs.ScaleSetting),
	ScaleMultiplier(rhs.ScaleMultiplier),
	ScaleTime(rhs.ScaleTime),
	pool(new ParticlePool(*rhs.pool))
{
}


Emitter::~Emitter()
{
	delete pool;
}
//due to calculation methond odd numbered spray angles will not be exact
glm::vec2 Emitter::RandomAcceleration()
{
	// Convert the spray angle to radians        pi
	float sprayAngleRad = (SprayAngle/2) * 3.14159265358979323846f / 180.0f;
	float rotationRad = Parent()->GetComponent<Transform>(cTransform)->GetRotation() * 3.14159265358979323846f / 180.0f;
	// Generate a random angle within the range of -sprayAngle to sprayAngle
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-sprayAngleRad, sprayAngleRad);
	float angleRad = dis(gen);

	// Calculate the x and y components of the acceleration vector
	float accel_x = sin(angleRad);
	float accel_y = cos(angleRad);
	if (rotationRad == 0.0f)
	{
		return glm::vec2(accel_x, accel_y);
	}
	float rotated_x = accel_x * cos(rotationRad) + accel_y * sin(rotationRad);
	float rotated_y = accel_x * sin(rotationRad) - accel_y * cos(rotationRad);
	// Return the resulting acceleration vector as a glm::vec2
	return glm::vec2(rotated_x, rotated_y);
}
	void Emitter::spawn(int numOfParticles)
	{
		pool->Spawn(numOfParticles);
	}
	void Emitter::Read(const char* bufchar)
	{
		using namespace rapidjson;
		Document d;
		d.Parse(bufchar);

		const rapidjson::Value& ghead = d["GameObject"];
		const Value& head = ghead["Emitter"];
		if (head.HasMember("ParticleMoveWithObject"))
		{
			const Value& particleMoveWithObject = head["ParticleMoveWithObject"];
			ParticleMoveWithObject = particleMoveWithObject.GetBool();
		}
		if (head.HasMember("PatternType"))
		{
			const Value& pattterntype = head["PatternType"];
			PatternType = pattterntype.GetString();
		}
		if (head.HasMember("SpawnRate"))
		{
			const Value& rate = head["SpawnRate"];
			SpawnRate = rate.GetFloat();
		}
		if (head.HasMember("ParticleLife"))
		{
			const Value& lifelength = head["ParticleLife"];
			lifeLength = lifelength.GetFloat();
		}
		if (head.HasMember("SprayAngle"))
		{
			const Value& sprayangle = head["SprayAngle"];
			SprayAngle = sprayangle.GetInt();
		}
		if (head.HasMember("MinSpeed"))
		{
			const Value& sprayangle = head["MinSpeed"];
			MinSpeed = sprayangle.GetFloat();
		}
		if (head.HasMember("MaxSpeed"))
		{
			const Value& sprayangle = head["MaxSpeed"];
			MaxSpeed = sprayangle.GetFloat();
		}
		if (head.HasMember("HiveMind"))
		{
			const Value& move = head["HiveMind"];
			HiveMind= move.GetBool();
		}
		if (head.HasMember("UniformAnimation"))
		{
			const Value& move = head["UniformAnimation"];
			UniformAnimation = move.GetBool();
		}
		if (head.HasMember("Fade"))
		{
			const Value& fade = head["Fade"];
			Fade = fade.GetString();
		}
		if (head.HasMember("FadeTime"))
		{
			const Value& fadetime = head["FadeTime"];
			FadeTime = fadetime.GetFloat();
		}
		if (head.HasMember("ScaleSetting"))
		{
			const Value& fadetime = head["ScaleSetting"];
			ScaleSetting = fadetime.GetString();
		}
		if (head.HasMember("ScaleMultiplier"))
		{
			const Value& fadetime = head["ScaleMultiplier"];
			ScaleMultiplier = fadetime.GetFloat();
		}
		if (head.HasMember("ScaleTime"))
		{
			const Value& fadetime = head["ScaleTime"];
			ScaleTime = fadetime.GetFloat();
		}
	}

void Emitter::SetAcceleration(glm::vec2 Acceleration)
{
	pool->SetAcceleration(Acceleration*randomRange(MinSpeed,MaxSpeed));
}
void Emitter::SetAccelerationWithoutMultiplier(glm::vec2 Acceleration)
{
	pool->SetAcceleration(Acceleration);
}
float Emitter::GetSpawnRate()
{
	return SpawnRate;
}

void Emitter::SetSpawnRate(float new_rate)
{
	SpawnRate = new_rate;
}

std::string Emitter::GetPatternType()
{
	return PatternType;
}

bool Emitter::IsFull()
{
	return pool->GetFull();
}
Emitter* Emitter::Clone()const
{
	Emitter* component = new Emitter(*this);

	return component;
}

void Emitter::Init()
{
	pool->SetSprite(this->Parent()->GetComponent<Sprite>(cSprite));
	pool->SetLifeLength(lifeLength);
	pool->SetMoveBool(ParticleMoveWithObject);
	pool->SethiveMind(HiveMind);
	pool->SetUniformAnimation(UniformAnimation);
	pool->SetTransform(this->Parent()->GetComponent<Transform>(cTransform));
	pool->SetFade(Fade);
	pool->SetFadeTime(FadeTime);
	pool->SetScaleSetting(ScaleSetting);
	pool->SetScaleMultiplier(ScaleMultiplier);
	pool->SetScaleTime(ScaleTime);
	if (Parent()->Has(Animation))
	{
		pool->SetFrameDuration(Parent()->Has(Animation)->getFrameDuration());
		pool->SetFrameCount(Parent()->Has(Animation)->getFrameCount());
		pool->SetIsLooping(Parent()->Has(Animation)->isLooping());
	}
	
}
void Emitter::Exit()
{
}
void Emitter::Update(float dt)
{
	pool->Update(dt, this->Parent());
}
void Emitter::Draw()const
{
	pool->Draw();
}


