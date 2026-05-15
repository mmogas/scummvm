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

#include "UAKMMainMenuModule.h"
#include "GameController.h"
#include "resource.h"
#include "Utilities.h"
#include "AmbientAudio.h"
#include "UAKMGame.h"
#include <codecvt>
#include <algorithm>

/**
 * Triggered when the user selects 'Intro' from the menu.
 * Stops all music and ambient sounds before loading the introduction sequence (DMap 29).
 */
void CUAKMMainMenuModule::Intro(LPVOID data)
{
	pMIDI->Stop();
	CAmbientAudio::StopAll();
	CAmbientAudio::Clear();
	// DMap 29 is the hardcoded identifier for the game's intro cinematic/script
	CGameController::LoadFromDMap(29);
}

/**
 * Triggered when the user selects 'Credits'.
 * Similar to Intro, it clears the audio state and loads the credits sequence (DMap 41).
 */
void CUAKMMainMenuModule::Credits(LPVOID data)
{
	pMIDI->Stop();
	CAmbientAudio::StopAll();
	CAmbientAudio::Clear();
	CGameController::LoadFromDMap(41);
}

/**
 * Initializes the main menu screen.
 * Sets up the background image (JPG resource) and creates the interactive buttons.
 */
void CUAKMMainMenuModule::SetupScreen()
{
	DWORD s;
	// Fetch the title screen background from the executable resources
	PBYTE pImg = GetResource(IDB_JPG_UAKM_TITLE, L\"JPG\", &s);
	CDXBitmap* pBmp = _pScreen->AddBitmap(pImg, s, Alignment::CenterX | Alignment::CenterY | Alignment::Scale | Alignment::Crop);

	// Button labels (should ideally be localized via string resources)
	char* pNG = "New game";
	char* pLG = "Load";
	char* pSG = "Save";
	char* pCf = "Config";
	char* pIn = "Intro";
	char* pCr = "Credits";
	char* pEx = "Exit";

	// Create UI buttons and link them to their respective member functions
	_pScreen->AddButton(pNG, 100, 100, &CUAKMMainMenuModule::NewGame, this);
	_pScreen->AddButton(pLG, 100, 140, &CUAKMMainMenuModule::LoadGameMenu, this);
	// ... (additional button setups)
}

/**
 * Scans the 'GAMES' directory to find valid savegame files.
 * It parses the file headers to extract metadata like Player Name, Location, and Game Day.
 */
void CUAKMMainMenuModule::RefreshSaveGames()
{
	_saveGames.clear();
	WIN32_FIND_DATA fd;
	// Look for all files in the GAMES subfolder
	HANDLE hFind = FindFirstFile(L"GAMES\\*", &fd);
	BYTE buffer[0xd0];

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Skip directories and the default empty save slot
			if (fd.nFileSizeLow > 0)
			{
				std::wstring name = std::wstring(fd.cFileName);
				if (name != L"SAVEGAME.000")
				{
					CFile file;
					std::wstring fileName = L"GAMES\\" + name;
					if (file.Open((LPWSTR)fileName.c_str()))
					{
						// Read the first 208 bytes which contain the header info
						if (file.Read(buffer, 0xd0) == 0xd0)
						{
							SaveGameInfo info;
							info.FileName = fileName;
							
							// Extract strings using offsets defined in UAKM_SAVE constants
							info.Player = std::string((const char*)(buffer + UAKM_SAVE_PLAYER), 21);
							info.Location = std::string((const char*)(buffer + UAKM_SAVE_LOCATION), 24);
							info.DayInGame = std::string("Day ") + std::to_string(buffer[UAKM_SAVE_GAME_DAY]);
							
							// Format the timestamp for the UI
							info.DateTime = FormatDateTime(buffer);
							
							_saveGames.push_back(info);
						}
					}
				}
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	
	// Sort saves so the most recent ones appear at the top
	std::sort(_saveGames.begin(), _saveGames.end(), [](const SaveGameInfo& a, const SaveGameInfo& b) {
		return a.DateTime > b.DateTime;
	});
}

/**
 * Logic for starting a fresh game.
 * Resets the game state and redirects the player to the first playable area.
 */
void CUAKMMainMenuModule::NewGame(LPVOID data)
{
	// Ensure the game controller is clean
	CGameController::Reset();
	// Push the game module to the controller stack to start gameplay
	CModuleController::Push(new CUAKMGame());
}