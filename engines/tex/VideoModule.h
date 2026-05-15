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
#include "ScriptBase.h"
#include <unordered_map>
//#include "String.h"
// #include "DXListBox.h"
#include "ScriptState.h"

enum class VideoType
{
	Single = 1,
	Scripted = 2,
};

class CVideoModule : public CModuleBase
{
public:
	CVideoModule(VideoType type, int dmapIndex, int activeScript = -1);
	CVideoModule(VideoType type, LPCWSTR fileName, int itemIndex);
	virtual ~CVideoModule();

	VideoType Type;

	virtual void Resize(int width, int height);

	virtual void Dispose();
	virtual void Render();

	virtual void Resume();

protected:
	virtual void Initialize();

	CScriptBase* _scriptEngine;
	CScriptState* _scriptState;
	CDXListBox _listBox;

	static void DialogueOptionA(LPVOID data);
	static void DialogueOptionB(LPVOID data);
	static void DialogueOptionC(LPVOID data);

	static void SelectOption(int option);
	void SelectDialogueOption(int option);

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void Back();

	int _askAboutBase;
};
