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

#include "UAKMSafeModule.h"
#include "Globals.h"
#include "GameController.h"
#include "Utilities.h"
#include "UAKMGame.h"
#include "AmbientAudio.h"

/**
 * Screen locations for the numeric keypad buttons (X, Y).
 */
int keyLocations[] = { 493, 280, 461, 200, 493, 200, 524, 200, 461, 226, 493, 226, 524, 226, 461, 253, 493, 253, 524, 253, 461, 280, 524, 280, 493, 307, 493, 334 };

/**
 * Hardcoded correct codes for different safes in the game.
 * Eddie Ching's Safe: 1-0-1-4-1-2
 * GRS Safe: 1-4-2-2-3-x...
 */
signed char eddieChingsSafeCode[] = { 1, 0, 1, 4, 1, 2, -1, -1 };
signed char grsSafeCode[] = { 1, 4, 2, 2, 3, 5, 1, 1 };

/**
 * Constructor: Initializes the safe based on which location/parameter is active.
 * parameter 0: Eddie Ching's Safe
 * parameter 1: G.R.S. Safe
 */
CUAKMSafeModule::CUAKMSafeModule(int parameter) : CFullScreenModule(ModuleType::Safe)
{
	_parameter = parameter;
	_keyPos = 0; // Current digit being entered
	_codeCorrect = FALSE;
	_ready = TRUE;
	_openSafeSequence = -1; // -1 means the safe is currently closed
	// ... buffer and screen initialization ...
}

/**
 * Handles visual and audio feedback when a key is pressed.
 */
void CUAKMSafeModule::Press(int key, int sound)
{
	_keyDown[key] = 1;

	// Render the "pressed" state of the button (using a specific frame offset)
	PartialRender(19 + key * 2 + 1, keyLocations[key * 2 + 0], keyLocations[key * 2 + 1] - 30, TRUE);

	// Set a small delay before the button pops back up
	_frameDelay = (DWORD)(12 * TIMER_SCALE);
	_frameTimes[key] = static_cast<DWORD>(GetTickCount64());

	// Play the mechanical click sound
	_sound.Play(_safeSoundOffsets[sound % 12]);
}

/**
 * Triggered when the 'Enter' or 'Check' button is pressed.
 * Validates the entered digits against the target code.
 */
void CUAKMSafeModule::Check()
{
	// Determine which code to use based on the current safe
	signed char* pCorrectCode = (_parameter == 0 ? eddieChingsSafeCode : grsSafeCode);
	
	_codeCorrect = TRUE;
	for (int i = 0; i < 8; i++)
	{
		// -1 marks the end of a code shorter than 8 digits
		if (pCorrectCode[i] == -1) break;

		if (pCorrectCode[i] != _enteredCode[i])
		{
			_codeCorrect = FALSE;
			break;
		}
	}

	// Trigger the opening sequence
	_openSafeSequence = 0;
	_frameDelay = (DWORD)(60 * TIMER_SCALE);
	_frameTime = GetTickCount64();
	_ready = FALSE; // Disable input while the safe is animating
}

/**
 * Updates the animation state of the safe handle and door.
 */
void CUAKMSafeModule::Tick(int ticks)
{
	if (_openSafeSequence >= 0)
	{
		auto now = GetTickCount64();
		if (now - _frameTime >= _frameDelay)
		{
			_frameTime = now;
			_openSafeSequence++;

			// If the code was correct, play the "Success" animation sequence
			if (_codeCorrect)
			{
				// (Logic for advancing frames of the door opening)
				// Once animation finishes, the script is resumed to allow item pickup
				if (_openSafeSequence > MAX_OPEN_FRAMES) 
				{
					CModuleController::Pop();
					CGameController::ResumeScript(1); // 1 = Success
				}
			}
			else
			{
				// If wrong, play a "rejection" sound/jiggle and reset
				if (_openSafeSequence > MAX_ERROR_FRAMES)
				{
					_openSafeSequence = -1;
					_ready = TRUE;
					_keyPos = 0; // Reset input buffer
				}
			}
		}
	}
}

/**
 * Mouse logic for detecting which button on the numeric pad is clicked.
 */
void CUAKMSafeModule::BeginAction()
{
	if (!_ready) return;

	int x = static_cast<int>((_cursorPosX - _left) / _scale);
	int y = static_cast<int>((_cursorPosY - _top) / _scale);

	// Hit-test against the 10 numeric buttons
	for (int i = 0; i < 10; i++)
	{
		if (x >= keyLocations[i*2] && x < keyLocations[i*2] + 20 &&
		    y >= keyLocations[i*2+1] && y < keyLocations[i*2+1] + 20)
		{
			Number(i); // Add digit to buffer
			Press(i, i); // Play animation/sound
			return;
		}
	}

	// Check for 'Cancel' (button 11) or 'Enter' (button 12)
	// ... (similar hit-test logic) ...
}