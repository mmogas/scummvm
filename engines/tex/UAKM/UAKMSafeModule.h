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
#include <unordered_map>
#include "D3D11-NoWarn.h"
#include "Texture.h"
#include "AmbientAudio.h"

class CUAKMSafeModule : public CModuleBase
{
public:
	CUAKMSafeModule(int parameter, BOOL alternatePalette);
	virtual ~CUAKMSafeModule();

	virtual void Resize(int width, int height);

	virtual void Dispose();
	virtual void Render();
	virtual void KeyDown(WPARAM key, LPARAM lParam);

protected:
	virtual void Initialize();

	int _parameter;
	BOOL _alternatePalette;

	LPBYTE _screen;

	void SetCursorArea(int x1, int y1, int x2, int y2);
	void ClipMouse(BOOL move);
	float _left;
	float _top;
	float _right;
	float _bottom;
	float _scale;

	int _palette[256];
	std::unordered_map<int, LPBYTE> _safeImageOffsets;
	std::unordered_map<int, LPBYTE> _safeSoundOffsets;

	LPBYTE _pImages;
	LPBYTE _pSounds;

	ID3D11Buffer* _vertexBuffer;
	CTexture _texture;
	bool _textureDirty;
	DWORD _frameTimes[14];

	ID3D11Buffer* _handVertexBuffer;
	CTexture _handTexture;

	void UpdateTexture();
	void PartialRender(int entry, int offsetX, int offsetY, BOOL updateTexture);

	BOOL _ready;
	DWORD _frameDelay;
	ULONGLONG _frameTime;
	int _startupFrame;
	int _keyDown[14];
	BYTE _enteredCode[8];
	int _keyPos;

	void Start();
	void Enter();
	void Number(int number);
	void Exit();

	CAmbientAudio _sound;

	void Press(int key, int sound);
	BOOL _codeCorrect;

	BOOL _flashingLightOn;
	int _rollingLightPosition;
	ULONGLONG _rollingLightTime;

	int _openSafeSequence;

	// Input related
	virtual void BeginAction();
	virtual void Back();
};
