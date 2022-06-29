#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

#pragma warning( push )
#pragma warning ( disable : 26495 )
#include <fbxsdk.h>
#pragma warning( pop )

// 디버그 모드일때.
//#pragma comment(lib, "zlib-mt.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

class GameEngineFBX : public GameEngineObjectNameBase
{
public:
	GameEngineFBX();
	~GameEngineFBX();
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

public:
	void RecursiveAllNode(
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn);

	void Reset();

public:
	inline fbxsdk::FbxNode* GetRootNode()
	{
		return RootNode;
	}

protected:
	bool CreateFBXSystemInitialize(const std::string& _Path);
	void FBXConvertScene();
	void RecursiveAllNode(fbxsdk::FbxNode* _Node,
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn);

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

};

