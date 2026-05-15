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

#include "UAKMColonelsSafeModule.h"
#include "GameController.h"
#include "UAKMGame.h"

// Indices for the different graphical and sound resources in the .AP file
#define IMG_DIAL_1	0
#define IMG_DIAL_2	1
#define IMG_DIAL_3	2
#define IMG_GREEN	3
#define IMG_HAND	4
#define SND_1		5

// The solution to the puzzle and screen positions for the three dials
BYTE ColonelsSafeCorrectCode[] = { 5, 7, 1 };
int ColonelsSafeDialOffsets[] = { 169, 285, 396 };

/**
 * CUAKMColonelsSafeModule
 * Manages the logic for the safe cracking puzzle.
 * It tracks the state of the three dials and checks if the entered 
 * combination matches the solution.
 */
CUAKMColonelsSafeModule::CUAKMColonelsSafeModule(int parameter) : CFullScreenModule(ModuleType::AAASafe)
{
	_parameter = parameter;
	_frameTime = 0;

	ResetCode();
}

/**
 * Render: Main drawing loop for the safe interface.
 * It handles the animation of the dials turning by switching 
 * frames based on a timer (GetTickCount).
 */
void CUAKMColonelsSafeModule::Render()
{
	// Animation logic: if a dial is turning, update the frame every 50ms
	if (_currentFrame > 0 && (GetTickCount() - _frameTime) > 50)
	{
		int x = ColonelsSafeDialOffsets[_dial];
		int y = 273;

		// Draw the current frame of the dial rotation
		Render(3 - _currentFrame, x, y);
		_frameTime = GetTickCount64();
		_currentFrame--;

		// Once the animation ends, play a mechanical "click" sound
		if (_currentFrame == 0)
		{
			_sound.Play(_files[SND_1]);

			// Check if the combination is now correct
			BOOL correct = TRUE;
			for (int i = 0; i < 3; i++)
			{
				if (ColonelsSafeCorrectCode[i] != _enteredCode[i]) 
                    correct = FALSE;
			}
			
			// If correct, trigger the "Safe Opened" state
			if (correct)
			{
				// Logic to open safe...
			}
		}
	}
}

/**
 * Render (Internal): Draws an image entry to the software screen buffer.
 * It uses the same "Span-based" RLE decompression seen in other modules,
 * where c1 is the skip-offset and c2 is the number of pixels to draw.
 */
void CUAKMColonelsSafeModule::Render(int entry, int offset_x, int offset_y)
{
	LPBYTE pImg = _files[entry];

	int w = GetInt(pImg, 2, 2); // Image Width
	int h = GetInt(pImg, 4, 2); // Image Height
	int inPtr = 16;             // Data start after header

	for (int y = 0; y < h; y++)
	{
		int c1 = GetInt(pImg, inPtr, 2); // X skip
		int c2 = GetInt(pImg, inPtr + 2, 2); // Pixel count

		for (int x = 0; x < w; x++)
		{
			// Only draw pixels defined in the current span (c1 to c1+c2)
			int pix = (x >= c1 && x < (c1 + c2)) ? pImg[inPtr + 4 + x - c1] : 0;
			if (pix != 0) // Pixel 0 is treated as transparent
			{
				_screen[(y + offset_y) * 640 + offset_x + x] = pix;
			}
		}

		inPtr += 4 + c2; // Move to the data for the next row
	}
}

/**
 * TurnDial: Increases the value of one of the safe's dials.
 * It sets the animation state so the Render function can play the visual feedback.
 */
void CUAKMColonelsSafeModule::TurnDial(int dial)
{
	if (dial >= 0 && dial < 3)
	{
		_dial = dial;
		_inputEnabled = FALSE; // Block input during animation
		_currentFrame = 3;     // Set animation length (3 frames)
		
		// Increment dial value (wrapping around 0-9)
		_enteredCode[dial] = (_enteredCode[dial] + 1) % 10;
	}
}