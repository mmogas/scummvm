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

#include "UAKMEncodedMessageModule.h"
#include "Globals.h"
#include "GameController.h"
#include "Utilities.h"
#include "UAKMGame.h"
#include "resource.h"

/**
 * The encrypted source message (Ciphertext)
 */
char* pMsg = "YV UZNV SIAKWBHVG RIPB ZEEIWALHVAL  YWLU SUZXLWLR ZL  LUV XPWLV WA LUV  CIOGVA CZLV UILVO ZL LUV PXPZO LWHV.LUV EZXXYIBG LIGZRWX XWOWSIA.";

/**
 * The target decoded message (Plaintext) 
 * Used to validate the player's progress.
 */
char* pDec = "WE HAVE CONFIRMED YOUR APPOINTMENT  WITH CHASTITY AT  THE SUITE IN THE  GOLDEN GATE HOTEL AT THE USUAL TIME.THE PASSWORD TODAYIS SILICON.";

/**
 * Vertical pixel offsets for each of the 8 lines of text on the note background.
 */
int LineOffsets[] = { 35, 70, 104, 139, 173, 209, 243, 278 };

CUAKMEncodedMessageModule* CUAKMEncodedMessageModule::pUAKMEMM = NULL;

/**
 * Initializes the decoding puzzle module.
 * Loads the current progress from the game's global save data (A-array).
 */
CUAKMEncodedMessageModule::CUAKMEncodedMessageModule() : CModuleBase(ModuleType::EncodedMessage)
{
	pUAKMEMM = this;
    // ... clipping and UI setup ...

	// Load the player's current message buffer from save data
	_pSaveMsg = (char*)(pGame->GetBuffer() + UAKM_SAVE_ENCODED_MESSAGE);
	
	// If the buffer is empty (new game), initialize it with the encrypted message
	if (_pSaveMsg[0] == 0)
	{
		strcpy(_pSaveMsg, pMsg);
	}
}

/**
 * Handles keyboard input.
 * When a letter is pressed, it replaces the selected character in the message.
 */
void CUAKMEncodedMessageModule::KeyDown(int key)
{
	if (_charPos >= 0 && key >= 'A' && key <= 'Z')
	{
		_pSaveMsg[_charPos] = (char)key;
		
		// If the player successfully decodes the entire message
		if (CheckCompleted())
		{
			CGameController::AddScore(UAKM_SCORE_DECODE_MESSAGE);
			// Trigger script progression (Index 1 usually means "Success")
			CGameController::ResumeScript(1);
		}
	}
}

/**
 * Checks if the current state of the player's buffer matches the target plaintext.
 */
bool CUAKMEncodedMessageModule::CheckCompleted()
{
	std::size_t len = strlen(pDec);
	for (std::size_t i = 0; i < len; i++)
	{
		// Comparison is case-sensitive and must be an exact match for all chars
		if (_pSaveMsg[i] != pDec[i])
		{
			return false;
		}
	}
	return true;
}

/**
 * Main rendering loop.
 * Draws the background note and overlays the text lines.
 */
void CUAKMEncodedMessageModule::Render()
{
	// Render the background image (the piece of paper)
	if (_background) _background->Render();

	// Render the text lines with specific character spacing and offsets
	// 'pMsg' is rendered as the static reference, '_pSaveMsg' is the interactive layer
	RenderText(pMsg, 0, FALSE);      // The original hint
	RenderText(_pSaveMsg, 2, TRUE);  // The player's current attempt

	// Draw the selection indicator (the underline/box) if a character is hovered
	if (_indicatorX >= 0)
	{
		DrawIndicator(_indicatorX, _indicatorY);
	}
}

/**
 * Mouse logic to determine which character the player is clicking on.
 * Maps screen coordinates (X, Y) to a 1D index in the message string.
 */
void CUAKMEncodedMessageModule::Cursor(float x, float y, BOOL relative)
{
	CModuleBase::Cursor(x, y, relative);

	_indicatorX = -1.0f;
	_charPos = -1;

	// Scale cursor to internal resolution (typically 640x480)
	int cy = (int)((_cursorPosY - _top) / _scale);
	int cx = (int)((_cursorPosX - _left) / _scale);

	for (int sy = 0; sy < 8; sy++)
	{
		// Check if mouse is within the vertical bounds of a text line
		if (cy >= LineOffsets[sy] && cy < LineOffsets[sy] + 11)
		{
			// Check horizontal bounds (assuming fixed-width font of 13 pixels)
			if (cx >= 41 && cx < 41 + (18 * 13))
			{
				int linePos = (cx - 41) / 13;
				_charPos = sy * 18 + linePos;
				
				// Set indicator position for rendering
				_indicatorX = (float)(41 + linePos * 13);
				_indicatorY = (float)LineOffsets[sy];
			}
		}
	}
}

/**
 * Low-level character renderer.
 * Copies pixel data from a font resource into the screen buffer.
 */
void CUAKMEncodedMessageModule::RenderChar(int x, int y, char c, bool transparent)
{
	if (c < ' ' || c > 'Z') return;

	// Font mapping: Space is index 0, 'A' is 1, etc.
	int index = (c == ' ') ? 0 : (c == '.') ? 27 : (c - 'A' + 1);
	LPBYTE pCharData = _fontData[index];

	// Standard 2D blitting logic with optional transparency support
	for (int i = 0; i < 11; i++) // Height
	{
		for (int j = 0; j < 11; j++) // Width
		{
			BYTE pix = pCharData[i * 11 + j];
			if (pix != 0 || !transparent)
			{
				_screen[(y + i) * 640 + (x + j)] = pix;
			}
		}
	}
}