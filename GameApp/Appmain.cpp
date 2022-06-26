#include "PreCompile.h"

#include <conio.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/GameEngineFBX.h>
#include "UserGame.h"



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	GameEngineFBX ttttt;

	ttttt.Load();

	GameEngineCore::Start<UserGame>();
}