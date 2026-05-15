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
#include <list>
#include "AnimatedCursor.h"
#include "Map.h"
#include "Mutex.h"

class CControllerData;

class CModuleController
{
public:
	static BOOL Init(CMap* map, CMap* dmap);

	static void GotFocus();
	static void LostFocus();

	static void Push(CModuleBase* pModule, BOOL overrideCurrent = FALSE);
	static void Pop(CModuleBase* pModule);
	static void SendToBack(CModuleBase* pModule);
	static void SendToFront(CModuleBase* pModule);

	static CModuleBase* Get(ModuleType type);
	static void ClearExcept(CModuleBase* pModule);

	static void Render();
	static void MouseMove(POINT pt);
	static void MouseDown(POINT pt, int btn);
	static void MouseUp(POINT pt, int btn);
	static void MouseWheel(int scroll);
	static void KeyDown(WPARAM key, LPARAM lParam);
	static void KeyUp(WPARAM key, LPARAM lParam);
	static void GamepadInput(InputSource source, int offset, int data);

	static void Resize(int width, int height);

	static CAnimatedCursor Cursors[12];

	static DWORD MainThreadId;
	static DWORD D3DThreadId;
	static DWORD TimerThreadId;

	static CModuleBase* CurrentModule;
	static CModuleBase* NextModule;

	static CMap* pMap;
	static CMap* pDMap;

	static void Resume();

protected:
	static std::list<CModuleBase*> Modules;

	static void PauseModule(CModuleBase* pModule);
	static void ResumeModule(CModuleBase* pModule);
};
