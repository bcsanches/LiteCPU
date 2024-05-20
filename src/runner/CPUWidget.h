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

namespace LiteCPU
{
	class CPU;
}


class CPUWidget
{
	public:
		void Display();
		void Update() {};

		~CPUWidget() = default;

		void SetCPU(LiteCPU::CPU *cpu) noexcept
		{
			m_pCPU = cpu;
		}

	private:
		LiteCPU::CPU	*m_pCPU = nullptr;

		int				m_iTicks = 20;
		float			m_fpTicksToRun = 0;

		bool			m_fPaused = true;
};
