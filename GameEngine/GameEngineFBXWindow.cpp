#include "PreCompile.h"
#include "GameEngineFBXWindow.h"
#include <iostream>
#include "GameEngineFBXMeshManager.h"
#include "GameEngineCore.h"


GameEngineFBXWindow::GameEngineFBXWindow()
    : Select(-1)
{
}

GameEngineFBXWindow::~GameEngineFBXWindow()
{
}

void GameEngineFBXWindow::OnGUI()
{
    std::vector<const char*> Arr;

    for (auto& Ref : GameEngineFBXMeshManager::GetInst().ResourcesMap)
    {
        Arr.push_back(Ref.first.c_str());
    }

    ImGui::BeginChildFrame(static_cast<ImGuiID>(
        reinterpret_cast<uint64_t>("##FBXRANGE")), { 200, 500 });
    ImGui::Text("FBXLIST");
    ImGui::ListBox("##FBXLIST", &Select, &Arr[0], static_cast<ImGuiID>(Arr.size()));
    ImGui::EndChildFrame();
    ImGui::SameLine();

    std::string info = "특이사항 ";

    static float Scroll = 0.0f;

    GameEngineFBXMesh* SelectMesh = nullptr;
    if (Select != -1)
    {
        SelectMesh = GameEngineFBXMeshManager::GetInst().Find(Arr[Select]);
    }

    ImGui::BeginChildFrame(static_cast<ImGuiID>(
        reinterpret_cast<uint64_t>("##NODETREERANGE"))
        , { (Scroll * 3) + 300 , 500 }/*, ImGuiWindowFlags_HorizontalScrollbar*/);

    if (nullptr != SelectMesh)
    {
        SelectMesh->RecursiveAllNode(
            [&] (fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _ParentReturn)
            {
                int Result = 0;

                std::string TypeName = " ";

                switch (_Type)
                {
                case fbxsdk::FbxNodeAttribute::eUnknown:
                    TypeName += "Unknown";
                    break;
                case fbxsdk::FbxNodeAttribute::eNull:
                    TypeName += "Null";
                    break;
                case fbxsdk::FbxNodeAttribute::eMarker:
                    TypeName += "Marker";
                    break;
                case fbxsdk::FbxNodeAttribute::eSkeleton:
                    TypeName += "Skeleton";
                    break;
                case fbxsdk::FbxNodeAttribute::eMesh:
                    TypeName += "Mesh";
                    break;
                case fbxsdk::FbxNodeAttribute::eNurbs:
                    TypeName += "Nurbs";
                    break;
                case fbxsdk::FbxNodeAttribute::ePatch:
                    TypeName += "Patch";
                    break;
                case fbxsdk::FbxNodeAttribute::eCamera:
                    TypeName += "Camera";
                    break;
                case fbxsdk::FbxNodeAttribute::eCameraStereo:
                    TypeName += "CameraStereo";
                    break;
                case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
                    TypeName += "CameraSwitcher";
                    break;
                case fbxsdk::FbxNodeAttribute::eLight:
                    TypeName += "Light";
                    break;
                case fbxsdk::FbxNodeAttribute::eOpticalReference:
                    TypeName += "OpticalReference";
                    break;
                case fbxsdk::FbxNodeAttribute::eOpticalMarker:
                    TypeName += "OpticalMarker";
                    break;
                case fbxsdk::FbxNodeAttribute::eNurbsCurve:
                    TypeName += "NurbsCurve";
                    break;
                case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
                    TypeName += "TrimNurbsSurface";
                    break;
                case fbxsdk::FbxNodeAttribute::eBoundary:
                    TypeName += "Boundary";
                    break;
                case fbxsdk::FbxNodeAttribute::eNurbsSurface:
                    TypeName += "NurbsSurface";
                    break;
                case fbxsdk::FbxNodeAttribute::eShape:
                    TypeName += "Shape";
                    break;
                case fbxsdk::FbxNodeAttribute::eLODGroup:
                    TypeName += "LODGroup";
                    info += "LODGroup";
                    break;
                case fbxsdk::FbxNodeAttribute::eSubDiv:
                    TypeName += "SubDiv";
                    break;
                case fbxsdk::FbxNodeAttribute::eCachedEffect:
                    TypeName += "CachedEffect";
                    break;
                case fbxsdk::FbxNodeAttribute::eLine:
                    TypeName += "Line";
                    break;
                default:
                    break;
                }

                if (1 == _ParentReturn)
                {
                    std::string DisName = _Node->GetName() + TypeName;

                    Result = ImGui::TreeNodeEx(DisName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf);

                    if (Scroll <= ImGui::GetScrollMaxX())
                    {
                        Scroll = ImGui::GetScrollMaxX();
                    }

                }
                return Result;
            }
            ,
            [&](fbxsdk::FbxNodeAttribute::EType _Type, fbxsdk::FbxNode* _Node, int _StartReturn)
            {
                if (_StartReturn == 1)
                {
                    ImGui::TreePop();
                }
            },
            1
        );
    }

    ImGui::EndChildFrame();

    if (nullptr != SelectMesh)
    {
        if (ImGui::Button("MeshLoad"))
        {
            SelectMesh->MeshLoad();
            SelectMesh->CreateRenderingBuffer();
        }

        if (ImGui::Button("MeshCreate"))
        {
            if (0 == SelectMesh->GetMeshSet().size())
            {
                SelectMesh->MeshLoad();
                SelectMesh->CreateRenderingBuffer();
            }

            if (0 != SelectMesh->GetMeshSet().size())
            {
                for (size_t MeshSetIndex = 0; MeshSetIndex < SelectMesh->GetMeshSet().size(); MeshSetIndex++)
                {
                    for (size_t VertexBufferIndex = 0; VertexBufferIndex < SelectMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers.size(); VertexBufferIndex++)
                    {
                        for (size_t IndexBufferIndex = 0; IndexBufferIndex < SelectMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex].size(); IndexBufferIndex++)
                        {

                            GameEngineVertexBuffer* VertexBuffer = SelectMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers[VertexBufferIndex];
                            GameEngineIndexBuffer* IndexBuffer = SelectMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex][IndexBufferIndex];

                            GameEngineActor* NewActor = GameEngineCore::CurrentLevel()->CreateActor<GameEngineActor>();

                            GameEngineRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineRenderer>(NewActor->GetTransform());
                            Renderer->SetRenderingPipeLine("Color");
                            Renderer->SetMesh(VertexBuffer, IndexBuffer);
                            Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
                            Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
                            Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 0.0f));
                        }
                    }
                }
            }
            else
            {
                GameEngineDebug::MsgBox("매쉬노드 존재하지 않는 FBX 입니다.");
            }
        }
    }

    ImGui::Text(GameEngineString::AnsiToUTF8Return(info).c_str());

}