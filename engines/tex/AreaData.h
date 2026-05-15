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

#include <Windows.h>
#include "Utilities.h"

struct AreaData_Table1b
{
	short Y1;
	short Y2;
	short X1;
	short X2;
};

struct AreaData_Table3
{
	short X;
	short Y;
	BYTE unk1;	// ID fields?
	BYTE unk2;
	BYTE unk3;
	BYTE unk4;
};

struct AreaData_Table4
{
	BYTE unk1;
	BYTE unk2;
	short Y1;
	short Y2;
	short X1;
	short X2;
	BYTE WidthIndex;
	BYTE unk4;
	BYTE unk5;
	BYTE unk6;
};

class CAreaData
{
public:
	CAreaData();
	~CAreaData();

	void Init(LPBYTE data);

	// Table 1
	LPBYTE CategoryOptionOffsets;
	// Table 1b
	LPBYTE CategoryOptionCounts;
	AreaData_Table1b* CategoryAreas;

	// Table 2
	LPBYTE Table2;

	// Table 3
	AreaData_Table3* Table3;

	// Table 4
	AreaData_Table4* Table4;
};
