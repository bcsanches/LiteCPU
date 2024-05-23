// Copyright (C) 2019 - Bruno Sanches. See the COPYRIGHT
// file at the top-level directory of this distribution.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 
// This Source Code Form is "Incompatible With Secondary Licenses", as
// defined by the Mozilla Public License, v. 2.0.

#include "DebugWidget.h"

#include <array>

#include <fmt/format.h>

#include "imgui.h"

#include "Computer.h"
#include "CPUWidget.h"

void DebugWidget::Display()
{
	assert(m_pCPU);

    ImGui::SetNextWindowPos(ImVec2{ CPU_WIDGET_WIDTH + 1, 20 });
	//ImGui::SetNextWindowSize(ImVec2{473, 0}, ImGuiCond_Always);

    static bool selected[10] = {};

	if (ImGui::Begin("Debugger"))
	{	
        if (ImGui::BeginTable("split2", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
        {
            for (int i = 0; i < 10; i++)
            {
                char label[32];
                sprintf(label, "Item %d", i);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Selectable(label, &selected[i], ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableNextColumn();
                ImGui::Text("Some other contents");
                ImGui::TableNextColumn();
                ImGui::Text("123456");
            }
            ImGui::EndTable();
        }
	}

	ImGui::End();	
}