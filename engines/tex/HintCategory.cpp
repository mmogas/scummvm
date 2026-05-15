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

#include "HintCategory.h"
#include "GameController.h"

CHintCategory::CHintCategory(int categoryIndex, std::wstring title)
{
	_hintCategoryIndex = categoryIndex;
	_title = title;
}

CHintCategory::~CHintCategory()
{
	// TODO: Clear items from hint list
	//std::list<CHint*> _hints;
}

void CHintCategory::AddHint(int hintIndex, std::wstring text)
{
	Hints.push_back(new CHint(hintIndex, text));
}

void CHintCategory::Render(float x, float y, bool directory)
{
	if (directory)
	{
		_categoryTextBlack.Render(x, y);
	}
	else
	{
		_categoryText.Render(x, y);
	}
}

void CHintCategory::Prepare(int colBlack, int colBlue, int colCategory, int colOrange, int colGreen, int colScore, int colHighlight, int colShade, Rect directoryRect, Rect categoryRect, Rect hintRect)
{
	_categoryText.SetColours(colCategory, colCategory, colCategory);
	_categoryText.SetText(_title.c_str(), categoryRect);
	_categoryTextBlack.SetColours(colShade, colBlack, colBlack, colHighlight);
	_categoryTextBlack.SetText(_title.c_str(), directoryRect);
}

void CHintCategory::Reset()
{
	for (auto it : Hints)
	{
		it->SetState(-1);
	}
}
