#pragma once
#include "GameEngineLightComponent.h"
#include "GameEngineActor.h"

// Ό³Έν : 
class LightActor : public GameEngineActor
{
public:
	LightActor();
	~LightActor();

	GameEngineLightComponent* GetLight()
	{
		return Light_;
	}

protected:
	LightActor(const LightActor& _other) = delete; 
	LightActor(LightActor&& _other) noexcept = delete;
	LightActor& operator=(const LightActor& _other) = delete;
	LightActor& operator=(const LightActor&& _other) = delete;

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	class GameEngineLightComponent* Light_;
};

