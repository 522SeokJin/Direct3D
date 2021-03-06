#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "Monster.h"
#include "TopUI.h"
#include "Map.h"
#include <GameEngine/PostFade.h>
#include <GameEngine/PostBlur.h>
#include <GameEngine/SmallPostBlur.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include "UserGame.h"

PlayLevel::PlayLevel()
	: MActor(nullptr)
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::LevelStart()
{
	GameEngineFBXWindow* ImageWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	GameEngineDirectory Dir;
	Dir.MoveParent("Direct3D");
	Dir.MoveChild("EngineResources");
	Dir.MoveChild("FBX");

	std::vector<GameEngineFile> Files = Dir.GetAllFile("FBX");

	for (auto& File : Files)
	{
		GameEngineFBXMeshManager::GetInst().Load(File.GetFullPath());
	}
		
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));


	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MOn", 'p');
		GameEngineInput::GetInst().CreateKey("MOff", 'o');
		GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	}

}

void PlayLevel::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder
		&& false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;

	if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Check = true;
	}

	if (true == GameEngineInput::GetInst().Down("LevelControl"))
	{
		GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("LevelControlWindow");

		Window->OnOffChange();
	}

	if (true == GameEngineInput::GetInst().Down("MOn"))
	{
		GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow");

		Window->OnOffChange();
	}

	if (true == GameEngineInput::GetInst().Down("MOff"))
	{
		//Window->Off();
		//MActor->WindowCursorOff();

	}
}

void PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void PlayLevel::CreateActorLevel()
{
	static bool Check = false;
	
	if (true == Check)
	{
		return;
	}

	{
		MActor = CreateActor<MouseActor>();
		MActor->GetUIRenderer()->SetRenderGroup(1000);
	}

	{
		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.9f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });
	}

	{
		Player* Actor = CreateActor<Player>();
		Actor->GetTransform()->SetWorldPosition({0.0f, 200.0f, -500.0f});
	}

	{
		Map* Actor = CreateActor<Map>();
	}
	
	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
	}

	{
		TopUI* Actor = CreateActor<TopUI>();
		Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	}

	Check = true;
}