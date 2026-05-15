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

#include "UAKMNewsPaperModule.h"
#include "Utilities.h"
#include "GameController.h"
#include "UAKMGame.h"

// Screen states for the newspaper interface
#define NEWSPAPER_PALETTE		0
#define NEWSPAPER_PAGE			1
#define NEWSPAPER_ARTICLE_1		2 // Individual zoomed-in articles start here

CUAKMNewsPaperModule* CUAKMNewsPaperModule::pUAKMNPM = NULL;

/**
 * Initializes the Newspaper module.
 * Sets up coordinates and the internal state for handling article highlights.
 */
CUAKMNewsPaperModule::CUAKMNewsPaperModule() : CModuleBase(ModuleType::NewsPaper)
{
	pUAKMNPM = this;
	_display = 0;
	_highLight = 0; // Stores the color index of the currently hovered article
	
    // ... basic initialization ...
	ZeroMemory(_palette, 256 * sizeof(int));
}

/**
 * Handles mouse movement and article highlighting.
 * Under a Killing Moon uses a clever trick: specific pixel values in the 
 * newspaper image correspond to specific articles (e.g., 0x80, 0x81).
 */
void CUAKMNewsPaperModule::Cursor(float x, float y, BOOL relative)
{
	CModuleBase::Cursor(x, y, relative);

	if (_display == NEWSPAPER_PAGE)
	{
		CNewspaperData* np = _files[NEWSPAPER_PAGE];
		if (np != NULL)
		{
			// Translate screen mouse coordinates to image pixel coordinates
			int sx = (int)((_cursorPosX - _left) / _scale);
			int sy = (int)((_cursorPosY - _top) / _scale);

			if (sx >= 0 && sx < np->Width && sy >= 0 && sy < np->Height)
			{
				// Scan pixels to find the "Hot Zone" ID
				// The game uses high-index palette colors (>= 0x80) to define clickable areas
				while (sy >= 0)
				{
					BYTE pixel = np->Data[sy * np->Width + sx];
					if (pixel >= 0x7f) // Found a control pixel or interactive zone
					{
						if (pixel != _highLight)
						{
							// Restore old highlight to white
							if (_highLight >= 0x80)
							{
								_palette[_highLight] = 0xffffffff; 
							}

							// Apply yellow highlight to the new active article
							if (pixel >= 0x80)
							{
								_palette[pixel] = 0xffffff00; // Yellow (BGR Format)
							}

							_highLight = pixel;
							UpdateTexture(np); // Refresh the GPU texture with the new palette
						}
						break;
					}
					sy--; // Scan upwards to find the base index if needed
				}
			}
		}
	}
}

/**
 * Handles the logic when a player clicks on an article.
 * Triggers game events, such as enabling new "Ask About" topics for NPCs.
 */
void CUAKMNewsPaperModule::BeginAction()
{
	if (_pBtnResume->HitTest(_cursorPosX, _cursorPosY))
	{
		_pBtnResume->Click();
	}
	else
	{
		// If an article is highlighted (index >= 0x80), switch to full-screen view
		if (_display == NEWSPAPER_PAGE && _highLight >= 0x80)
		{
			// Map the highlight ID to the specific article resource index
			_display = NEWSPAPER_ARTICLE_1 + _highLight - 0x80;

			// Logic for unlocking clues based on which article was read
			if (_highLight == 0x80)
			{
				// Example: Reading the first article enables talking about Mac Malden
				CGameController::SetAskAboutState(27, 1); 
				CGameController::SetAskAboutState(31, 1);
			}
			// ... other article logic ...
		}
		else if (_display >= NEWSPAPER_ARTICLE_1)
		{
			// If already viewing an article, clicking returns to the main page
			_display = NEWSPAPER_PAGE;
		}
	}
}

/**
 * Renders the current view (Main page or Zoomed article).
 * Updates the Direct3D texture using the current palette state.
 */
void CUAKMNewsPaperModule::Render()
{
	// Choose which image data to display
	CNewspaperData* np = (_display == 0) ? _files[NEWSPAPER_PAGE] : _files[_display];
	
	if (np != NULL)
	{
		// Render the newspaper using the custom software-to-D3D pipeline
		UpdateTexture(np);
		_texture.Render(_left, _top, _scale);
	}

	// Always render the 'Resume/Exit' button on top
	if (_pBtnResume) _pBtnResume->Render();
}