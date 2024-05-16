// Copyright (C) 2019 - Bruno Sanches. See the COPYRIGHT
// file at the top-level directory of this distribution.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 
// This Source Code Form is "Incompatible With Secondary Licenses", as
// defined by the Mozilla Public License, v. 2.0.

#pragma once

#include <SDL3/SDL.h>

#include "imgui.h"

#include "CPUWidget.h"

#define LITECPU_DISABLE_CLASS_COPY_AND_MOVE(X)	\
	X(const X &) = delete;						\
	X(X &&) = delete;							\
	X operator=(const X &) = delete;			\
	X &&operator=(X &&) = delete;
	
class MainApp
{
	public:
		MainApp();

		LITECPU_DISABLE_CLASS_COPY_AND_MOVE(MainApp);

		bool Display();
			
		void Run();			

		void HandleEvent(const SDL_KeyboardEvent &key);			

	private:		
		//
		//
		//Gui states
		// 
		ImVec2	m_vec2ViewportSize = { 0, 0 };

		CPUWidget	m_wCPU;

		bool	m_fShowAbout = false;
		bool	m_fShowDemo = false;
		bool	m_fShowMetrics = false;
		bool	m_fShowDebugLog = false;
		bool	m_fShowIdStackTool = false;

		bool	m_fKeepRunning = true;
};
