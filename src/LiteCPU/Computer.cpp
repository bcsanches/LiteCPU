// Copyright (C) 2019 - Bruno Sanches. See the COPYRIGHT
// file at the top-level directory of this distribution.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 
// This Source Code Form is "Incompatible With Secondary Licenses", as
// defined by the Mozilla Public License, v. 2.0.


#include "Computer.h"
#include <stdio.h>

#include <spdlog/spdlog.h>

enum OpCodes
{
	NOP = 0xEA
};

namespace LiteCPU
{	
	CPU::CPU()
	{
		//empty
	}

	void CPU::Tick()
	{
		switch (m_kState)
		{
			case States::RESET:
				this->ResetTick();
				break;

			case States::RUN:
				this->RunTick();
				break;
		}
	}

	void CPU::RunTick()
	{
		switch (m_uStage)
		{
			//fetch instruction
			case 0:
				m_uOpCode = m_tMemory[PC++];
				break;

			case 1:

				//Run OPCODE
				switch (m_uOpCode)
				{
					//do nothing?
					case NOP:
						m_uStage = 0;
						return;
				}
		}

		++m_uStage;
	}

	void CPU::ResetTick()
	{
		switch (m_uStage)
		{
			case 8:		
				PC = ((uint16_t)(m_tMemory[0xFFFC]) << 8) | (m_tMemory[0xFFFD]);
				m_kState = States::RUN;

				m_uStage = 0;
				return;
		}		

		++m_uStage;
	}

	void CPU::Reset()
	{
		m_kState = States::RESET;
		m_uStage = 0;
	}

	bool CPU::LoadRom(const std::filesystem::path &path, uint16_t baseAddress)
	{
		return m_tMemory.LoadRom(path, baseAddress);
	}

	void CPU::FillMem(uint8_t value)
	{
		m_tMemory.FillMem(value);
	}	

	bool MemoryBus::LoadRom(const std::filesystem::path &path, uint16_t baseAddress)
	{
		FILE *fp = fopen(path.string().c_str(), "rb");
		if (fp == nullptr)
		{
			spdlog::error("[CPU::LoadRom] Cannot open {}", path.string());

			return false;
		}

		fseek(fp, 0, SEEK_END);
		const size_t size = ftell(fp);

		bool result = false;

		if (size + baseAddress > RAM_SIZE)
		{			
			spdlog::error("[CPU::LoadRom] Rom is too big, baseAddress + rom: {}", size + baseAddress);

			goto CLEANUP;
		}

		fread(&m_uRam[baseAddress], 1, size, fp);
		result = true;

CLEANUP:
		fclose(fp);

		return result;
	}

	void MemoryBus::FillMem(uint8_t value)
	{
		memset(m_uRam, value, sizeof(m_uRam));
	}

}



