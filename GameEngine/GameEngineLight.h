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
	float4 LightDir; // ����Ʈ�� ������ ����
	float4 NegLightDir; // ����Ʈ�� ������ -����
};

struct LightsData
{
	int LightCount;
	LightData Lights[128];
};

// ���� :
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

