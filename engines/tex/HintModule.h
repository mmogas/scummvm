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
#include "HintCategory.h"
#include "Texture.h"
// #include "DXButton.h"

class CHintModule : public CModuleBase
{
public:
	CHintModule();
	~CHintModule();

	virtual void Resize(int width, int height);
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void Back();

protected:
	std::list<CHintCategory*> _activeHintCategories;

	CTexture _blankTexture;
	CTexture _dotTexture;
	CTexture _checkTexture;
	CTexture _questionmarkTexture;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _hintVertexBuffer;
	ID3D11Buffer* _categoryVertexBuffer;

	CHintCategory* _pCurrentHintCategory;

	int _colBlack;
	int _colBlue;
	int _colCategory;
	int _colOrange;
	int _colGreen;
	int _colScore;
	int _colHighlight;
	int _colShade;

	CDXButton* _pBtnResume;
	CDXButton* _pBtnDirectory;

	CTexture _hintTexture;
	CTexture _categoryTexture;
};
