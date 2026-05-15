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

#include "UAKMPusShellGameModule.h"
#include "ModuleController.h"
#include "GameController.h"
#include "AnimationController.h"
#include "MainMenuModule.h"

/**
 * Clickable zones for the shell game.
 * Format: {x1, x2, y1, y2} for each of the four possible selection areas.
 */
int Coordinates[] = { 106,129,128,153, 57,78,262,284, 106,130,237,261, 201,230,234,263 };

/**
 * Constructor for the Pus Shell Game (Ferrelette).
 * Calculates scaling and centering offsets to ensure the minijuego 
 * maintains its aspect ratio on modern high-resolution displays.
 */
CUAKMPusShellGameModule::CUAKMPusShellGameModule(int parameter, int correctAnswer) : CModuleBase(ModuleType::Ferrelette)
{
	_parameter = parameter;
	_correctAnswer = correctAnswer;

	float w = (float)dx.GetWidth();
	float h = (float)dx.GetHeight();
	
	// Determine the scale factor based on original animation dimensions
	float sx = w / (float)CAnimationController::Width();
	float sy = h / (float)CAnimationController::Height();
	_scale = min(sx, sy);

	// Calculate black-bar offsets for letterboxing/pillarboxing
	float sw = (float)CAnimationController::Width() * _scale;
	float sh = (float)CAnimationController::Height() * _scale;

	_top = -(h - sh) / 2.0f;
	_left = (w - sw) / 2.0f;

	// Constrain cursor movement to the active game area
	_cursorMinX = static_cast<int>(_left);
	_cursorMaxX = static_cast<int>(CAnimationController::Width() * _scale);
	_cursorMinY = static_cast<int>(_top);
	_cursorMaxY = static_cast<int>(CAnimationController::Height() * _scale);
}

/**
 * Renders the minigame frame.
 * Disables the Z-Buffer to draw the 2D software-rendered animation 
 * and the hardware-accelerated cursor on top.
 */
void CUAKMPusShellGameModule::Render()
{
	dx.DisableZBuffer();

	// Handled by the global animation controller (plays the FMV/sprites)
	CAnimationController::UpdateAndRender();

	// Update and draw the game-specific cursor
	CModuleController::Cursors[0].SetPosition(_cursorPosX, _cursorPosY);
	CModuleController::Cursors[0].Render();

	dx.EnableZBuffer();
}

/**
 * Process the player's attempt to pick a shell.
 * Translates mouse screen coordinates back into the original 
 * low-res coordinate space to check against the 'Coordinates' table.
 */
void CUAKMPusShellGameModule::BeginAction()
{
	// Inverse scaling: Screen pixels -> Original local coordinates
	int x = 8 + static_cast<int>((_cursorPosX - _left) / _scale);
	int y = 6 + static_cast<int>((_cursorPosY - _top) / _scale);

	// Loop through the 4 possible interactive zones (cubes/shells)
	for (int i = 0; i < 4; i++)
	{
		int offset = i * 4;
		// Boundary check for the shell click area
		if (x >= Coordinates[offset] && x <= Coordinates[offset + 1] && 
		    y >= Coordinates[offset + 2] && y <= Coordinates[offset + 3])
		{
			// Result is 1 if correct, 0 if wrong.
			// This result is passed back to the script engine to determine the outcome.
			int result = (i == _correctAnswer) ? 1 : 0;
			
			// Close the module and resume game script with the result
			CModuleController::Pop(result);
			break;
		}
	}
}

/**
 * Initializes the cursor to the center of the screen at the start of the game.
 */
void CUAKMPusShellGameModule::Initialize()
{
	_cursorPosX = dx.GetWidth() / 2.0f;
	_cursorPosY = dx.GetHeight() / 2.0f;
}