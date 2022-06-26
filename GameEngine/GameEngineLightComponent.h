#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

struct LightData
{
	float4 ViewLightDir; // ����Ʈ�� ������ ����
	float4 ViewNegLightDir; // ����Ʈ�� ������ -����
};

struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// ���� :
class GameEngineLightComponent : public GameEngineTransformComponent
{
public:
	// constrcuter destructer
	GameEngineLightComponent();
	~GameEngineLightComponent();

	// delete Function
	GameEngineLightComponent(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent(GameEngineLightComponent&& _Other) noexcept = delete;
	GameEngineLightComponent& operator=(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent& operator=(GameEngineLightComponent&& _Other) noexcept = delete;

	const LightData& GetLightData() 
	{
		return LightDataObject;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	LightShapeType ShapeType;
	LightData LightDataObject;
};

