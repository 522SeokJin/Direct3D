#include "PreCompile.h"
#include "GameEngineFBXWindow.h"
#include <iostream>
#include "GameEngineImageShotWindow.h"
#include "GameEngineFBXMeshManager.h"


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

    ImGui::BeginChildFrame(reinterpret_cast<ImGuiID>("##FBXRANGE"), {200, 500});
    ImGui::Text("FBXLIST");
    ImGui::ListBox("##FBXLIST", &Select, &Arr[0], Arr.size());
    ImGui::EndChildFrame();
    ImGui::SameLine();

    static float Scroll = 0.0f;

    GameEngineFBXMesh* SelectMesh = nullptr;
    if (Select != -1)
    {
        SelectMesh = GameEngineFBXMeshManager::GetInst().Find(Arr[Select]);
    }

    // ImGui::BeginChildFrame(reinterpret_cast<ImGuiID>("##NODETREERANGE"), { 200, 500 }, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);


    ImGui::BeginChildFrame(reinterpret_cast<ImGuiID>("##NODETREERANGE"), { (Scroll * 3) + 300 , 500 }/*, ImGuiWindowFlags_HorizontalScrollbar*/);

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
}