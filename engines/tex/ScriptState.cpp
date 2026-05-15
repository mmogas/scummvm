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

#include "ScriptState.h"
#include "Utilities.h"

CScriptState::CScriptState()
{
	Script = NULL;
	Length = 0;

	DebugMode = FALSE;

	Clear();
}

void CScriptState::Clear()
{
	ExecutionPointer = 0;
	WaitingForMediaToFinish = FALSE;
	WaitingForInput = FALSE;
	WaitingForExternalModule = FALSE;
	SelectedOption = 0;
	SelectedValue = 0;

	AskAbout = FALSE;
	Offer = FALSE;
	Buy = FALSE;
	AskingAboutBuyables = FALSE;
	TopItemOffset = 0;

	Mode = InteractionMode::None;

	AllowedAction =  ActionType::None;
	QueryAction = FALSE;
	CurrentAction = ActionType::None;

	LastDialoguePoint = -1;
	FrameTrigger = -1;

	Parameter = -1;
}

int CScriptState::GetScript(int id)
{
	int i = _scriptEntries[id];
	return (i > 0) ? i : -1;
}

int CScriptState::GetInt(int offset, int size)
{
	return ::GetInt(Script, offset, size);
}

int CScriptState::Read8()
{
	return Script[ExecutionPointer++];
}

int CScriptState::Read8s()
{
	int ret = Script[ExecutionPointer++];
	if ((ret & 0x80) != 0) ret |= ~0xff;
	return ret;
}

int CScriptState::Read16()
{
	int ret = GetInt(ExecutionPointer, 2);
	ExecutionPointer += 2;
	return ret;
}

int CScriptState::Read16s()
{
	int ret = GetInt(ExecutionPointer, 2);
	if ((ret & 0x8000) != 0) ret |= ~0xffff;
	ExecutionPointer += 2;
	return ret;
}

int CScriptState::Read32()
{
	int ret = GetInt(ExecutionPointer, 4);
	ExecutionPointer += 4;
	return ret;
}

float CScriptState::Read16_16()
{
	int i = GetInt(ExecutionPointer, 4);
	ExecutionPointer += 4;
	return ((float)i) / 65536.0f;
}

float CScriptState::Read12_4()
{
	int i = GetInt(ExecutionPointer, 2);
	ExecutionPointer += 2;
	if ((i & 0x8000) != 0) i |= 0xffff0000;
	return ((float)i) / 16.0f;
}
