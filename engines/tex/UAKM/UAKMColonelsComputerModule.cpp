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

#include "UAKMColonelsComputerModule.h"
#include "Utilities.h"
#include "GameController.h"

/**
 * CUAKMColonelsComputerModule Class
 * This module manages the interactive "Colonel's Computer" puzzle.
 * It handles raw image decompression, software-based software rendering 
 * to a backbuffer, and custom input handling for a 90s-style terminal.
 */

// Hardcoded coordinates for UI elements (Buttons, text areas, etc.)
short _colonelsComputerRectCoords[] = { 
    305, 235, 314, 240,
    264, 209, 365, 272,
    103, 78, 530, 375 
};

/**
 * Init: Loads the puzzle resources.
 * It opens the 'COLCOMP.AP' archive, which contains the graphics and
 * data specific to this computer interface.
 */
void CUAKMColonelsComputerModule::Init()
{
	BinaryData bd = CGameController::LoadResource(L"COLCOMP.AP", 1);
	_data = bd.Data;
	_dataLength = bd.Length;

    // Decompress and store different interface states (buttons, screens)
	for (int i = 0; i < 18; i++)
	{
		int start = GetInt(_data, 2 + i * 4, 4);
		int end = GetInt(_data, 6 + i * 4, 4);
		_files[i] = _data + start;
	}

    // Initialize the software screen buffer (640x480)
	_screen = new byte[640 * 480];
	memset(_screen, 0, 640 * 480);
	
	_inputEnabled = TRUE;
}

/**
 * RenderImage: A custom software renderer.
 * It handles a specialized RLE (Run-Length Encoding) format where 
 * each row defines its starting offset (c1) and length (c2).
 */
void CUAKMColonelsComputerModule::RenderImage(int entry, int offset_x, int offset_y, int x1, int x2, int y1, int y2)
{
	LPBYTE pImg = _files[entry];
	int inPtr = 16; // Skip header
	int imageHeight = GetInt(pImg, 4, 2);

	for (int y = 0; y < imageHeight; y++)
	{
		int c1 = GetInt(pImg, inPtr, 2); // X-offset for this row
		int c2 = GetInt(pImg, inPtr + 2, 2); // Pixel count for this row
		int ry = offset_y + y;

        // Clipping: only draw if the row is within the vertical bounds
		if (ry >= y1 && ry <= y2)
		{
			for (int x = 0; x < c2; x++)
			{
				int rx = offset_x + c1 + x;
                // Clipping: only draw if the pixel is within horizontal bounds
				if (rx >= x1 && rx <= x2)
				{
					_screen[ry * 640 + rx] = pImg[inPtr + 4 + x];
				}
			}
		}
		inPtr += 4 + c2; // Advance to next row data
	}
}

/**
 * RenderRaw: Decompresses and draws a full block image.
 * Uses CLZ (a Lempel-Ziv variant) for data decompression.
 */
void CUAKMColonelsComputerModule::RenderRaw(int entry, int offset_x, int offset_y)
{
	int w = 455, h = 322;
	LPBYTE pImg = _files[entry];
	int l = GetInt(_data, 6 + entry * 4, 4) - GetInt(_data, 2 + entry * 4, 4);
	
    // LZ Decompression happens here
	BinaryData bd = CLZ::Decompress(pImg, l);
	LPBYTE pRaw = bd.Data;

    // Copy decompressed pixels to our software backbuffer
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			_screen[(offset_y + y) * 640 + offset_x + x] = pRaw[y * w + x];
		}
	}

	delete pRaw;
}

/**
 * BeginAction: Processes mouse clicks on the virtual computer.
 */
void CUAKMColonelsComputerModule::BeginAction()
{
	if (_inputEnabled)
	{
		// Logic to detect which "virtual button" on the computer screen 
        // was pressed and trigger the corresponding animation or puzzle state.
        // Example: Checking against _colonelsComputerRectCoords...
	}
}