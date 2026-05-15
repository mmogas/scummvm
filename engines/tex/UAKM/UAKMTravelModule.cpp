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

#include "UAKMTravelModule.h"
#include "Utilities.h"
#include "File.h"
#include "GameController.h"
#include "UAKMGame.h"
#include "LocationModule.h"
#include "VideoModule.h"
#include "AmbientAudio.h"
#include "AnimationController.h"

/**
 * Constructor: Initializes the list of available sub-locations in the game.
 * Each entry maps a name to a specific map index used by the engine.
 */
CUAKMTravelModule::CUAKMTravelModule()
{
	// Offset in the global save data (A array) where travel flags are stored
	_travelDataOffset = UAKM_SAVE_TRAVEL;

	// Hardcoded list of locations. The first parameter is the Map ID.
	_subLocations.push_back(new CSubLocation(1, "STREET or NEWSSTAND"));
	_subLocations.push_back(new CSubLocation(1, "BREW & STEW"));
	_subLocations.push_back(new CSubLocation(1, "SLICE O'HEAVEN PIZZA"));
	_subLocations.push_back(new CSubLocation(1, "ROOK'S PAWNSHOP"));
	_subLocations.push_back(new CSubLocation(1, "SNO WHITE WAREHOUSE"));
	_subLocations.push_back(new CSubLocation(1, "ALLEY (behind PAWNSHOP)"));
	_subLocations.push_back(new CSubLocation(1, "RUSTY'S FUN HOUSE"));
	_subLocations.push_back(new CSubLocation(1, "ELECTRONICS SHOP"));
	_subLocations.push_back(new CSubLocation(8, "LIBRARY"));
	// ... (additional locations omitted for brevity)
}

/**
 * Validates which locations should be visible to the player.
 * A location appears in the travel menu only if its corresponding 
 * flag in the game save data (A-array) is set to 1.
 */
void CUAKMTravelModule::RefreshAvailableLocations()
{
	_availableLocations.clear();
	for (size_t i = 0; i < _subLocations.size(); i++)
	{
		// Check if the location has been discovered/unlocked in the story
		if (pGame->GetData(_travelDataOffset + i) == 1)
		{
			_availableLocations.push_back(_subLocations[i]);
		}
	}
}

/**
 * Handles the selection of a travel destination.
 * When a location is clicked, it triggers the transition:
 * 1. Stops ambient audio.
 * 2. Loads the new map geometry and scripts.
 * 3. Positions the player at the designated startup point.
 */
void CUAKMTravelModule::TravelTo(int subLocationIndex)
{
	if (subLocationIndex >= 0 && subLocationIndex < _availableLocations.size())
	{
		CSubLocation* loc = _availableLocations[subLocationIndex];
		
		// Map ID and internal entrance index (startup position)
		int mapId = loc->MapID;
		int entrance = loc->EntranceIndex;

		// Cleanup current module and change the world state
		CModuleController::Pop(); 
		CAmbientAudio::Stop();

		// Trigger the level load via the Game Controller
		CGameController::GoToLocation(mapId, entrance);
	}
}

/**
 * Renders the Travel Menu interface.
 * Draws the background image, the text list of locations, 
 * and the selection cursor indicator.
 */
void CUAKMTravelModule::Render()
{
	// 1. Draw the main UI background (The Map/PDA screen)
	if (_backgroundGraphic) _backgroundGraphic->Render();

	int x = _listStartX;
	int y = _listStartY;
	int subCount = 0;

	// 2. Iterate through available locations to draw the menu items
	for (int subix = 0; subix < _availableLocations.size(); subix++)
	{
		CSubLocation* it = _availableLocations[subix];

		// Update bounding box for mouse collision detection
		it->Left = x;
		it->Top = y;
		
		// If this is the currently hovered item, draw it with the selection highlight
		bool isSelected = (subix == _selectedSubLocation);
		
		// Render the text using the game's font system
		DrawText(it->Name, x, y, isSelected ? COLOR_YELLOW : COLOR_WHITE);

		y += _lineHeight;

		// Handle multi-column layout if the list is too long
		if (++subCount == 8) 
		{
			y = _listStartY;
			x += _columnWidth;
		}

		// 3. Draw the "Selection Indicator" (the red/green bracket)
		if (isSelected)
		{
			RenderSelectionBracket(it->Left, it->Top);
		}
	}
}

/**
 * Logic for mouse movement over the travel menu.
 * Highlights the location under the cursor.
 */
void CUAKMTravelModule::OnMouseMove(Point pt)
{
	_selectedSubLocation = -1;
	for (int i = 0; i < _availableLocations.size(); i++)
	{
		CSubLocation* loc = _availableLocations[i];
		// Standard AABB (Axis-Aligned Bounding Box) collision check
		if (pt.X >= loc->Left && pt.X <= loc->Right &&
			pt.Y >= loc->Top && pt.Y <= loc->Bottom)
		{
			_selectedSubLocation = i;
			break;
		}
	}
}