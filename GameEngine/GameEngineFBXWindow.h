#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include <unordered_map>
#include "GameEngineFBXMeshManager.h"

// ���� :
class GameEngineFBXWindow : public GameEngineGUIWindow
{
	struct RenderTargetDrawData
	{
	public:
		std::string Name;
		GameEngineRenderTarget* Target;
		float4 Size_;
		bool ScaleCheck;
	};

public:
	// constrcuter destructer
	GameEngineFBXWindow();
	~GameEngineFBXWindow();

	// delete Function
	GameEngineFBXWindow(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow(GameEngineFBXWindow&& _Other) noexcept = delete;
	GameEngineFBXWindow& operator=(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow& operator=(GameEngineFBXWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

public:

private:
	// GameEngineFBXMesh FBXMesh;

};

