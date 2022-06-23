#pragma once
#include "GameEngineActor.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

struct LightData
{
	float4 LightDir; // 라이트의 포워드 벡터
	float4 NegLightDir; // 라이트의 포워드 -벡터
};

struct LightsData
{
	int LightCount;
	LightData Lights[128];
};

// 설명 :
class GameEngineLight : public GameEngineActor
{
public:
	// constrcuter destructer
	GameEngineLight();
	~GameEngineLight();

	// delete Function
	GameEngineLight(const GameEngineLight& _Other) = delete;
	GameEngineLight(GameEngineLight&& _Other) noexcept = delete;
	GameEngineLight& operator=(const GameEngineLight& _Other) = delete;
	GameEngineLight& operator=(GameEngineLight&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	LightShapeType ShapeType;
	LightData* Data;
};

