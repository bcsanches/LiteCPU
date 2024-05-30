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

#define MAGIC_ENUM_RANGE_MAX 256
#include <magic_enum/magic_enum.hpp>

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

constexpr auto MIN_ROWS = 10;

//
//
// We do not expect too many breakpoints, so do a simple and stupid linear search...
//
//

const Breakpoint *DebugWidget::TryFindOpcodeBreakPoint(LiteCPU::OpCodes opcode) const noexcept
{
	for (auto &b : m_vecBreakpoints) 
	{
		if (b.m_kType != BreakpointTypes::OPCODE)
			continue;

		if (b.m_kOpCode == opcode)
			return &b;
	}

	return nullptr;
}

const Breakpoint *DebugWidget::TryFindAddressBreakPoint(uint16_t address) const noexcept
{
	for (auto &b : m_vecBreakpoints)
	{
		if (b.m_kType != BreakpointTypes::ADDRESS)
			continue;

		if (b.m_uAddress == address)
			return &b;
	}

	return nullptr;
}

//
//
// This is ugly.... but....
//
//
bool DebugWidget::ShowEditBreakpointdialog(bool &p_open)
{    
	constexpr auto dialogName = "Add new breakpoint";

	ImGui::OpenPopup(dialogName);

	LiteCPU::OpCodes selectedOpcode = LiteCPU::OpCodes::NOP;

	static int address = 0;
	static bool validationError = false;

	if (m_fInitEditDialog)
	{
		assert(m_iSelected >= 0);

		address = m_vecBreakpoints[m_iSelected].m_uAddress;
	}

	bool result = false;

	if (ImGui::BeginPopupModal(dialogName, &p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		static int selectedType = 0;

		if (m_fInitEditDialog)
		{
			selectedType = m_vecBreakpoints[m_iSelected].m_kType == BreakpointTypes::OPCODE ? 0 : 1;
		}

		const char *types[] = { "OPCODE", "ADDRESS", nullptr };		

		if (ImGui::BeginCombo("Type", types[selectedType]))
		{
			for (int i = 0; types[i]; ++i)
			{
				if (ImGui::Selectable(types[i], i == selectedType))
				{
					validationError = false;
					selectedType = i;

					//do not think it is possible to change it on first run... but just in case
					m_fInitEditDialog = false;
				}
			}
			ImGui::EndCombo();
		}        
		ImGui::Separator();

		const auto breakPointType = static_cast<BreakpointTypes>(selectedType);

		if (breakPointType == BreakpointTypes::OPCODE)
		{
			static int selectedOpcodeIndex = 0;

			if (m_fInitEditDialog)
			{
				selectedOpcodeIndex = (int)magic_enum::enum_index(m_vecBreakpoints[m_iSelected].m_kOpCode).value();
			}

			//add magic enum and list opcodes
			auto opcodes = magic_enum::enum_entries<LiteCPU::OpCodes>();

			constexpr auto opcodesCount = magic_enum::enum_count<LiteCPU::OpCodes>();

			if (ImGui::BeginCombo("Opcode", opcodes[selectedOpcodeIndex].second.data()))
			{
				for (int i = 0; i < opcodesCount; ++i)
				{
					if (ImGui::Selectable(opcodes[i].second.data(), i == selectedOpcodeIndex))
					{
						validationError = false;
						selectedOpcodeIndex = i;
					}
				}

				ImGui::EndCombo();
			}
			
			selectedOpcode = opcodes[selectedOpcodeIndex].first;
		}
		else
		{	
			if (m_fInitEditDialog)
				address = m_vecBreakpoints[m_iSelected].m_uAddress;

			if (ImGui::InputInt("Address", &address, 1, 100, ImGuiInputTextFlags_CharsHexadecimal))
			{
				address = std::max(0, address);
				address = std::min(0xFFFF, address);

				validationError = false;
			}
		}			

		m_fInitEditDialog = false;		

		if (ImGui::Button("Ok"))
		{
			const auto newAddress = static_cast<uint16_t>(address);

			if(breakPointType == BreakpointTypes::OPCODE)
			{
				validationError = this->TryFindOpcodeBreakPoint(selectedOpcode) != nullptr;
			}
			else
			{	
				validationError = this->TryFindAddressBreakPoint(newAddress) != nullptr;																		
			}		
					
			if (!validationError)
			{
				if (m_fEditBreakpointDialog)
				{
					m_vecBreakpoints[m_iSelected].m_kType = breakPointType;
					m_vecBreakpoints[m_iSelected].m_kOpCode = selectedOpcode;
					m_vecBreakpoints[m_iSelected].m_uAddress = newAddress;
				}
				else
				{
					m_vecBreakpoints.push_back(Breakpoint
						{
							breakPointType,
							selectedOpcode,
							newAddress
						}
					);
				}

				p_open = false;
				result = true;
			}								
		}		

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{			
			p_open = false;
		}		

		if (validationError)
		{
			ImGui::SeparatorText("Error: Breakpoint already exists");
		}

		ImGui::EndPopup();		
	}

	return result;
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

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * (MIN_ROWS + 1));
		if (ImGui::BeginTable("Breakpoints", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY, outer_size))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Id");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Data");
			ImGui::TableSetupColumn("Disabled");
			ImGui::TableHeadersRow();

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

				ImGui::TableNextColumn();
				
				ImGui::Text(item.m_fDisabled ? "Yes" : "");

				++count;
			}
#if 0
			for(;count < MIN_ROWS; ++count)
			{                
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("");

				ImGui::TableNextColumn();
				ImGui::Text("");

				ImGui::TableNextColumn();
				ImGui::Text("");

				ImGui::TableNextColumn();
				ImGui::Text("");
			}
#endif
			ImGui::EndTable();
		}

		if (ImGui::Button("New"))
		{
			m_fAddBreakpointDialog = true;
		}

		ImGui::SameLine();
		
		if (ImGui::Button("Edit") && (m_iSelected >= 0))
		{
			m_fEditBreakpointDialog = true;

			//seems to be a hack...
			m_fInitEditDialog = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete") && (m_iSelected >= 0))
		{			
			m_vecBreakpoints.erase(m_vecBreakpoints.begin() + m_iSelected);

			if (m_iSelected >= m_vecBreakpoints.size())
				--m_iSelected;			
		}

		if (m_iSelected >= 0)
		{            
			ImGui::SameLine();

			if (m_vecBreakpoints[m_iSelected].m_fDisabled)
			{
				if (ImGui::Button("Enable"))
					m_vecBreakpoints[m_iSelected].m_fDisabled = false;                
			}
			else
			{
				if (ImGui::Button("Disable"))
					m_vecBreakpoints[m_iSelected].m_fDisabled = true;
			}                                 
		}    
	}

	ImGui::End();	

	if (m_fAddBreakpointDialog && ShowEditBreakpointdialog(m_fAddBreakpointDialog) && (m_iSelected < 0))
	{		
		m_iSelected = 0;
	}    

	if (m_fEditBreakpointDialog)
	{
		ShowEditBreakpointdialog(m_fEditBreakpointDialog);
	}
}
