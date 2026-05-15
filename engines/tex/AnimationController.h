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

#include "AnimBase.h"
#include "Mutex.h"
// #include "DXText.h"
#include "Image.h"

class CAnimationController
{
public:
	static void Init();
	static void Clear();
	static BOOL Load(LPCWSTR fileName, int itemIndex);
	static BOOL Skip();
	static BOOL IsWave();
	static BOOL HasAnim();
	static BOOL NoAnimOrWave();
	static BOOL UpdateAndRender(BOOL render = TRUE);
	static BOOL UpdateAndRender(CAnimBase* pAnim, BOOL render = TRUE);
	static BOOL IsDone();
	static BOOL AnimNotDoneOrCondition(BOOL condition);
	static BOOL NoVideoAnim();
	static int Frame();
	static int Frame(CAnimBase* pAnim);
	static int Exists() { return (_anim != NULL); };
	static void Resize(int width, int height);
	static void SetOutputBuffer(LPBYTE pBuffer, int width, int height, int offsetX, int offsetY, LPINT pPalette, int minColAllowChange, int maxColAllowChange);

	static CAnimBase* Load(BinaryData bd, int factor = 1);
	static CImage* LoadImage(DoubleData bd, int width, int height, int factor = 1);
	static CImage* LoadImage(LPBYTE palette, BinaryData image, int width, int height, int factor = 1);

	static int Width() { return (_anim != NULL) ? _anim->Width() : 0; }
	static int Height() { return (_anim != NULL) ? _anim->Height() : 0; }

	static void SetCaptionColours(int texColour1, int texColour2, int texColour3, int texColour4, int otherColour1, int otherColour2, int otherColour3, int otherColour4);

	static void RenderCaptions(float z);

protected:
	static CAnimBase* _anim;
	static CDXText* _pCaption;

	static int _texCaptionColour1;
	static int _texCaptionColour2;
	static int _texCaptionColour3;
	static int _texCaptionColour4;
	static int _otherCaptionColour1;
	static int _otherCaptionColour2;
	static int _otherCaptionColour3;
	static int _otherCaptionColour4;
};
