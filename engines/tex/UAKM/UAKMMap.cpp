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

#include "UAKMMap.h"
#include "BinaryData.h"
#include "LZ.h"
#include "MapData.h"
#include "Utilities.h"

/**
 * Initializes the global map system by decompressing and parsing 'MAP.LZ'.
 * This file serves as a master index, linking game locations to their respective assets.
 */
BOOL CUAKMMap::Init()
{
	// Decompress the main index file which contains metadata for all 64 game maps
	BinaryData map = CLZ::Decompress(L"MAP.LZ");

	if (map.Data != NULL && map.Length > 0)
	{
		LPBYTE data = map.Data;

		// The engine supports up to 64 distinct map slots (locations)
		for (int i = 0; i < 64; i++)
		{
			CMapData* pMD = new CMapData();

			// Read the pointer (offset) for the current map entry
			int ptr = GetInt(data, i * 4, 4);
			if (ptr > 0)
			{
				// --- Internal Headers ---
				// Skip first sub-table (terminated by 0xFFFF)
				for (int j = 0; j < 10; j++)
				{
					int w = GetInt(data, ptr, 2);
					ptr += 2;
					if (w == 0xffff) break;
				}

				// Skip past the file lookup table (up to 99 entries)
				for (int j = 0; j < 99; j++)
				{
					int w = GetInt(data, ptr, 2);
					ptr += 2;
					if (w == 0xffff) break;
				}

				ptr += 6; // Skip unknown 6-byte header/padding section

				// --- Script Assignment ---
				// Each location is driven by a bytecode script found in a specific .RES/Entry
				pMD->ScriptFileIndex = GetInt(data, ptr, 2);
				ptr += 2;
				pMD->ScriptFileEntry = GetInt(data, ptr, 2);
				ptr += 2;

				// --- Asset Mapping (Sound, Video, Images) ---
				// Resources are listed as pairs of Words (FileIndex, EntryIndex)
				// until an 0xFFFF terminator is reached.

				// Environmental Audio (Looping background sounds)
				while (ptr < map.Length)
				{
					FileMap fm;
					fm.File = GetInt(data, ptr, 2);
					ptr += 2;
					if (fm.File == 0xffff) break;
					fm.Entry = GetInt(data, ptr, 2);
					ptr += 2;
					pMD->EnvironmentAudioMap.push_back(fm);
				}

				// Standard Audio (SFX triggered by scripts)
				while (ptr < map.Length)
				{
					FileMap fm;
					fm.File = GetInt(data, ptr, 2);
					ptr += 2;
					if (fm.File == 0xffff) break;
					fm.Entry = GetInt(data, ptr, 2);
					ptr += 2;
					pMD->AudioMap.push_back(fm);
				}

				// Video sequences (Cutscenes or interactive video screens)
				while (ptr < map.Length)
				{
					FileMap fm;
					fm.File = GetInt(data, ptr, 2);
					ptr += 2;
					if (fm.File == 0xffff) break;
					fm.Entry = GetInt(data, ptr, 2);
					ptr += 2;
					pMD->VideoMap.push_back(fm);
				}

				// 2D Static Images (Sprites or inventory item graphics)
				while (ptr < map.Length)
				{
					FileMap fm;
					fm.File = GetInt(data, ptr, 2);
					ptr += 2;
					if (fm.File == 0xffff) break;
					fm.Entry = GetInt(data, ptr, 2);
					ptr += 2;
					pMD->ImageMap.push_back(fm);
				}

				// --- 3D World Metadata ---
				// Main location file index (the 3D geometry file)
				pMD->LocationFileIndex = GetInt(data, ptr, 2);
				ptr += 4; // Skip padding

				// List of allowed animation sequences for this map (1-byte IDs)
				while (ptr < map.Length)
				{
					int u = data[ptr++];
					if (u == 0xff) break;
					pMD->AnimationMap.push_back(u);
				}

				// List of 3D objects/meshes to be loaded into the scene (4-byte IDs)
				while (ptr < map.Length)
				{
					int u = GetInt(data, ptr, 4);
					ptr += 4;
					if (u == 0xffffffff) break;
					pMD->ObjectMap.push_back(u);
				}

				// --- Player Startup Positions ---
				// Offset 0x100 contains a secondary pointer table for spawn points
				ptr = GetInt(data, 0x100 + i * 4, 4);
				if (ptr > 0 && ptr < map.Length)
				{
					int nextPtr = GetInt(data, 0x104 + i * 4, 4);
					if (nextPtr != 0 && nextPtr < map.Length)
					{
						// Calculate number of 10-byte startup position structures
						int entries = (nextPtr - ptr) / 10;
						ReadStartupPositions(pMD, data, ptr, entries, 10);
					}
				}
			}

			_entries.push_back(pMD);
		}

		delete[] map.Data; // Cleanup the temporary decompression buffer
		return TRUE;
	}

	return FALSE;
}