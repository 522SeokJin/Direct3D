#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

// 설명 : 하나의 랜더 단위를 표현합니다.
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineRenderer : public GameEngineTransformComponent
{
private:
	friend GameEngineLevel;
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	void SetRenderingPipeLine(const std::string& _Value);

	virtual void SetRenderingPipeLineSettingNext() {}

	GameEngineShaderResHelper ShaderHelper;
	virtual void SetRenderGroup(int _Order);

	inline GameEngineRenderingPipeLine* GetGameEngineRenderingPipeLine()
	{
		return PipeLine_;
	}

	void SetMesh(const std::string& _Vtx, const std::string& _Idx);

	void SetMesh(const std::string& _Value);


protected:
	void Start() override;

	virtual void Render();

private:
	GameEngineRenderingPipeLine* PipeLine_;

	void Update(float _DeltaTime) override;


};

