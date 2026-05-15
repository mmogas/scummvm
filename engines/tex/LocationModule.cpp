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

#include "LocationModule.h"
#include "AnimationController.h"
// ... (other includes)

/**
 * CLocationModule manages the high-level logic for a 3D scene.
 * It handles input (Keyboard/Mouse), interaction modes, and movement physics.
 */

// Static variables for movement accumulation (WASD / Arrow Keys)
float CLocationModule::_movement_forward = 0.0f;
float CLocationModule::_movement_backward = 0.0f;
// ... (other movement floats)

ActionType CLocationModule::CurrentAction = ActionType::None;

CLocationModule::CLocationModule(int locationId, int startupPosition) : CModuleBase(ModuleType::Location)
{
	_locationId = locationId;
	_startupPosition = startupPosition;

	CurrentObjectIndex = -1; // Index of the 3D object the mouse is over
	CurrentActions = ActionType::None; // Bitmask of possible actions for hovered object
	CurrentAction = ActionType::None;  // Currently selected action (Look, Get, etc.)
}

/**
 * Update loop: Calculated every frame to handle smooth camera interpolation
 * and "Bobbing" effect while walking.
 */
void CLocationModule::Update()
{
	if (_loading) return;

	// Calculate movement vectors based on speed and friction
	_smooth_movement_x += (_movement_x - _smooth_movement_x) * 0.1f;
	_smooth_movement_z += (_movement_z - _smooth_movement_z) * 0.1f;

	// Apply vectors to the 3D location camera
	_location.SetPosition(_smooth_movement_x, _smooth_movement_z);
	_location.Update();
	
	// Raycast check: What is the player looking at?
	UpdateHoverObject();
}

/**
 * Interaction System: Determines which actions are valid for the 3D object
 * currently under the mouse cursor.
 */
void CLocationModule::UpdateHoverObject()
{
	float mx, my;
	CModuleController::GetMousePosition(mx, my);

	// Perform a 3D raycast from the mouse coordinates into the 3D world
	int objIdx = _location.Raycast(mx, my);

	if (objIdx != CurrentObjectIndex)
	{
		CurrentObjectIndex = objIdx;
		if (objIdx != -1)
		{
			// Get the action bitmask (e.g., 0x01 = Look, 0x02 = Get) from game data
			CurrentActions = CGameController::GetAvailableActions(objIdx);
		}
		else
		{
			CurrentActions = ActionType::None;
		}
	}
}

/**
 * Mouse Click Handler: Executes the selected action (Look, Get, Open, Use).
 */
void CLocationModule::OnMouseDown(float x, float y, int button)
{
	if (button == 0) // Left Click
	{
		if (CurrentObjectIndex != -1 && CurrentAction != ActionType::None)
		{
			// Trigger the interaction script or cinematic
			ExecuteAction(CurrentObjectIndex, CurrentAction);
		}
	}
	else if (button == 1) // Right Click
	{
		// Cycle through available actions (e.g., from 'Look' to 'Get')
		CycleActions();
	}
}

/**
 * Inventory & Travel: Opens sub-menus like the inventory grid
 * or the "Travel" map.
 */
void CLocationModule::OpenInventory()
{
	if (pOverlay == NULL)
	{
		// Push the Inventory Module onto the stack (pauses the 3D view)
		CModuleController::Push(new CInventoryModule());
	}
}

/**
 * Debug Controls: Toggles for developers to see engine internals.
 */
void CLocationModule::KeyDown(WPARAM key, LPARAM lParam)
{
	if (pOverlay == NULL)
	{
#ifdef DEBUG
		if (key == VK_F1)
		{
			_location._renderPoints = !_location._renderPoints; // View vertices
		}
		else if (key == VK_F2)
		{
			_location._renderLines = !_location._renderLines;   // View wireframe
		}
		else if (key == VK_F5)
		{
			_location._disableClipping = !_location._disableClipping; // No-clip mode
		}
#endif
		// Standard game controls (Escape for menu, I for Inventory, etc.)
		if (key == VK_ESCAPE)
		{
			CModuleController::Push(new CMainMenuModule());
		}
	}
}