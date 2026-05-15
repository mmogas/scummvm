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

class CCaption
{
public:
	CCaption() { _frame = -1; _text = NULL; _tex = FALSE; _processed = FALSE; }
	CCaption(int frame, char* text, BOOL tex) { _frame = frame; _text = text; _tex = tex; _processed = FALSE; }
	~CCaption() { }

	int Frame() { return _frame; }
	char* Text() { return _text; }
	BOOL Tex() { return _tex; }
	BOOL Processed() { return _processed; }
	void SetProcessed(BOOL processed) { _processed = processed; }

protected:
	int _frame;
	char* _text;
	BOOL _tex;
	BOOL _processed;
};
