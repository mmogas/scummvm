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

#include "UAKMHintModule.h"
#include "GameController.h"
#include "File.h"
#include "Utilities.h"

/**
 * Initializes the Hint Module by scanning the game state for active categories.
 * Only hints marked as 'open' or 'unlocked' in the game's logic are displayed.
 */
void CUAKMHintModule::Initialize()
{
	int w = dx.GetWidth();
	int h = dx.GetHeight();

	// Start the cursor in the middle of the screen
	_cursorPosX = static_cast<float>(w) / 2.0f;
	_cursorPosY = static_cast<float>(h) / 2.0f;

	// Retrieve how many hint categories exist in the game definition
	int categoryCount = CGameController::GetHintCategoryCount();
	for (int hc = 0; hc < categoryCount; hc++)\n	{
		// Check if the specific hint category is currently active in the story
		int hcs = CGameController::GetHintCategoryState(hc);
		if (hcs == 1)
		{
			// Add active categories to the current display list
			CHintCategory* pHC = CGameController::GetHintCategory(hc);
			pHC->Reset();
			_activeHintCategories.push_back(pHC);
		}
	}

	// ... (Palette and Graphics loading logic) ...
}

/**
 * Renders the hint UI, including the text descriptions and status icons.
 * Status icons show if a hint is locked (?), available (.), or already read (check).
 */
void CUAKMHintModule::Render()
{
	// Render the background interface
	if (_background) _background->Render();

	int x = _startX;
	int y = _startY;
	bool questionMarkUsed = false;

	for (auto it : _activeHintCategories)
	{
		// Render the category title (e.g., "The Warehouse" or "The Note")
		it->RenderTitle(x, y);

		// Iterate through individual hints within this category
		for (int h = 0; h < it->GetHintCount(); h++)
		{
			y += _lineHeight;
			
			// Get the state: 0 = Locked, 1 = Read, 2 = Available to buy
			int state = CGameController::GetHintState(it->GetID(), h);

			// Resource View pointers for the status icons
			ID3D11ShaderResourceView* pRV1 = _dotTexture.GetTextureRV(); // Default dot
			ID3D11ShaderResourceView* pRV2 = _dotTexture.GetTextureRV();

			if (state == 0 && !questionMarkUsed)
			{
				// Show a question mark for the next available hint in the sequence
				pRV2 = _questionmarkTexture.GetTextureRV();
				questionMarkUsed = true;
			}
			else if ((state & 1) != 0)
			{
				// Hint has already been read/unlocked: Show a checkmark
				pRV1 = _checkTexture.GetTextureRV();
				it->Render(x + boxw * 2, y); // Render the actual hint text
			}
			else if ((state & 2) != 0)
			{
				// Hint is available but hasn't been bought yet
				it->Render(x + boxw * 2, y);
				questionMarkUsed = true;
			}

			// (DirectX rendering calls to draw the icons at the calculated coordinates)
			// ...
		}
		y += _categoryPadding;
	}
}

/**
 * Handles mouse clicks. 
 * If a player clicks on a locked hint, it triggers the "unlock" logic,
 * which usually deducts points from the player's total score.
 */
void CUAKMHintModule::BeginAction()
{
	// Determine which hint was clicked based on cursor Y position
	int selectedHintIndex = (int)((_cursorPosY - _startY) / _lineHeight);
	
	// Check if the player has enough points and confirm the action
	if (CanUnlockHint(selectedHintIndex))
	{
		// Update game data: Mark hint as "Read" and reduce Score
		UnlockHint(selectedHintIndex);
	}
}