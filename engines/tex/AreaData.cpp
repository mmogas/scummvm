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

#include "AreaData.h"

/**
 * CAreaData Class
 * This class acts as a data parser for "Area" resource files.
 * Instead of copying data, it sets up pointers to specific offsets within 
 * a raw binary buffer, allowing fast access to game world metadata.
 */
CAreaData::CAreaData()
{
	// Initialize all pointers to NULL to prevent illegal memory access
	CategoryOptionOffsets = NULL;
	CategoryOptionCounts = NULL;
	CategoryAreas = NULL;
	Table2 = NULL;
	Table3 = NULL;
	Table4 = NULL;
}

CAreaData::~CAreaData()
{
	// Destructor is empty because this class points to external memory 
	// managed by a Resource Manager or GameController.
}

/**
 * Init: Maps the raw binary buffer to structured pointers.
 * The data format is packed, meaning tables follow each other immediately.
 */
void CAreaData::Init(LPBYTE data)
{
	// HEADER MAPPING:
	// The first 6 bytes usually contain global offsets.
	CategoryOptionOffsets = data;
	
	// The next 6 bytes contain counts for how many options each category has.
	CategoryOptionCounts = data + 6;
	
	// Table 1b starts at offset 12 and contains specific area definitions (Rects/Polys).
	CategoryAreas = (AreaData_Table1b*)(data + 12);
	
	// Move the working pointer forward past the fixed-size header (61 bytes).
	data += 61;

	// TABLE 2 MAPPING (Dynamic Size):
	// The first 2 bytes of this section define the length of the table.
	Table2 = data + 2;
	// Skip the current table: Length is stored in Little-Endian (byte 0 + byte 1 << 8).
	data += (2 + data[0] + (data[1] << 8));

	// TABLE 3 MAPPING:
	// Points to AreaData_Table3 structures, which likely define interaction logic.
	Table3 = (AreaData_Table3*)(data + 2);
	data += (2 + data[0] + (data[1] << 8));

	// TABLE 4 MAPPING:
	// Final table in the sequence, mapping to AreaData_Table4.
	Table4 = (AreaData_Table4*)(data + 2);
}