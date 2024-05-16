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

#include "imgui.h"

void CPUWidget::Display()
{
	ImGui::SetNextWindowSize(ImVec2{393, 100}, ImGuiCond_Always);

	if (ImGui::Begin("CPU Controller"))
	{	
		if (ImGui::BeginTable("CPU", 7, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings))
		{
			ImGui::TableSetupColumn("PC", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 58);
			ImGui::TableSetupColumn("AC", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("XR", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("YR", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("SR", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("SP", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 35);
			ImGui::TableSetupColumn("NV-BDIZC", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoSort, 80);
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

			char input[10] = "0000";

			ImGui::TableNextColumn();
			ImGui::InputText("##PC", input, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			strcpy(input, "00");

			ImGui::TableNextColumn();			
			ImGui::InputText("##AC", input, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			ImGui::TableNextColumn();
			ImGui::InputText("##XR", input, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			ImGui::TableNextColumn();
			ImGui::InputText("##YR", input, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			ImGui::TableNextColumn();
			ImGui::InputText("##SR", input, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			ImGui::TableNextColumn();
			ImGui::InputText("##SP", input, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

			strcpy(input, "00000000");
			ImGui::TableNextColumn();
			ImGui::InputText("##FLAGS", input, 9, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);
			
			ImGui::EndTable();
		}

#if 0
		char memInput[10] = "0xAA00";
		ImGui::InputText("Memory", memInput, sizeof(memInput), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_ReadOnly);

		ImGui::Separator();
#endif

		ImGui::Button("Start");
		ImGui::SameLine();		

		ImGui::Button("Stop");
		ImGui::SameLine();

		ImGui::Button("Step");
		ImGui::SameLine();

		ImGui::Button("Reset");		
	}

	ImGui::End();
}