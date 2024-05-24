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

void PrintBreakpointLabel(char *dest, const Breakpoint &bp)
{
    if (bp.m_kType == BreakpointTypes::OPCODE)
        sprintf(dest, "OPCODE %s", LiteCPU::TryGetOpCodeName(bp.m_kOpCode));
    else
        sprintf(dest, "ADDRESS %04X", bp.m_uAddress);        
}

void DebugWidget::Display()
{
	assert(m_pCPU);

    ImGui::SetNextWindowPos(ImVec2{ CPU_WIDGET_WIDTH + 1, 20 });
	ImGui::SetNextWindowSize(ImVec2{400, 0}, ImGuiCond_Appearing);

    static bool selected[10] = {};

	if (ImGui::Begin("Debugger"))
	{	
        ImGui::Text("Breakpoints");

#ifdef TABLE_MODE
        if (ImGui::BeginTable("Breakpoints", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
        {
#if 1
            int count = 0;

            bool itemAlreadySelected = m_iSelected >= 0;

            for (auto &item : m_vecBreakpoints)
            {
                char label[32];

                sprintf(label, "%d", count);                

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                bool selected = m_iSelected == count;

                if (ImGui::Selectable(label, &selected, ImGuiSelectableFlags_SpanAllColumns))
                {
                    if (selected)
                    {
                        m_iSelected = count;
                    }
                    else
                    {
                        m_iSelected = count == m_iSelected ? -1 : m_iSelected;
                    }                    
                }                                

                ImGui::TableNextColumn();
                ImGui::Text(item.m_kType == BreakpointTypes::OPCODE ? "OPCODE" : "ADDRESS");

                ImGui::TableNextColumn();
                if (item.m_kType == BreakpointTypes::OPCODE)
                {
                    ImGui::Text(LiteCPU::TryGetOpCodeName(item.m_kOpCode));
                }
                else
                {
                    sprintf(label, "%04X", item.m_uAddress);
                    ImGui::Text(label);
                }

                ++count;
            }
            ImGui::EndTable();

#else
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
#endif
        }
#endif

        char label[128];

        if (m_iSelected >= 0)
        {
            PrintBreakpointLabel(label, m_vecBreakpoints[m_iSelected]);
        }

        if (ImGui::BeginCombo("Breakpoints", m_iSelected >= 0 ? label : nullptr, 0))
        {
            int count = 0;
            for (auto &item : m_vecBreakpoints)
            {
                const bool is_selected = (count == m_iSelected);

                PrintBreakpointLabel(label, item);

                if (ImGui::Selectable(label, is_selected))
                    m_iSelected = count;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                ++count;
            }
            ImGui::EndCombo();
        }            

        if (ImGui::Button("New"))
        {
            static uint16_t counter = 5;

            m_vecBreakpoints.push_back(Breakpoint{ BreakpointTypes::ADDRESS, LiteCPU::OpCodes::NOP, static_cast<uint16_t>(0x0010 + counter)});

            if (m_iSelected < 0)
                m_iSelected = 0;

            ++counter;
        }

        ImGui::SameLine();
        ImGui::Button("Edit");

        ImGui::SameLine();
        if (ImGui::Button("Delete"))
        {
            if (m_iSelected >= 0)
            {
                m_vecBreakpoints.erase(m_vecBreakpoints.begin() + m_iSelected);

                if (m_iSelected >= m_vecBreakpoints.size())
                    --m_iSelected;
            }
        }
	}

	ImGui::End();	
}