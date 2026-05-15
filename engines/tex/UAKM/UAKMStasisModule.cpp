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

#include "UAKMStasisModule.h"
#include "ModuleController.h"
#include "GameController.h"
#include "AnimationController.h"
#include "MainMenuModule.h"
#include "Utilities.h"
#include "UAKMGame.h"
#include "UAKMHintModule.h"

// Horizontal offsets for the 12 progress indicator lights
int ProgressLightsXOffsets[] = { 108, 130, 152, 174, 241, 263, 285, 307, 377, 399, 421, 443 };

// Vertical positions for the temperature and oxygen needles based on current puzzle stage
int TemperatureStagePositions[] = { 252, 238, 238, 238, 238, 172, 172, 172, 172, 144, 144, 144, 144 };
int OxygenStagePositions[] = { 252, 252, 218, 218, 218, 218, 218, 218, 178, 178, 144, 144, 144 };

/**
 * Maps screen coordinates to specific control buttons on the Stasis device.
 * Format: { ID, X, Y }
 */
ControlTable StasisControls[] = { 
    {3,309,238}, { 5,113,294 }, { 7,232,294 }, { 9,354,294 }, 
    { 11,93,353 }, { 13,226,353 }, { 15,361,353 } 
};

/**
 * Initializes the Stasis puzzle module.
 * Loads the background animation, sets initial levels, and clears indicators.
 */
void CUAKMStasisModule::Init()
{
	_anim = _animBase = CAnimationController::LoadAnimation(15, 11); // Main console background
	_animPlayer.Init(_anim);
	_anim2 = CAnimationController::LoadAnimation(15, 12); // Secondary state animation (failure/success)

	_stasisSettings = 0;
	_progress = 0;
	_badEventCount = 0;
	_temperatureStage = 0;
	_oxygenStage = 0;
	_inputEnabled = TRUE;
	_badPlayed = FALSE;

	_caption.SetRect({ 110, 420, 530, 470 });
	_caption.SetText(L"");
}

/**
 * Handles the logic when the player makes a mistake.
 * Updates internal warning flags and triggers failure animations or death.
 */
void CUAKMStasisModule::HandleBadEvent(LPBYTE pAnim)
{
	if (_badEventCount == 0)
	{
		if (!_badPlayed)
		{
			// First failure warning
			if (_badAnimCounter > 0)
			{
				_caption.SetText(GeneralWarning);
				_nextAudio = 2; // Play sound index for warning
			}
			else
			{
				// Specific button error
				_caption.SetText(ButtonWarning);
				_nextAudio = 3;
			}

			CAnimationController::UpdateAndRender();
			_badPlayed = TRUE;
			MouseUp({ 0,0 }, -1);
		}
	}
	else
	{
		// Two consecutive bad events usually lead to a Game Over
		if (_anim != NULL)
		{
			_animPlayer.Merge(_anim2); // Merge with death/failure animation
			_anim = _anim2;
		}
		_inputEnabled = FALSE;

		// Set bitmask for critical system failure visuals
		_stasisSettings |= (STASIS_SETTINGS_WARNING_ON | STASIS_SETTINGS_STATE_DYING_1 | STASIS_SETTINGS_FAILED_ON | STASIS_SETTINGS_STATE_DYING_2);
		_nextWarningChange = GetTickCount64() + 500;
	}

	_badEventCount++;

	if (pAnim != NULL)
	{
		_anim = pAnim;
		_badAnimCounter = 0;
		_animPlayer.Init(pAnim);
		_nextAnimFrameTime = GetTickCount64() + 100;
	}
}

/**
 * Checks if the mouse click hits a valid button.
 * Validates the puzzle logic: buttons must be pressed in a specific sequence
 * to advance the progress lights and stabilize the oxygen/temperature needles.
 */
void CUAKMStasisModule::CheckMouseAction(Point pt)
{
	if (!_inputEnabled) return;

	for (int i = 0; i < 7; i++)
	{
		// Check if click is within button bounds (using a standard 40x40 pixel hit-box)
		if (pt.X >= StasisControls[i].X && pt.X <= StasisControls[i].X + 40 &&
			pt.Y >= StasisControls[i].Y && pt.Y <= StasisControls[i].Y + 40)
		{
			// Puzzle Logic Implementation:
			// Advancing the progress requires specific IDs at specific stages.
			if (StasisControls[i].ID == ExpectedButtonPerStage[_progress])
			{
				_progress++;
				_badEventCount = 0; // Reset error streak on success
				_badPlayed = FALSE;
				
				// Update gauge levels
				_temperatureStage = min(_temperatureStage + 1, 12);
				_oxygenStage = min(_oxygenStage + 1, 12);

				if (_progress >= 12)
				{
					// Puzzle Solved: Trigger victory sequence
					_caption.SetText(SuccessMessage);
					_inputEnabled = FALSE;
				}
			}
			else
			{
				HandleBadEvent(NULL);
			}
			break;
		}
	}
}

/**
 * Main rendering loop for the Stasis console.
 * Draws the background animation and overlays the dynamic UI elements (needles, lights).
 */
void CUAKMStasisModule::Render()
{
	_animPlayer.RenderNextFrame();

	// Draw Oxygen Needle
	int ox = 527; // Base X position for Oxygen gauge
	int oy = OxygenStagePositions[_oxygenStage];
	DrawNeedle(ox, oy);

	// Draw Temperature Needle
	int tx = 63; // Base X position for Temperature gauge
	int ty = TemperatureStagePositions[_temperatureStage];
	DrawNeedle(tx, ty);

	// Render the 12 progress lights based on current '_progress' value
	for (int i = 0; i < _progress; i++)
	{
		DrawActiveLight(ProgressLightsXOffsets[i], 244);
	}

	// Render text feedback (Warnings or Instructions)
	_caption.Render();
}