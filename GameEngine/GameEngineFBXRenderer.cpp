#include "PreCompile.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

GameEngineFBXRenderer::GameEngineFBXRenderer()
{
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
}


void GameEngineFBXRenderer::Render(float _DeltaTime)
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		RenderSets[i].ShaderHelper.Setting();
		RenderSets[i].PipeLine_->Rendering();
		RenderSets[i].ShaderHelper.ReSet();
		RenderSets[i].PipeLine_->Reset();
	}
}

void GameEngineFBXRenderer::SetRenderGroup(int _Order)
{
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Value)
{

}

void GameEngineFBXRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{

}
