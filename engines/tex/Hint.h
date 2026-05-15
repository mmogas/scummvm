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

#include <string>
// #include "DXMultiColouredText.h"

class CHint
{
public:
	CHint(int hintIndex, std::wstring text);
	~CHint();

	int GetHintIndex() { return _hintIndex; }

	int GetState() { return _state; }
	void SetState(int state) { _state = state; }

	void Render(float x, float y);

	void Prepare(int state, int col1, int col2, int col3, int col4, Rect hintRect);
	int Height() { return static_cast<int>(_mcText.Height()); }

protected:
	int _hintIndex;
	std::wstring _text;
	int _state;
	CDXMultiColouredText _mcText;
};
