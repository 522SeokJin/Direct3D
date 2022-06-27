#pragma once
#include "GameEngineFBX.h"

struct FbxExMeshInfo
{
	std::string Name;
	fbxsdk::FbxMesh* Mesh;
	bool bTriangulated;

	unsigned __int64 UniqueId;
	int FaceNum;
	int VertexNum;
	int MaterialNum;

	// 애니메이션과 연결되는 매쉬인가?
	bool bIsSkelMesh;
	std::string SkeletonRoot;
	int SkeletonElemNum;

	bool bIsLodGroup;
	std::string LODGroup;
	// int LODLevel;
	int MorphNum;

	FbxExMeshInfo()
	{
		Name = "";
		UniqueId = 0;
		FaceNum = 0;
		VertexNum = 0;
		bTriangulated = false;
		MaterialNum = 0;
		bIsSkelMesh = false;
		SkeletonRoot = "";
		SkeletonElemNum = 0;
		bIsLodGroup = false;
		LODGroup = "";
		//LODLevel = 0;
		MorphNum = 0;
	}
};

// 설명 : 
class GameEngineFBXMesh : public GameEngineFBX
{
public:
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

	void Load(const std::string& _Path);
	void Initialize();
	void MeshNodeCheck();

protected:
	GameEngineFBXMesh(const GameEngineFBXMesh& _other) = delete; 
	GameEngineFBXMesh(GameEngineFBXMesh&& _other) noexcept = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _other) = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh&& _other) = delete;

private:
	std::vector<FbxExMeshInfo> MeshInfos;

	// Lod
	// 부모쪽으로 계속 올라가면서 LOD 그룹이 나올때까지 찾는 함수
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

};

