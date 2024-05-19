// Copyright (C) 2019 - Bruno Sanches. See the COPYRIGHT
// file at the top-level directory of this distribution.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 
// This Source Code Form is "Incompatible With Secondary Licenses", as
// defined by the Mozilla Public License, v. 2.0.

#include "MainApp.h"

#include "imgui_internal.h"


static void ImGuiDemoFunc()
{
	// Our state
	static bool show_demo_window = true;
	static bool show_another_window = false;

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");								// Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");					// Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);			// Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);				// Edit 1 float using a slider from 0.0f to 1.0f		

		if (ImGui::Button("Button"))								// Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGuiIO &io = ImGui::GetIO();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

static void ShowAboutWindow(bool *p_open)
{
	if (ImGui::Begin("About Lite CPU", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Text("Lite CPU %s", LITECPU_VERSION);
		ImGui::Separator();
		ImGui::Text("By Bruno C. Sanches");
		ImGui::Text("Licensed under the Mozilla License, see LICENSE for more information.");
	}

	ImGui::End();
}

MainApp::MainApp()
{
#if 0
	auto recentFile = Settings::GetLastProjectPath();

	m_wConsole.RegisterCommand(RName{ "App.Quit" }, [this](ConsoleCmdParams &params)
		{
			m_fKeepRunning = false;
		}
	);

	m_wConsole.RegisterCommand(RName{ "Editor.New" }, [this](ConsoleCmdParams &params)
		{
			this->NewFile();
		}
	);

	m_clBindings.Bind("Editor.New", SDL_SCANCODE_N, KEY_MODIFIER_CTRL);
	m_clBindings.Bind("Editor.Open", SDL_SCANCODE_O, KEY_MODIFIER_CTRL);
	m_clBindings.Bind("Editor.Save", SDL_SCANCODE_S, KEY_MODIFIER_CTRL);
	m_clBindings.Bind("Editor.SaveAs", SDL_SCANCODE_S, KEY_MODIFIER_CTRL | KEY_MODIFIER_SHIFT);

	m_wToolBar.RegisterCmds(m_wConsole, m_clBindings);

	m_clDocument.New();

	m_wDocumentWidget.SetDocument(&m_clDocument);
#endif

	//m_tCPU.FillMem(0xEA);
	m_tCPU.LoadRom("../BenEater/blink.bin", 0x8000);

	m_wCPU.SetCPU(&m_tCPU);
}

bool MainApp::Display()
{
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

	//disable padding, rouding and border
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);		

	ImGui::Begin("MainWindow", nullptr, window_flags);

	//restore padding, rouding and border
	ImGui::PopStyleVar(3);

	// Submit the DockSpace
	ImGuiIO &io = ImGui::GetIO();

	ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");		

	ImVec2 currentViewPortSize = viewport->WorkSize;	

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{			
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				//OpenFileDialog();
			}						

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
				m_fKeepRunning = false;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Show Demo Window", "", &m_fShowDemo);
			ImGui::MenuItem("Show Metrics Window", "", &m_fShowMetrics);
			ImGui::MenuItem("Show Debug Log Window", "", &m_fShowDebugLog);
			ImGui::MenuItem("Show Id Stack Tool Window", "", &m_fShowIdStackTool);

			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", nullptr, &m_fShowAbout);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();	

	m_wCPU.Display();

	if (m_fShowAbout)
		ShowAboutWindow(&m_fShowAbout);

	if (m_fShowDemo)
		ImGui::ShowDemoWindow(&m_fShowDemo);

	if (m_fShowMetrics)
		ImGui::ShowMetricsWindow(&m_fShowMetrics);

	if (m_fShowDebugLog)
		ImGui::ShowDebugLogWindow(&m_fShowDebugLog);

	if (m_fShowIdStackTool)
		ImGui::ShowIDStackToolWindow(&m_fShowIdStackTool);	

	return m_fKeepRunning;
}

void MainApp::HandleEvent(const SDL_KeyboardEvent &key)
{
}

void MainApp::Run()
{

}	
