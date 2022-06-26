#include "PreCompile.h"
#include "GameEngineLightComponent.h"

GameEngineLightComponent::GameEngineLightComponent() 
{
}

GameEngineLightComponent::~GameEngineLightComponent() 
{
}


void GameEngineLightComponent::Start() 
{
	//GetLevel()->GetMainCamera()->PushLight(this);
}

void GameEngineLightComponent::Update(float _DeltaTime)
{
	LightDataObject.ViewLightDir = GetTransform()->GetWorldForwardVector();
	// LightDataObject.NegLightDir = -LightDataObject.LightDir;
}