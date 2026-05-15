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

#include "UAKMCrimeLinkModule.h"
#include "Utilities.h"
#include "LZ.h"
#include "GameController.h"

/**
 * CUAKMCrimeLinkModule Class
 * Manages the "CrimeLink" forensic computer system.
 * This module includes logic for multi-page navigation, evidence matching,
 * and real-time palette manipulation for UI feedback.
 */

// The 'CorrectSelections' array stores the bitmask solutions for the 
// various forensic puzzles within CrimeLink.
short CUAKMCrimeLinkModule::CorrectSelections[15] = { 1,1,0x10,0x40,2,2,2,8,0x20,0x100,0,2,0,0,0 };

/**
 * Init: Sets up the CrimeLink interface.
 * Loads the 'CRIMELNK.AP' resource which contains all the terminal 
 * graphics and UI layouts.
 */
void CUAKMCrimeLinkModule::Init()
{
	BinaryData bd = CGameController::LoadResource(L"CRIMELNK.AP", 1);
	_data = bd.Data;
	
	// Decompress and prepare individual UI pages/elements
	// CrimeLink uses LZ decompression for its full-screen layouts
	for (int i = 0; i < 50; i++)
	{
		// Logic to map internal file pointers to specific UI pages...
	}

	_page = 1; // Start at the main menu/first page
	_inputEnabled = TRUE;
	UpdateTexture(); // Convert the software buffer to a GPU texture
}

/**
 * DrawRectangle: Highlights UI elements.
 * Instead of drawing a new image, it modifies the pixels in the backbuffer
 * to create a selection box around buttons or text.
 */
void CUAKMCrimeLinkModule::DrawRectangle(int index, BYTE color, bool category)
{
    // Logic to calculate X/Y coordinates based on the button index
    // and fill the area in the _screen buffer with the 'color' index.
    int x = _buttonCoords[index].x;
    int y = _buttonCoords[index].y;
    
    // Manual pixel manipulation for the UI border
    for(int i = 0; i < width; i++) {
        _screen[y * 640 + x + i] = color;
    }
}

/**
 * ReplaceColour: A legacy trick for UI animation.
 * This function scans the screen buffer and replaces one color index 
 * with another. Used for "flashing" text or highlighting entire categories.
 */
void CUAKMCrimeLinkModule::ReplaceColour(bool category, int index, BYTE oldCol, BYTE newCol)
{
	if (index < 0) return;

	// Iterates through a specific region of the software buffer
	// and performs a search-and-replace on the pixel data.
	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			if (_screen[y * 640 + x] == oldCol) 
                _screen[y * 640 + x] = newCol;
		}
	}
}

/**
 * BeginAction: Processes user interaction (Mouse Clicks).
 * Evaluates which button was pressed and whether it advances the 
 * investigation or marks a piece of evidence as a "match".
 */
void CUAKMCrimeLinkModule::BeginAction()
{
	POINT pt = GetMouseOver();
	if (pt.x >= 0)
	{
		// Toggle the selection state for the clicked item
		PlayerSelections[_page] ^= (1 << pt.x);
		
		// Play the classic terminal "beep" sound
		CGameController::PlaySound(CL_SOUND_BEEP);
		
		// Refresh the visual display
		UpdateTexture();
	}
}