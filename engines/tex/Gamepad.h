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

// #include "dinput-NoWarn.h"
#include <list>
#include "Utilities.h"
#include <string>

#define UM_GAMEPAD		WM_USER+0x2000

class CControllerData
{
public:
	InputSource Source;
	DWORD Offset;
	DWORD Type;
	DWORD Data;
};

class CGamepad
{
public:
	CGamepad(IDirectInput8* pInput, HWND hWnd, GUID deviceId);

	void Update();

	std::string GetName(int offset, int data);

private:
	IDirectInput8* _pInput;
	HWND _hWnd;
	GUID _deviceId;

	IDirectInputDevice8* _pDevice;

	static BOOL EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* context);

	DIJOYSTATE2 _state;

	std::list<CControllerData*> _controllerData;
};

class CGamepadController
{
public:
	CGamepadController(HWND hWnd);
	~CGamepadController();

	static void Init(HWND hWnd);
	static void Dispose();

	void AddGamepad(GUID deviceId);

	void Update();

	static CGamepadController* GamepadController;

	std::string GetName(int offset, int data);

private:
	static BOOL CALLBACK DeviceEnumCallback(const DIDEVICEINSTANCE* instance, void* context);

	IDirectInput8* _pInput;
	HWND _hWnd;

	std::list<CGamepad*> _pads;
};
