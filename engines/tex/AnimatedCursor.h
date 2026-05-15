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

#include <list>
// #include "DXBitmap.h"

class CAnimatedCursor
{
public:
	CAnimatedCursor();
	~CAnimatedCursor();

	void Dispose();

	enum class CursorType
	{
		Arrow = 0,
		Note = 1,
		Crosshair = 2,
		Diskette = 3,
		Look = 4,
		Move = 5,
		Grab = 6,
		OnOff = 7,
		Talk = 8,
		Hint = 9,
		Open = 10,
		Loading = 11,
		Special = 12
	};

	void SetIcons(CursorType type, std::list<CDXBitmap*> icons);

	void SetPosition(float x, float y)
	{
		_x = x - _hotspotX;
		_y = y - _hotspotY;
	}
	void Render();

protected:
	CDXBitmap** _ppIcons;
	int _iconCount;
	float _x;
	float _y;
	float _hotspotX;
	float _hotspotY;

	ULONGLONG _lastChange;
	int _currentIcon;
	int _direction;
	DWORD _interval;
	DWORD _forwardInterval;
	DWORD _reverseInterval;
	DWORD _loopDelay;
	DWORD _loopDelayCounter;

	CursorType _type;
};
