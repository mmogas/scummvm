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
#include <list>
#include "Hint.h"
// #include "DXMultiColouredText.h"

class CHintCategory
{
public:
	CHintCategory(int categoryIndex, std::wstring title);
	~CHintCategory();

	void AddHint(int hintIndex, std::wstring text);

	void Prepare(int colBlack, int colBlue, int colCategory, int colOrange, int colGreen, int colScore, int colHighlight, int colShade, Rect directoryRect, Rect categoryRect, Rect hintRect);
	void Render(float x, float y, bool directory);

	std::list<CHint*> Hints;

	float Width() { return _categoryText.Width(); }
	void Reset();

private:
	int _hintCategoryIndex;
	std::wstring _title;

	CDXMultiColouredText _categoryText;
	CDXText _categoryTextBlack;
};
