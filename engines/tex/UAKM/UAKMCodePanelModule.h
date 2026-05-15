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

#include "FullScreenModule.h"
#include <unordered_map>
#include "D3D11-NoWarn.h"
#include "Texture.h"
#include "AmbientAudio.h"

class CUAKMCodePanelModule : public CFullScreenModule
{
public:
	CUAKMCodePanelModule(int parameter);
	virtual ~CUAKMCodePanelModule();

	virtual void Render();
	virtual void KeyDown(WPARAM key, LPARAM lParam);

protected:
	virtual void Initialize();

	int _parameter;

	void RenderDot(int pos);

	void ResetCode();
	void Key(int key);
	signed char _enteredCode[8];
	int _keyPos;

	CAmbientAudio _sound;

	ULONGLONG _passwordMessageTime;
	int _lastMessageOffset;

	int _wrongFrame;
	ULONGLONG _wrongFrameTime;

	int _correctFrame;
	ULONGLONG _correctFrameTime;

	void Render(int entry, int offset_x, int offset_y);

	// Input related
	virtual void BeginAction();
};
