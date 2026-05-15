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

// #include <Windows.h>
#include <unordered_map>

enum class InputAction
{
	Cursor,
	Action,
	Back,
	Cycle,
	MoveForward,
	MoveBack,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	Run,
	Next,
	Prev,
	Inventory,
	Travel,
	Hints,
};

enum class InputSource
{
	Unknown = 0,
	Mouse = 1,
	MouseButton = 2,
	MouseWheel = 4,
	Key = 8,
	JoystickAxis = 16,
	JoystickButton = 32,
	JoystickDPad = 64
};

BOOL IsJoystickSource(InputSource source);

inline InputSource operator|(InputSource a, InputSource b)
{
	return static_cast<InputSource>(static_cast<int>(a) | static_cast<int>(b));
}

inline InputSource operator&(InputSource a, InputSource b)
{
	return static_cast<InputSource>(static_cast<int>(a)& static_cast<int>(b));
}

struct InputMap
{
	InputSource JoystickSource;
	int JoystickIdentifier;
	InputSource MouseKeySource;
	int MouseKeyIdentifier;
	InputSource AcceptableSource;
	int CurrentData;
	InputSource CurrentSource;
	BOOL IsActive;
	int CurrentJoystickData;
};

class CInputMapping
{
public:
	static std::unordered_map<InputAction, InputMap> ControlsMap;
	static BOOL IgnoreNextMouseInput;
	static void LoadControlsMap();
	static void SaveControlsMap();

	static void Input(InputSource source, int identifier, int value);

private:
	static void WriteConfig(int* pConfig, int ix, InputAction action);
	static void ReadConfig(int* pConfig, int ix, InputAction action);
};
