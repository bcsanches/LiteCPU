// Copyright (C) 2019 - Bruno Sanches. See the COPYRIGHT
// file at the top-level directory of this distribution.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 
// This Source Code Form is "Incompatible With Secondary Licenses", as
// defined by the Mozilla Public License, v. 2.0.

#include "CPUWidget.h"

#include <array>

#include <fmt/format.h>

#include "imgui.h"

#include "Computer.h"
#include "DebugWidget.h"

void CPUWidget::Display()
{
	assert(m_pCPU);

	const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2{CPU_WIDGET_WIDTH, 0}, ImGuiCond_Always);

	if (ImGui::Begin("CPU Controller"))
	{	
		fmt::memory_buffer input;

		if (ImGui::BeginTable("CPU", 8, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings))
		{
			ImGui::TableSetupColumn("PC",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 58);
			ImGui::TableSetupColumn("AC",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("XR",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("YR",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("SR",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("SP",		ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("NV-BDIZC", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 99);
			ImGui::TableSetupColumn("MBus", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoSort, 58);
			ImGui::TableHeadersRow();

#if 0
			ImGui::TableNextColumn(); 
			ImGui::Text("PC");

			ImGui::TableNextColumn();
			ImGui::Text("AC");

			ImGui::TableNextColumn();
			ImGui::Text("XR");

			ImGui::TableNextColumn();
			ImGui::Text("YR");

			ImGui::TableNextColumn();
			ImGui::Text("SR");

			ImGui::TableNextColumn();
			ImGui::Text("SP");

			ImGui::TableSetupColumn("flags", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableNextColumn();			
			ImGui::Text("NV-BDIZC");

			ImGui::TableNextRow();
#endif
			
			fmt::format_to_n(std::back_inserter(input), 4, "{:04X}", m_pCPU->PC);
			input[4] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##PC", &input[0], 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 2, "{:02X}", m_pCPU->A);
			input[2] = '\0';			

			ImGui::TableNextColumn();			
			ImGui::InputText("##AC", &input[0], 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 2, "{:02X}", m_pCPU->X);
			input[2] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##XR", &input[0], 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 2, "{:02X}", m_pCPU->Y);
			input[2] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##YR", &input[0], 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 2, "{:02X}", m_pCPU->F);
			input[2] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##SR", &input[0], 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 2, "{:02X}", m_pCPU->S);
			input[2] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##SP", &input[0], 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 8, "{:08b}", m_pCPU->F);
			input[8] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##FLAGS", &input[0], 9, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			input.clear();
			fmt::format_to_n(std::back_inserter(input), 4, "{:04X}", m_pCPU->GetBus());
			input[4] = '\0';

			ImGui::TableNextColumn();
			ImGui::InputText("##BUS", &input[0], 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_ReadOnly);
			
			ImGui::EndTable();
		}

#if 0
		char memInput[10] = "0xAA00";
		ImGui::InputText("Memory", memInput, sizeof(memInput), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_ReadOnly);

		ImGui::Separator();
#endif
		
		if (ImGui::Button("Start"))
		{
			m_fPaused = false;
		}

		ImGui::SameLine();		

		if (ImGui::Button("Stop"))
		{
			m_fPaused = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Step") && m_fPaused)
			m_pCPU->Tick();

		ImGui::SameLine();

		if (ImGui::Button("Reset"))
		{
			m_pCPU->Reset();
			m_fPaused = true;
		}

		ImGui::SameLine();

		input.clear();
		fmt::format_to_n(std::back_inserter(input), 2, "{:2X}", m_pCPU->GetOpCode());
		input[2] = '\0';

		ImGui::Text(&input[0]);		

		ImGui::SameLine();		
		ImGui::Text(m_pCPU->GetOpCodeName());

		ImGui::SameLine();
		ImGui::Text("Stage: %s - %d", m_pCPU->GetStateName(), m_pCPU->GetStage());
		
		if (ImGui::InputInt("Ticks per second", &m_iTicks))
		{
			m_iTicks = std::max(0, m_iTicks);
		}		

		if (m_fBreakpointHit)
		{
			ImGui::Separator();

			ImGui::Text("Breakpoint hit");

			ImGui::SameLine();

			if (ImGui::Button("Clear"))
				m_fBreakpointHit = false;			
		}
	}

	ImGui::End();

	if (!m_fPaused)
	{
		auto &io = ImGui::GetIO();

		m_fpTicksToRun += io.DeltaTime * m_iTicks;

		while (m_fpTicksToRun > 1)
		{
			m_pCPU->Tick();
			m_fpTicksToRun -= 1;

			if (m_pCPU->GetState() != LiteCPU::States::RUN)
				continue;

			if (m_pCPU->GetStage() == 1)
			{
				//just have fetch instruction
				auto opcode = static_cast<LiteCPU::OpCodes>(m_pCPU->GetOpCode());

				if(auto breakpoint = m_pDebugWidget->TryFindOpcodeBreakPoint(opcode))				
				{
					this->OnBreakpointHit(*breakpoint);
					break;
				}
			}

			if(auto breakpoint = m_pDebugWidget->TryFindAddressBreakPoint(m_pCPU->PC))			
			{
				this->OnBreakpointHit(*breakpoint);
				break;
			}
		}
	}	
}

void CPUWidget::OnBreakpointHit(const Breakpoint &breakpoint)
{
	m_fPaused = true;
	m_fBreakpointHit = true;

	m_pDebugWidget->SelectBreakpoint(breakpoint);
}
