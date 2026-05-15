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

#include "UAKMDMap.h"
#include "BinaryData.h"
#include "LZ.h"
#include "Utilities.h"

/**
 * Initializes the DMAP (Dialogue/Dynamic Map) system.
 * Decompresses 'DMAP.LZ' and parses the location-specific asset tables.
 */
BOOL CUAKMDMap::Init()
{
	// Decompress the DMAP index file
	BinaryData dmap = CLZ::Decompress(L"DMAP.LZ");

	LPBYTE data = dmap.Data;
	if (data != NULL && dmap.Length > 0)
	{
		// Process 64 potential map slots
		for (int i = 0; i < 64; i++)
		{
			CMapData* pMD = new CMapData();

			// Read the jump-table pointer for this map entry
			int ptr = GetInt(data, i * 4, 4);
			if (ptr > 0)
			{
				// --- Header Section ---
				// Skip system configuration table (terminated by 0xFFFF)
				for (int j = 0; j < 10; j++)
				{
					int w = GetInt(data, ptr, 2);
					ptr += 2;
					if (w == 0xffff) break;
				}

				// Skip file lookup table (99 entries)
				for (int j = 0; j < 99; j++)
				{
					int w = GetInt(data, ptr, 2);
					ptr += 2;
					if (w == 0xffff) break;
				}

				ptr += 2; // Skip unknown word

				// --- Palette Assignment ---
				// DMAP often defines specific palettes for dialogue or transition screens
				int w1 = GetInt(data, ptr, 2); // Palette file index
				ptr += 2;
				int w2 = GetInt(data, ptr, 2); // Palette file entry
				ptr += 2;
				
				if (w1 != 0xffff)
				{
					// Entry for external palette loading logic (if needed)
				}

				// --- Object/Resource Mapping ---
				// Skip object-related 4-byte identifiers
				for (int j = 0; j < 30; j++)
				{
					ptr += 4;
				}

				// Skip unused A4 script table (specific to legacy Access Software code)
				while (ptr < dmap.Length)
				{
					int a = data[ptr++];
					if (a == 0xff) break;

					int a1 = GetInt(data, ptr, 2);
					ptr += 2;
					int a2 = GetInt(data, ptr, 2);
					ptr += 2;
				}

				// --- Secondary File Mappings ---
				int f1 = GetInt(data, ptr, 2); // Generic file index
				ptr += 2;
				int f2 = GetInt(data, ptr, 2); // Generic file entry
				ptr += 2;

				// --- Dialogue Script Assignment ---
				// The bytecode script that controls interactions in this specific context
				pMD->ScriptFileIndex = GetInt(data, ptr, 2);
				ptr += 2;
				pMD->ScriptFileEntry = GetInt(data, ptr, 2);
				ptr += 2;

				// --- Dynamic Video Mapping ---
				// Reads pairs of (File, Entry) until 0xFFFF. 
				// These are often "talking head" videos or transition FMVs.
				while (ptr < dmap.Length)
				{
					int fi1 = GetInt(data, ptr, 2);
					ptr += 2;
					if (fi1 == 0xffff) break;

					int fe1 = GetInt(data, ptr, 2);
					ptr += 2;

					FileMap fm;
					fm.File = fi1;
					fm.Entry = fe1;
					pMD->VideoMap.push_back(fm);
				}

				// --- Audio/Sound Effects Mapping ---
				while (ptr < dmap.Length)
				{
					int fi1 = GetInt(data, ptr, 2);
					ptr += 2;
					if (fi1 == 0xffff) break;

					int fe1 = GetInt(data, ptr, 2);
					ptr += 2;

					FileMap fm;
					fm.File = fi1;
					fm.Entry = fe1;
					pMD->AudioMap.push_back(fm);
				}
			}

			_entries.push_back(pMD);
		}

		delete[] dmap.Data; // Free the decompressed temporary buffer
		return TRUE;
	}

	return FALSE;
}