#include "PreCompile.h"
#include "SKySphereActor.h"
#include "GameEngineRenderer.h"

SKySphereActor::SKySphereActor()
{

}

SKySphereActor::~SKySphereActor()
{

}

void SKySphereActor::Start()
{
	{
		GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		Renderer->SetRenderingPipeLine("Texture");
		Renderer->SetMesh("Sphere");
		Renderer->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerFront");
		Renderer->ShaderHelper.SettingConstantBufferSet("TextureResultColor", Data);
		Renderer->ShaderHelper.SettingTexture("Tex", "EngineSky.jpg");

		Renderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	}
}

void SKySphereActor::Update(float _DeltaTime)
{
}

