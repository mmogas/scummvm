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
#include "DXButton.h"

class CUAKMEncodedMessageModule : public CModuleBase
{
public:
	CUAKMEncodedMessageModule();
	virtual ~CUAKMEncodedMessageModule();

	virtual void Resize(int width, int height) { }
	virtual void Dispose();
	virtual void Render();
	virtual void KeyDown(WPARAM key, LPARAM lParam);

	CDXButton* _pBtnResume;
	static void OnResume(LPVOID data);

protected:
	virtual void Initialize();

	static CUAKMEncodedMessageModule* pUAKMEMM;

	int _palette[256];

	ID3D11Buffer* _vertexBuffer;
	CTexture _texture;
	bool _textureDirty;

	float _scale;
	float _left;
	float _top;
	float _width;
	float _height;

	void UpdateTexture();
	LPBYTE _screen;

	std::unordered_map<char, char> _codeMap;

	void RenderChar(int x, int y, char c, BOOL transparent);
	void RenderText(char* pText, int yOffset, BOOL transparent);

	LPBYTE _font;
	std::unordered_map<char, LPBYTE> _fontMap;

	int _col1;
	int _col2;
	int _col3;
	int _col4;

	char* _pSaveMsg;

	ID3D11Buffer* _indicatorVertexBuffer;
	float _indicatorX;
	float _indicatorY;

	int _charPos;

	bool _completed;
	bool CheckCompleted();

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void Back();
};
