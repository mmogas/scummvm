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

#include "UAKMCodePanelModule.h"
#include "Utilities.h"
#include "LZ.h"
#include "GameBase.h"
#include "GameController.h"

// Define resource indices for the internal file array
#define IMG_HAND		0   // Background or hand graphic
#define IMG_ENTER_PW	1   // "Enter Password" prompt
#define IMG_PATRONAGE	2   // Success message
#define IMG_INCORRECT	3   // Failure message
#define IMG_DOT			4   // Indicator dots for digits entered
#define DAT_COORDS1		5   // Coordinate data table 1
#define DAT_COORDS2		6   // Coordinate data table 2 (Button hitboxes)
#define SND_1			7   // Key-press sound effects...

/**
 * The hardcoded solution sequence for the keypad.
 * Values likely correspond to button IDs defined in the coordinate data.
 */
signed char CodePanelCorrectCode[] = { 18, 8, 11, 8, 2, 14, 13, -1 };

/**
 * Renders the keypad UI and handles visual feedback for correct/incorrect attempts.
 */
void CUAKMCodePanelModule::Render()
{
	// Handle the transition after a successful code entry
	if (_correctFrame > 0)
	{
		auto diff = GetTickCount64() - _correctFrameTime;
		if (_correctFrame == 2 && diff > 500)
		{
			// Display "PATRONAGE" success screen
			Render(IMG_PATRONAGE, 206, 175);
			if (diff > 2500)
			{
				// Exit module and return to game after delay
				CModuleController::Pop();
				CGameController::ResumeScript(1);
			}
		}
		// ... (logic for frame 1)
	}

	// Draw input progress dots
	for (int i = 0; i < _inputLength; i++)
	{
		// Render a dot for each character entered so far
		Render(IMG_DOT, 237 + i * 22, 117);
	}

	// Handle the blinking "INCORRECT" message on failed attempts
	if (_wrongFrame > 0)
	{
		auto diff = GetTickCount64() - _wrongFrameTime;
		if (diff < 2000)
		{
			// Blink the message every 250ms
			if ((diff / 250) % 2 == 0)
			{
				Render(IMG_INCORRECT, 206, 175);
			}
		}
		else
		{
			_wrongFrame = 0; // Reset error state
		}
	}
}

/**
 * Custom software renderer for the keypad assets.
 * Extracts width, height, and RLE-compressed pixel data from the custom resource format.
 */
void CUAKMCodePanelModule::Render(int entry, int offset_x, int offset_y)
{
	LPBYTE pImg = _files[entry];

	int w = GetInt(pImg, 2, 2);
	int h = GetInt(pImg, 4, 2);
	int inPtr = 16; // Skip resource header

	for (int y = 0; y < h; y++)
	{
		// Each row starts with a skip count (c1) and a pixel count (c2)
		int c1 = GetInt(pImg, inPtr, 2);
		int c2 = GetInt(pImg, inPtr + 2, 2);

		for (int x = 0; x < w; x++)
		{
			// Basic transparency: only draw pixels within the 'c2' range
			int pix = (x >= c1 && x < (c1 + c2)) ? pImg[inPtr + 4 + x - c1] : 0;
			if (pix != 0) 
			{
				_screen[(y + offset_y) * 640 + offset_x + x] = pix;
			}
		}
		inPtr += 4 + c2;
	}
}

/**
 * Handles mouse clicks on the keypad.
 * Uses a coordinate scan table (DAT_COORDS2) to identify which button was hit.
 */
void CUAKMCodePanelModule::BeginAction()
{
	if (!_inputEnabled) return;

	LPBYTE pButtonCoordinates = _files[DAT_COORDS2];

	// Adjust cursor coordinates to the scaled/centered screen
	int x = static_cast<int>((_cursorPosX - _left) / _scale);
	int y = static_cast<int>((_cursorPosY - _top) / _scale);

	LPBYTE scan = pButtonCoordinates;
	int i = 0;
	int hit = -1;

	// The coordinate file contains a list of bounding boxes: [ID, X1, Y1, X2, Y2]
	// Terminated by 0xFF
	while (*scan != 0xff)
	{
		int bid = *(scan++);
		int x1 = GetInt(scan, 0, 2); scan += 2;
		int y1 = GetInt(scan, 0, 2); scan += 2;
		int x2 = GetInt(scan, 0, 2); scan += 2;
		int y2 = GetInt(scan, 0, 2); scan += 2;

		if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
		{
			hit = bid; // Found the button ID
			break;
		}
	}

	if (hit != -1)
	{
		// Process the button press
		if (hit == 20) // ID 20 usually represents 'Enter' or 'Clear'
		{
			_inputLength = 0;
		}
		else
		{
			// Store the input and check against the solution
			_inputBuffer[_inputLength++] = (signed char)hit;
			
			// If input length matches the solution, verify the sequence
			if (CodePanelCorrectCode[_inputLength] == -1)
			{
				bool match = true;
				for(int j=0; j<_inputLength; j++) {
					if(_inputBuffer[j] != CodePanelCorrectCode[j]) match = false;
				}
				
				if (match) {
					_correctFrame = 1;
					_correctFrameTime = GetTickCount64();
					_inputEnabled = false;
				} else {
					_wrongFrame = 1;
					_wrongFrameTime = GetTickCount64();
					_inputLength = 0;
				}
			}
		}
	}
}