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
#include "Overlay.h"

COverlay::COverlay()
{
	_x = 0.0;
	_y = 0.0;

	_decision = 0;
}

void COverlay::Cursor(float x, float y, BOOL relative)
{
	_x = relative ? _x + x : x;
	_y = relative ? _y + y : y;

	for (auto it : _hitTestControls)
	{
		it->SetMouseOver(it->HitTest(_x, _y) != NULL);
	}
}
