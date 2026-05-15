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
#include <string>
#include "Enums.h"

class CScriptState
{
public:
	CScriptState();

	virtual void Init(LPBYTE script, int length, std::wstring file, int entry) = NULL;
	void Clear();

	int GetInt(int offset, int size);
	int Read8();
	int Read8s();
	int Read16();
	int Read16s();
	int Read32();
	float Read16_16();
	float Read12_4();

	std::wstring ScriptFile;
	int ScriptEntry;

	int GetScript(int id);

	int ExecutionPointer;
	BOOL WaitingForMediaToFinish;
	BOOL WaitingForInput;
	BOOL WaitingForExternalModule;
	int SelectedOption;
	int SelectedValue;

	BOOL AskAbout;
	BOOL Offer;
	BOOL Buy;
	BOOL AskingAboutBuyables;

	int TopItemOffset;

	ActionType AllowedAction;
	BOOL QueryAction;
	ActionType CurrentAction;

	int LastDialoguePoint;
	int FrameTrigger;

	LPBYTE Script;
	int Length;

	BOOL DebugMode;

	InteractionMode Mode;

	int Parameter;

protected:
	std::unordered_map<int, int> _scriptEntries;
};
