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
	constexpr auto RAM_SIZE = 0xFFFF;

	enum class States
	{
		RESET,
		RUN
	};

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
			uint16_t PC;
			uint8_t A;
			uint8_t X;
			uint8_t Y;
			uint8_t S;

			uint8_t F;

			CPU();

			void Tick();

			bool LoadRom(const std::filesystem::path &path, uint16_t baseAddress);

			void FillMem(uint8_t value);

			void Reset();

		private:
			void ResetTick();
			void RunTick();

		private:
			MemoryBus m_tMemory;

			States m_kState = States::RESET;

			uint8_t m_uStage = 0;
			uint8_t m_uOpCode;
	};


	class Computer
	{

	};
}