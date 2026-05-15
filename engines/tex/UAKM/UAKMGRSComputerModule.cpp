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

#include "UAKMGRSComputerModule.h"
#include "Utilities.h"
#include "GameController.h"

/**
 * Array defining the sequence of data pages available in the GRS computer.
 */
int GRS_Page[] = { 2, 3, 4, 5, 7, 9 };

/**
 * Initializes the GRS Computer module.
 * Sets up initial animation states and input flags.
 */
CUAKMGRSComputerModule::CUAKMGRSComputerModule() : CFullScreenModule(ModuleType::GRSComputer)
{
	_animation = NULL;
	_animationLength = 0;

	_animationPointer = NULL;
	_animationFrames = 0;
	_animationWidth = 0;
	_animationHeight = 0;
	_animationActive = FALSE;

	_previousPage = 0;
	_inputEnabled = FALSE;
}

/**
 * Main rendering loop for the computer screen.
 * Handles the startup sequence, page transitions, and data animations.
 */
void CUAKMGRSComputerModule::Render()
{
	BOOL popOnEnd = FALSE;
	auto delta = GetTickCount64() - _frameTime;

	// Page 0: Startup / Boot sequence
	if (_currentPage == 0)
	{
		if (_currentFrame == 0 && delta >= 1000)
		{
			_currentFrame++;
			_frameTime = GetTickCount64();
		}
		// ... logic for drawing the GRS logo and boot text ...
	}
	
	// Handle Data Animation playback (when the user clicks 'Play' on a file)
	if (_animationActive && _animationPointer != NULL)
	{
		// Render the frame using the custom RLE/Software renderer
		RenderAnimationFrame(_animationPointer, _animationWidth, _animationHeight);
		
		_currentFrame++;
		if (_currentFrame >= _animationFrames)
		{
			// Loop or stop animation after the last frame
			_animationActive = FALSE;
			_inputEnabled = TRUE;
		}
	}

	// Exit logic: Page 99 signals the user wants to log out
	if (_currentPage == 99)
	{
		CModuleController::Pop();
		CGameController::ResumeScript(1);
	}
}

/**
 * Processes mouse clicks on the computer interface.
 * Uses a 'buttonTable' to detect hits on UI elements like 'Next', 'Home', or 'Exit'.
 */
void CUAKMGRSComputerModule::BeginAction()
{
	if (!_inputEnabled) return;

	// Scale screen coordinates to the internal 640x480 resolution
	int x = static_cast<int>((_cursorPosX - _left) / _scale);
	int y = static_cast<int>((_cursorPosY - _top) / _scale);

	// Load the button definition table for the current page
	LPBYTE buttonTable = GetButtonTableForPage(_currentPage);
	
	// Iterate through buttons defined in the resource data
	while (buttonTable != NULL && *buttonTable != 0xff)
	{
		// Check bounding box: [Y_min, Y_max, X_min, X_max]
		if (y >= GetInt(buttonTable, 2, 2) && y < GetInt(buttonTable, 4, 2) && 
		    x >= GetInt(buttonTable, 6, 2) && x < GetInt(buttonTable, 8, 2))
		{
			int function = buttonTable[11]; // Action ID
			
			if (function == 5) // Logout/Exit
			{
				_currentPage = 99;
				_currentFrame = 0;
			}
			else if (function == 6) // Home Page
			{
				_currentPage = 1;
				_currentFrame = 0;
			}
			else if (function == 7) // Next Page
			{
				_currentPage++;
				_currentFrame = 0;
			}
			else if (function == 9) // Play Animation
			{
				_currentPage = 100; // Special state for full-screen animation
				_currentFrame = 0;
				_inputEnabled = FALSE;
				_animationActive = TRUE;
				// Setup animation pointers from resource
				_animationPointer = _animation + 8; 
			}
			break;
		}
		buttonTable += 12; // Advance to next button entry (12 bytes per record)
	}
}

/**
 * Software renderer for GRS animations.
 * Decodes custom compressed frame data into the 8-bit backbuffer.
 */
void CUAKMGRSComputerModule::RenderAnimationFrame(LPBYTE pData, int width, int height)
{
	int outPtr = (240 - (height / 2)) * 640 + (320 - (width / 2));
	int inPtr = 0;

	for (int y = 0; y < height; y++)
	{
		// Process scanlines
		int x = 0;
		while (x < width)
		{
			BYTE cmd = pData[inPtr++];
			if (cmd > 0x80) // Run-length encoding or skip command
			{
				int count = cmd & 0x7f;
				// ... copy or skip pixels ...
				x += count;
			}
			else
			{
				// Literal pixel copy
				_screen[outPtr + x] = pData[inPtr++];
				x++;
			}
		}
		outPtr += 640; // Move to next row in the 640-wide buffer
	}
}