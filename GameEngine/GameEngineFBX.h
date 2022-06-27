#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

#include <fbxsdk.h>

// 디버그 모드일때.
//#pragma comment(lib, "zlib-mt.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

// 릴리즈용 라이브러리
#pragma comment(lib, "libfbxsdk.lib")

// 설명 :
class GameEngineFBX : public GameEngineObjectNameBase
{
public:
	GameEngineFBX();
	~GameEngineFBX();

	inline fbxsdk::FbxNode* GetRootNode()
	{
		return RootNode;
	}

	void RecursiveAllNode(
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn
	);
	void Reset();

protected:
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

	bool CreateFBXSystemInitialize(const std::string& _Path);

	// 3DSMAX 는 오른손 좌표계, DirectX의 좌표계로 변경하기 위한 함수
	void FBXConvertScene();

	void RecursiveAllNode(fbxsdk::FbxNode* _Node,
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn);

private:

public:

protected:
	// 시작하려면 이녀석이 필요하다.
	fbxsdk::FbxManager* Manager;
	fbxsdk::FbxIOSettings* IOSetting;
	fbxsdk::FbxImporter* Importer;
	fbxsdk::FbxScene* Scene;

	fbxsdk::FbxNode* RootNode;
	fbxsdk::FbxAMatrix ConvertMatrix;
	fbxsdk::FbxAMatrix JointConvertMatrix;
	fbxsdk::FbxVector4 AxisVector;

private:

};

