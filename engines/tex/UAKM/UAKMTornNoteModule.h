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
#include <vector>
#include "DXText.h"
#include "DXButton.h"
#include "PuzzlePiece.h"

class CUAKMTornNoteModule : public CModuleBase
{
public:
	CUAKMTornNoteModule(int item);
	virtual ~CUAKMTornNoteModule();

	virtual void Resize(int width, int height);

	virtual void Dispose();
	virtual void Render();

	CDXButton* _pBtnResume;
	static void OnResume(LPVOID data);

protected:
	virtual void Initialize();

	static CUAKMTornNoteModule* pUAKMTNM;

	int _item;
	int _newItem;
	int _palette[256];

	ID3D11Buffer* _vertexBuffer;

	int _numberOfImages;
	LPBYTE _pImageData;

	int _positionOffset;
	float _scale;
	float _screenHeight;

	CPuzzlePiece* _selectedScrap;
	POINT _pt;

	BOOL _completed;
	BOOL CheckCompleted();

	ULONGLONG _timeToExit;

	CDXText _caption;

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void EndAction();
	virtual void Back();
	virtual void Cycle();
	virtual void Next();
	virtual void Prev();
};
