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

class CUAKMGRSComputerModule : public CFullScreenModule
{
public:
	CUAKMGRSComputerModule();
	virtual ~CUAKMGRSComputerModule();

	virtual void Dispose();
	virtual void Render();

	virtual void KeyDown(WPARAM key, LPARAM lParam);

protected:
	virtual void Initialize();

	LPBYTE _animation;
	int _animationLength;

	void RenderButton(int x, int y, int image);

	LPBYTE _animationPointer;
	int _animationFrames;
	int _animationWidth;
	int _animationHeight;
	BOOL _animationActive;

	int _previousPage;

	// Input related
	virtual void BeginAction();
	virtual void Back();
};
