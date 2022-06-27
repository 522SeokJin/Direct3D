#include "PreCompile.h"
#include "GameEngineFBXWindow.h"
#include <iostream>
#include "GameEngineImageShotWindow.h"
#include "GameEngineFBXMeshManager.h"


GameEngineFBXWindow::GameEngineFBXWindow()
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

    int Select = 0;

    ImGui::BeginChildFrame(reinterpret_cast<ImGuiID>("##FBXRANGE"), {200, 500});
    ImGui::Text("FBXLIST");
    ImGui::ListBox("##FBXLIST", &Select, &Arr[0], Arr.size());
    ImGui::EndChildFrame();
    ImGui::SameLine();
    ImGui::BeginChildFrame(reinterpret_cast<ImGuiID>("##NODETREERANGE"), { 200, 500 });
    if (ImGui::TreeNode("aaaa"))
    {
        if (ImGui::TreeNode("bbbb"))
        {
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
    ImGui::EndChildFrame();
}