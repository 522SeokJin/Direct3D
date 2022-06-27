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

	// �ִϸ��̼ǰ� ����Ǵ� �Ž��ΰ�?
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

// ���� : 
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
	// �θ������� ��� �ö󰡸鼭 LOD �׷��� ���ö����� ã�� �Լ�
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

};

