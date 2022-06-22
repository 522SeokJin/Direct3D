#pragma once
#include <GameEngine/GameEngineActor.h>

struct ResultColor
{
public:
	float4 MulColor = float4::ONE;
	float4 PlusColor = float4::ZERO;
};

// Ό³Έν : 
class SKySphereActor : public GameEngineActor
{
public:
	SKySphereActor();
	~SKySphereActor();

protected:
	SKySphereActor(const SKySphereActor& _other) = delete; 
	SKySphereActor(SKySphereActor&& _other) noexcept = delete;
	SKySphereActor& operator=(const SKySphereActor& _other) = delete;
	SKySphereActor& operator=(const SKySphereActor&& _other) = delete;

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SetRadius(float _Radius);

public:
	ResultColor Data;
	GameEngineRenderer* Renderer;
};

