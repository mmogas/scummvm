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

#include <Windows.h>
#include <unordered_map>
#include "ScriptState.h"
#include "Map.h"
#include "Location.h"
#include "Mutex.h"

class CScriptBase
{
public:
	CScriptBase();
	~CScriptBase();

	virtual void Execute(CScriptState* pState, int id) = NULL;
	virtual void Resume(CScriptState* pState, BOOL breakWait = FALSE) = NULL;
	virtual ActionType GetCurrentActions(CScriptState* pState, int currentObjectIndex) { return ActionType::None; };
	virtual void PermformAction(CScriptState* pState, int id, ActionType action, int item) = NULL;
	virtual void SelectDialogueOption(CScriptState* pState, int option) = NULL;

	CMapData* _mapEntry;

	CLocation* _pLoc;

	CMutex _scriptLock;
};
