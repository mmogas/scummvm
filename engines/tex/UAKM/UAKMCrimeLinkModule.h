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
#include "AreaData.h"

#define CL_ANIMATION_FRAMES	36

class CUAKMCrimeLinkModule : public CFullScreenModule
{
public:
	CUAKMCrimeLinkModule(int parameter);
	virtual ~CUAKMCrimeLinkModule();

	virtual void Render();
	virtual void Pause();

protected:
	virtual void Initialize();

	static short CorrectSelections[15];
	static short PlayerSelections[15];
	static short Animation[CL_ANIMATION_FRAMES * 4];

	ULONGLONG _animationFrameTime;
	ULONGLONG _animationFrameDuration;
	int _animationIndex;
	int _windowX;
	int _windowY;
	int _flashFrame;

	void SetCursorArea(int x1, int y1, int x2, int y2);

	int _page;
	CAreaData _areaData;
	int _pageAnswerOffset;

	void PartialRender(int entry, int offsetX, int offsetY, BOOL updateTexture);

	POINT GetMouseOver();
	POINT _lastMouseOver;
	void DrawRectangle(int entry, BYTE colour, BOOL isCategory);
	void ReplaceColour(BOOL isCategory, int entry, BYTE src, BYTE dst);

	void ShowPage1();
	void ShowPage2();
	void ShowPage3();
	void ShowPage4();
	void ShowPage5();
	void ShowPage6();
	void ShowPage7();
	void ShowPage8();

	BOOL CheckSelection();
	void Click(int entry, int category);

	void UpdateSelection();
	int GetAnswerIndex(int category, int answer);

	int _parameter;

	void Render1(int x, int y);
	void Render0(int x, int y);
	void RenderPlus(int x, int y);

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
};
