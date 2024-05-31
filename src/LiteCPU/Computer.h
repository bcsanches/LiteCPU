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

#include <cstdint>
#include <filesystem>

namespace LiteCPU
{
	constexpr auto RAM_SIZE = 0xFFFF + 1;

	enum class States
	{
		RESET,
		RUN,
		HALT
	};

	enum OpCodes
	{
		JSR = 0x20,
		PHA = 0x48,
		JMP = 0x4C,
		ROR = 0x6A,
		STA = 0x8D,
		TXS = 0x9A,
		LDX = 0xA2,
		LDA = 0xA9,
		LDA_ABS = 0xAD,
		NOP = 0xEA
	};

	const char *TryGetOpCodeName(const OpCodes code) noexcept;

	class MemoryBus
	{
		public:
			inline const uint8_t operator[](uint16_t index) const noexcept
			{
				return m_uRam[index];
			}

			inline uint8_t &operator[](uint16_t index) noexcept
			{
				return m_uRam[index];
			}

			bool LoadRom(const std::filesystem::path &path, uint16_t baseAddress);
			void FillMem(uint8_t value);

		private:
			uint8_t m_uRam[RAM_SIZE];
	};

	class CPU
	{
		public:
			//program counter
			uint16_t PC;

			uint8_t A;
			uint8_t X;
			uint8_t Y;

			//stack pointer
			uint8_t S;

			//flags
			uint8_t F;

			CPU();

			void Tick();

			bool LoadRom(const std::filesystem::path &path, uint16_t baseAddress);

			void FillMem(uint8_t value);

			void Reset();

			inline uint8_t GetOpCode() const noexcept
			{
				return m_uOpCode;
			}

			const char *GetOpCodeName() const noexcept;
			const char *GetStateName() const noexcept;

			inline uint8_t GetStage() const noexcept
			{
				return m_uStage;
			}

			inline uint16_t GetBus() const noexcept
			{
				return m_uBus;
			}

			inline States GetState() const noexcept
			{
				return m_kState;
			}

		private:
			void ResetTick();
			void RunTick();

			inline void Push(uint8_t data) noexcept;
			inline uint8_t Pop() noexcept;

			inline void ClearFlag(uint8_t bit) noexcept;
			inline void SetFlag(uint8_t bit) noexcept;

			inline void UpdateRegisterFlags(uint8_t registerValue) noexcept;

		private:
			MemoryBus m_tMemory;

			States m_kState = States::RESET;

			uint8_t m_uStage = 0;
			uint8_t m_uOpCode;

			uint16_t m_uBus;
	};


	class Computer
	{

	};
}