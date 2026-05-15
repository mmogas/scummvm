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

#include "Map.h"
#include "LZ.h"
#include "Utilities.h"

CMap::~CMap()
{
	// Delete map entries
	std::vector<CMapData*>::iterator it = _entries.begin();
	std::vector<CMapData*>::iterator end = _entries.begin();
	while (it != end)
	{
		delete* (it++);
	}
}

CMapData* CMap::Get(int entry)
{
	if (_entries.size() > entry)
	{
		return _entries.at(entry);
	}

	return NULL;
}

StartupPosition CMap::GetStartupPosition(int index, int entry)
{
	StartupPosition sp;
	ZeroMemory(&sp, sizeof(sp));

	if (_entries.size() > index)
	{
		CMapData* pMap = _entries.at(index);
		if (pMap != NULL)
		{
			if (pMap->StartupPositions.size() > entry)
			{
				sp = pMap->StartupPositions.at(entry);
			}
		}
	}

	return sp;
}

int CMap::ReadStartupPositions(CMapData* pMapdata, LPBYTE data, int offset, int numberOfStartupPositions, int positionDataStructSize)
{
	for (int p = 0; p < numberOfStartupPositions; p++)
	{
		int ix = GetInt(data, offset, 2);
		if ((ix & 0x8000) != 0) ix |= ~0xffff;
		int iz = GetInt(data, offset + 2, 2);
		if ((iz & 0x8000) != 0) iz |= ~0xffff;
		int iy = GetInt(data, offset + 4, 2);
		if ((iy & 0x8000) != 0) iy |= ~0xffff;
		int iym = GetInt(data, offset + 6, 2);
		if ((iym & 0x8000) != 0) iym |= ~0xffff;
		int a = GetInt(data, offset + 8, 2);

		float x = ((float)ix) / 16.0f;
		float z = ((float)iz) / 16.0f;
		float y = ((float)iy) / 16.0f;
		float ym = ((float)iym) / 16.0f;
		float dy = (y - ym) / 6.0f;
		float maxy = ym + (dy * 8.0f);
		float miny = ym + (dy * 1.5f);
		float fa = ((float)a) / 10.0f;

		StartupPosition pos;
		pos.X = -x;
		pos.Y = -y;
		pos.Z = -z;
		pos.InitialEyeLevel = -dy * 6.0f;
		pos.MinYAdj = -dy * 8.0f;
		pos.MaxYAdj = -dy * 1.5f;
		pos.Elevation = -ym;
		pos.Angle = -fa * XM_PI / 180.0f;

		pMapdata->StartupPositions.push_back(pos);

		offset += positionDataStructSize;
	}

	return offset;
}
