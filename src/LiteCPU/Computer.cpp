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
	JMP = 0x4C,
	ROR = 0x6A,
	STA = 0x8D,
	LDA = 0xA9,
	NOP = 0xEA
};

enum BitFlags
{
	BIT_NEGATIVE	= 0x80,
	BIT_OVERFLOW	= 0x40,
	BIT_IGNORED		= 0x20,
	BIT_BREAK		= 0x10,
	BIT_DECIMAL		= 0x08,
	BIT_IRQ			= 0x04,
	BIT_ZERO		= 0x02,
	BIT_CARRY		= 0x01
};

namespace LiteCPU
{	
	CPU::CPU()
	{
		//empty
	}

	const char *CPU::GetStateName() const noexcept
	{
		switch (m_kState)
		{
			case States::RESET:
				return "Reset";

			case States::RUN:
				return "Run";

			case States::HALT:
				return "Halt";

			default:
				return "???";
		}
	}

	const char *CPU::GetOpCodeName() const noexcept
	{
		switch (m_uOpCode)
		{
			case JMP:
				return "JMP";

			case ROR:
				return "ROR";

			case STA:
				return "STA";

			case LDA:
				return "LDA";

			case NOP:
				return "NOP";

			default:
				return "???";
		}
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

	void CPU::ClearFlag(uint8_t bit) noexcept
	{
		F = F & ~bit;
	}

	void CPU::SetFlag(uint8_t bit) noexcept
	{
		F = F | bit;
	}

	void CPU::UpdateRegisterFlags(uint8_t registerValue) noexcept
	{
		if (registerValue == 0)
			this->SetFlag(BIT_ZERO);
		else
			this->ClearFlag(BIT_ZERO);

		if(registerValue & 0x80)
			this->SetFlag(BIT_NEGATIVE);
		else
			this->ClearFlag(BIT_NEGATIVE);
	}


	inline void UpdateRegisterFlags(uint8_t registerValue, uint8_t bit) noexcept;

	void CPU::RunTick()
	{
		auto currentStage = m_uStage++;		

		switch (currentStage)
		{
			//fetch instruction
			case 0:
				m_uOpCode = m_tMemory[PC++];
				break;

			case 1:
				//Run OPCODE
				switch (m_uOpCode)
				{
					case ROR:
					{						
						bool newCarry = A & 0x01;

						this->A = A >> 1;
						this->A = A | ((F & BIT_CARRY) ? 0x80 : 0);

						this->UpdateRegisterFlags(this->A);

						if (newCarry)
							this->SetFlag(BIT_CARRY);
						else
							this->ClearFlag(BIT_CARRY);

						m_uStage = 0;
						break;
					}						

					case JMP:
					case STA:
						//read low byte
						this->m_uBus = m_tMemory[PC++];
						break;

					case LDA:
						this->A = m_tMemory[PC++];
						this->UpdateRegisterFlags(this->A);

						m_uStage = 0;
						break;

					//do nothing?
					case NOP:
						m_uStage = 0;
						break;

					default:
						spdlog::error("[CPU::RunTick] Unknown opcode - stage 1: {:x}", m_uOpCode);
						m_kState = States::HALT;
						break;
				}
				break;

			case 2:
				switch (m_uOpCode)
				{
					case JMP:
					case STA:					
						//read high byte
						this->m_uBus = this->m_uBus | (m_tMemory[PC++] << 8);
						break;

					default:
						spdlog::error("[CPU::RunTick] Unknown opcode - stage 2: {:x}", m_uOpCode);
						m_kState = States::HALT;
						break;
				}
				break;

			case 3:
				switch (m_uOpCode)
				{
					case JMP:
						this->PC = this->m_uBus;
						m_uStage = 0;
						break;

					case STA:
						m_tMemory[this->m_uBus] = this->A;
						m_uStage = 0;
						break;

					default:
						spdlog::error("[CPU::RunTick] Unknown opcode - stage 3: {:x}", m_uOpCode);
						m_kState = States::HALT;
						break;
				}
				break;
		}
	}

	void CPU::ResetTick()
	{
		switch (m_uStage)
		{
			case 0:
				this->F = 0x36;
				break;

			case 8:		
				PC = ((uint16_t)(m_tMemory[0xFFFD]) << 8) | (m_tMemory[0xFFFC]);
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
			spdlog::error("[CPU::LoadRom] Rom is too big, baseAddress + rom: {:x}", size + baseAddress);

			goto CLEANUP;
		}

		fseek(fp, 0, SEEK_SET);
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



