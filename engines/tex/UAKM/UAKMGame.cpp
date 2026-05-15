/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "UAKMGame.h"
#include "Utilities.h"
// ... (otros includes)

/**
 * Initializes the main game state.
 * It ensures the default player file exists and sets up the resource maps.
 */
BOOL CUAKMGame::Init()
{
	// Initialize Map and Dialogue Map systems, icons, and item databases
	if (CModuleController::Init(new CUAKMMap(), new CUAKMDMap()) && LoadIcons() && CItems::Init())
	{
		// Check for the default player profile (TEX___00.PLR)
		if (!CFile::Exists(L"PLAYERS\\TEX___00.PLR"))
		{
			CFile file;
			if (file.Open(L"PLAYERS\\TEX___00.PLR", CFile::Mode::Write))
			{
				// Initialize a new player buffer with the name "TEX"
				BYTE buffer[256];
				ZeroMemory(buffer, 256);
				buffer[1] = 1; // Active flag
				buffer[2] = 'T'; buffer[3] = 'E'; buffer[4] = 'X';
				memset(buffer + 5, ' ', 21); // Pad name with spaces
				buffer[26] = 0; // Initial location or state flag
				
				file.Write(buffer, 256);
				file.Close();
			}
		}

		// Set initial game data state (all variables to zero)
		ZeroMemory(_gameData, UAKM_SAVE_SIZE);
		return TRUE;
	}

	return FALSE;
}

/**
 * Loads a saved game state from a .PLR file.
 * The game data (flags, inventory, location) is stored in a fixed-size buffer.
 */
BOOL CUAKMGame::LoadPlayer(int index)
{
	wchar_t fileName[MAX_PATH];
	swprintf(fileName, MAX_PATH, L"PLAYERS\\TEX___%02d.PLR", index);

	CFile file;
	if (file.Open(fileName, CFile::Mode::Read))
	{
		// Read the player name (25 bytes starting at offset 1)
		file.Seek(1, CFile::Begin);
		file.Read(_playerName, 25);
		_playerName[25] = 0;

		// Load the full game state buffer
		file.Seek(0, CFile::Begin);
		file.Read(_gameData, UAKM_SAVE_SIZE);
		file.Close();

		// Sync current timers with the loaded data
		for (int i = 0; i < 32; i++)
		{
			Timers[i] = GetWord(UAKM_SAVE_TIMERS_CURRENT + i * 2);
		}

		return TRUE;
	}
	return FALSE;
}

/**
 * Main Game Tick: Updates the real-time logic.
 * This is called every frame to handle cooldowns, MIDI synchronization, and ambient sounds.
 */
void CUAKMGame::Tick(int ticks)
{
	CGameBase::Tick(ticks);

	// UAKM uses a specific set of 32 timers for world events:
	// Timer 0-7: General script delays
	// Timer 10: Dialogue/Speech sync
	// Timer 11: MIDI music timing
	// Timer 12-14: Ambient sound effects (like cars in the street)

	for (int i = 0; i < 32; i++)
	{
		// Check if the timer is currently active in the save data
		if (_gameData[UAKM_SAVE_TIMERS + i] > 0)
		{
			// Reduce timer value by the number of milliseconds passed (ticks)
			Timers[i] = max(0, Timers[i] - ticks);

			// If timer reaches zero, reset it using the initial value stored in the save file
			if (Timers[i] == 0)
			{
				_gameData[UAKM_SAVE_TIMERS + i] = 0; // Mark as inactive
				
				// Reset current timer word to its initial baseline
				SetWord(UAKM_SAVE_TIMERS_CURRENT + i * 2, GetWord(UAKM_SAVE_TIMERS_INITIAL + i * 2));
			}
		}
	}
}

/**
 * Helper to write a 16-bit word into the game's data buffer (Little Endian).
 */
void CUAKMGame::SetWord(int offset, WORD value)
{
	if (offset >= 0 && offset < UAKM_SAVE_SIZE - 1)
	{
		_gameData[offset] = value & 0xFF;        // Low byte
		_gameData[offset + 1] = (value >> 8);    // High byte
	}
}

/**
 * Retrieves a byte from the game state (used extensively by the script engine).
 */
BYTE CUAKMGame::GetData(int offset)
{
	return (offset >= 0 && offset < UAKM_SAVE_SIZE) ? _gameData[offset] : 0;
}