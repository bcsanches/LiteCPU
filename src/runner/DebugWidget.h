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

#include <vector>

#include "Computer.h"

enum class BreakpointTypes
{
	OPCODE = 0,
	ADDRESS
};

struct Breakpoint
{
	BreakpointTypes		m_kType;

	LiteCPU::OpCodes	m_kOpCode;
	uint16_t			m_uAddress;

	bool				m_fDisabled = false;
};

class DebugWidget
{
	public:
		void Display();
		void Update() {};

		~DebugWidget() = default;

		void SetCPU(LiteCPU::CPU *cpu) noexcept
		{
			m_pCPU = cpu;
		}

		const Breakpoint *TryFindOpcodeBreakPoint(LiteCPU::OpCodes opcode) const noexcept;
		const Breakpoint *TryFindAddressBreakPoint(uint16_t address) const noexcept;

		void SelectBreakpoint(const Breakpoint &breakpoint) noexcept;

	private:
		bool ShowEditBreakpointdialog(bool &p_open);

	private:
		LiteCPU::CPU	*m_pCPU = nullptr;

		std::vector<Breakpoint> m_vecBreakpoints;

		int						m_iSelected = -1;

		bool					m_fAddBreakpointDialog = false;
		bool					m_fEditBreakpointDialog = false;

		bool					m_fInitEditDialog = false;
};
