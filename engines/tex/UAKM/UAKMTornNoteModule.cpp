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

#include "UAKMTornNoteModule.h"
#include "Globals.h"
#include "GameController.h"
#include "Utilities.h"
#include "UAKMGame.h"
#include "InventoryModule.h"

/**
 * Relative distance offsets used to validate the correct assembly of the note.
 * Each pair represents the required (X, Y) distance between adjacent scraps.
 */
int TornNoteDistances[] = { -22, 0, -57, 0, -44, 0, 123, -10, -117, 4, 117, -46, -46, 12, -47, -8, -48, -8, 39, -17, 102, -13, 0, -14, -67, 2, -37, 6, 93, -47, -94, 26, 105, -28, -36, -20, -71, -6, -29, 12, 136, -42, -52, 2, -93, 5 };

CUAKMTornNoteModule* CUAKMTornNoteModule::pUAKMTNM = NULL;

#define PUZZLE_WIDTH	432.0f
#define PUZZLE_HEIGHT	300.0f

/**
 * Constructor: Sets up the puzzle based on which item (torn note) was opened.
 * Item 33 is the G.R.S. note, Item 57 is likely the Franco/Warehouse note.
 */
CUAKMTornNoteModule::CUAKMTornNoteModule(int item) : CModuleBase(ModuleType::TornNote)
{
	pUAKMTNM = this;
	_item = item;
	_selectedScrap = NULL;
	_completed = FALSE;
	// ... initialization of cursor bounds and scaling
}

/**
 * Handles scrap rotation. 
 * Pieces can be rotated in 90-degree increments (0, 1, 2, 3 corresponding to 0, 90, 180, 270 deg).
 */
void CUAKMTornNoteModule::Cycle()
{
	if (!_completed)
	{
		// If a piece is currently picked up, rotate it
		if (_selectedScrap != NULL)
		{
			_selectedScrap->Orientation = (_selectedScrap->Orientation + 1) & 3;
		}
		else
		{
			// Otherwise, check if the mouse is hovering over a piece to rotate it in place
			CPuzzlePiece* pScrap = CPuzzlePiece::HitTest(_cursorPosX, _cursorPosY, _item == 33 ? 4 : 0, _item == 33 ? 8 : 12);
			if (pScrap != NULL)
			{
				pScrap->Orientation = (pScrap->Orientation + 1) & 3;
			}
		}
	}
}

/**
 * Logic for picking up or dropping a scrap of paper.
 */
void CUAKMTornNoteModule::BeginAction()
{
	if (!_completed)
	{
		if (_selectedScrap == NULL)
		{
			// Try to pick up a scrap at the current mouse position
			_selectedScrap = CPuzzlePiece::HitTest(_cursorPosX, _cursorPosY, _item == 33 ? 4 : 0, _item == 33 ? 8 : 12);
			if (_selectedScrap != NULL)
			{
				_grabX = _cursorPosX - _selectedScrap->X;
				_grabY = _cursorPosY - _selectedScrap->Y;
				// Move the selected piece to the front of the rendering stack
				CPuzzlePiece::MoveToFront(_selectedScrap);
			}
		}
		else
		{
			// Drop the scrap and check if the overall puzzle is now solved
			_selectedScrap = NULL;
			CheckCompletion();
		}
	}
}

/**
 * Verifies if all scraps are correctly aligned and oriented.
 * If solved, it updates the game state (score, inventory transformation, etc.)
 */
void CUAKMTornNoteModule::CheckCompletion()
{
	_completed = TRUE;
	int distancePtr = (_item == 33) ? 0 : 16; // Pointer into the distance validation array

	for (int i = 0; i < (_item == 33 ? 8 : 12); i++)
	{
		CPuzzlePiece* p1 = _scraps[i];
		
		// 1. All pieces must be in the original orientation (0)
		if (p1->Orientation != 0) 
		{
			_completed = FALSE;
			break;
		}

		// 2. Check relative distance to the next piece in the sequence
		if (i < (_item == 33 ? 7 : 11))
		{
			CPuzzlePiece* p2 = _scraps[i + 1];
			int dx = p2->X - p1->X;
			int dy = p2->Y - p1->Y;

			// Compare against the hardcoded distance table with a small margin of error (2 pixels)
			if (abs(dx - TornNoteDistances[distancePtr]) > 2 || abs(dy - TornNoteDistances[distancePtr + 1]) > 2)
			{
				_completed = FALSE;
				break;
			}
			distancePtr += 2;
		}
	}

	// Logic for successful completion
	if (_completed)
	{
		// Reward player with points (Score)
		if (_item == 33) 
		{
			CGameController::AddScore(UAKM_SCORE_ASSEMBLE_NOTE);
			// Unlock new travel locations if applicable
			CGameController::SetData(UAKM_SAVE_TRAVEL + 14, 1);
		}

		// Inventory Transformation: Replace the "Torn Note" with a "Reassembled Note" item
		_newItem = (_item == 33) ? 103 : (_item == 57) ? 109 : -1;
		if (_newItem >= 0)
		{
			// Update the item ID in the player's inventory array
			int itemCount = CGameController::GetItemCount();
			for (int i = 0; i < itemCount; i++)
			{
				if (CGameController::GetItemId(i) == _item)
				{
					CGameController::SetData(UAKM_SAVE_INVENTORY + i, _newItem);
				}
			}
			CGameController::SetItemState(_item, 2); // State 2: Used/Gone
			CGameController::SetItemState(_newItem, 1); // State 1: Possessed
		}
	}
}

/**
 * Updates the position of the currently dragged scrap.
 */
void CUAKMTornNoteModule::Tick(int ticks)
{
	if (_selectedScrap != NULL)
	{
		_selectedScrap->X = _cursorPosX - _grabX;
		_selectedScrap->Y = _cursorPosY - _grabY;
	}
}