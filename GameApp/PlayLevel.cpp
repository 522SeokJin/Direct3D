#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "Monster.h"
#include "TopUI.h"
#include "Map.h"
#include <GameEngine\PostFade.h>
#include <GameEngine\PostBlur.h>
#include <GameEngine\SmallPostBlur.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include "UserGame.h"

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::LevelStart()
{

	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));


	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MOn", 'p');
		GameEngineInput::GetInst().CreateKey("MOff", 'o');
		GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	}

	//float _Dir,// 1
	//float Speed, // 
	//int Clip = 0

	/*FadeEffect = AddPostProcessCameraMergeNext<PostFade>();
	FadeEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());

	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	float4 Size = { 128, 72 };
	Window->PushRenderTarget("PostEffectFade", FadeEffect->GetResult(), Size * 3);

	FadeEffect->set(10, "BlurFilter.png");*/

	//{
	//	SmallPostBlur* BlurEffect = AddPostProcessCameraMergeNext<SmallPostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}


	//{
	//	SmallPostBlur* BlurEffect = AddPostProcessCameraMergeNext<SmallPostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}

	//{
	//	SmallPostBlur* BlurEffect = AddPostProcessCameraMergeNext<SmallPostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}


	//{
	//	PostBlur* BlurEffect = AddPostProcessCameraMergeNext<PostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}

	//{
	//	PostBlur* BlurEffect = AddPostProcessCameraMergeNext<PostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}

	//{
	//	PostBlur* BlurEffect = AddPostProcessCameraMergeNext<PostBlur>();
	//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
	//	BlurEffect->SetFilter("BlurFilter.png");
	//}


}

void PlayLevel::LevelUpdate(float _DeltaTime)
{
	// static GameEngineRenderWindow* Window = nullptr;

	// GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");

	// Player* Player = FindActor("Player");

	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder
		&& false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;

	/*

*/


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

// 지금 내가 보스방이야.
// 지금 내가 보스방이야.
void PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	

	//if (std::string::npos != _NextLevel->GetName().find("World")
	//	&& std::string::npos != _NextLevel->GetName().find("Boss"))
	//{

	//	Player::MainPlayer->GetLevel()->GetLevelActorMove(_NextLevel, Player::MainPlayer);

	//}

	// MoveLevelActor("TitleLevel", "BossLevel");

}
void PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void PlayLevel::CreateActorLevel()
{
	GameEngineTexture* Texture = GameEngineTextureManager::GetInst().Find("Animation.png");
	Texture->Cut(8, 8);

	{
		MActor = CreateActor<MouseActor>();
		MActor->GetUIRenderer()->SetRenderGroup(1000);
	}

	{
		Monster* Actor = CreateActor<Monster>();
		Actor->GetTransform()->SetWorldPosition(float4(200.0f, 0.0f, 0.0f));
	}

	{
		Map* Player = CreateActor<Map>();
	}

	if (nullptr == Player::MainPlayer)
	{
		Player* Actor = CreateActor<Player>();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(Actor->GetTransform()->GetLocalPosition());
	}


	{
		TopUI* Actor = CreateActor<TopUI>();
		Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	}
}