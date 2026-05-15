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

#pragma once

#include "ModuleBase.h"
#include "Location.h"
#include "ScriptBase.h"
// #include "DXText.h"
#include "ScriptState.h"

#define MOVEMENT_WALK_SPEED 0.2f
#define MOVEMENT_RUN_SPEED 0.5f

class CLocationModule : public CModuleBase
{
public:
	CLocationModule(int locationId, int startupPosition);
	virtual ~CLocationModule();

	virtual void Resize(int width, int height);

	virtual void Pause();
	virtual void Resume();
	virtual void Render();

	static float _movement_left;
	static float _movement_right;
	static float _movement_forward;
	static float _movement_backward;
	static float _movement_x;
	static float _movement_y;
	static float _movement_z;
	static float _smooth_movement_x;
	static float _smooth_movement_z;
	static float _speed;

	static ActionType CurrentAction;

	virtual void KeyDown(WPARAM key, LPARAM lParam);
#ifdef DEBUG
	virtual void MouseWheel(int scroll);
#endif

protected:
	virtual void Initialize();

	int _actionColour1;
	int _actionColour2;
	int _actionColour3;
	int _actionColour4;
	int _currentActionColour1;
	int _currentActionColour2;
	int _currentActionColour3;
	int _currentActionColour4;
	int _unavailableActionColour1;
	int _unavailableActionColour2;
	int _unavailableActionColour3;
	int _unavailableActionColour4;

	int _locationId;
	int _startupPosition;

	CLocation _location;

	// Properies for location object selection
	int CurrentObjectIndex;
	ActionType CurrentActions;
	int CurrentActionMousePointerIndex;
	CDXText _actionText[7];

	virtual void SelectMouseAction();
	void CycleActions(BOOL allowUse);

	CScriptState* _environmentScriptState;
	CScriptState* _actionScriptState;
	CScriptState* _queryActionScriptState;
	CScriptBase* _scriptEngine;
	CScriptState* _initScriptState;

	POINT _oldPoint;

	virtual void LoadLocation(int locationFileIndex, BinaryData script, std::wstring file, int entry);
	virtual void SetLocationPosition(StartupPosition pos) { _location.SetPosition(pos); _location.UpdateSprites(); }

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void Back();
	virtual void Cycle();
	virtual void MoveForward(float v);
	virtual void MoveBack(float v);
	virtual void MoveLeft(float v);
	virtual void MoveRight(float v);
	virtual void MoveUp(float y);
	virtual void MoveDown(float y);
	virtual void Run(BOOL run);
	virtual void Next();
	virtual void Prev();
	virtual void Inventory();
	virtual void Travel();
	virtual void Hints();

	void CycleItems(int direction);
};
