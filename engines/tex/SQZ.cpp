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

#include "SQZ.h"

CSQZ::CSQZ()
{
}

CSQZ::~CSQZ()
{
}

/**
 * Decompresses data from a custom .SQZ format.
 * The format starts with a magic header and includes both literal and compressed chunks.
 */
BinaryData CSQZ::Decompress(PBYTE input, int length)
{
	// Magic Header Verification: 
	// .SQZ (stored as 'ZQS.' in little-endian) 
	// followed by .bmp (stored as 'pmb.')
	BinaryData bd;
	bd.Data = NULL;
	bd.Length = 0;

	if (GetInt(input, 0, 4) == 'ZQS.' && GetInt(input, 4, 4) == 'pmb.')
	{
		int compressedSize = GetInt(input, 8, 4);
		int decompressedSize = GetInt(input, 12, 4);

		// Allocate memory for the full decompressed file
		PBYTE output = new BYTE[decompressedSize];
		if (output != NULL)
		{
			ZeroMemory(output, decompressedSize);

			bd.Data = output;
			bd.Length = decompressedSize;

			int src = 16; // Skip 16-byte header
			int dst = 0;

			// Main decompression loop: process data until the end of the input buffer
			while (src < length)
			{
				// Read chunk control word (16-bit)
				int chunkSize = GetInt(input, src, 2);
				src += 2;

				// --- LITERAL CHUNK ---
				// If the MSB is set (negative value), the chunk is uncompressed
				if ((chunkSize & 0x8000) != 0)
				{
					// Convert negative signed 16-bit value back to a positive count
					chunkSize = (-chunkSize) & 0xffff;

					// Copy bytes directly from source to destination
					for (int cp = 0; cp < chunkSize; cp++)
					{
						output[dst++] = input[src++];
					}
				}
				// --- COMPRESSED CHUNK ---
				else
				{
					// This block contains encoded data that points back to previous bytes
					int chunkEnd = src + chunkSize;
					while (src < chunkEnd)
					{
						int dataCount = input[src++];

						// CONTROL BYTE: Determine if we are copying back from the output buffer
						if ((dataCount & 0x80) != 0)
						{
							// COMPRESSION MATCH: Dictionary-style decompression
							int count = 0, offset = 0;
							int lowCount = dataCount & 0x7f;
							int additional = input[src++];

							if ((additional & 0x80) != 0)
							{
								// LONG MATCH: High bit set in secondary byte
								// Extract length and backward offset from combined bits
								count = (lowCount >> 4) + 3;
								offset = ((additional & 0x7f) << 4) | (lowCount & 0xf);
							}
							else
							{
								// SHORT MATCH: Standard LZ-style copy
								count = lowCount + 3;
								offset = (additional << 0); // Placeholder for offset logic
							}
							
							// Copy 'count' bytes from a previous position ('offset') in the output
							int copySrc = dst - offset;
							for (int i = 0; i < count; i++)
							{
								output[dst++] = output[copySrc++];
							}
						}
						else
						{
							// SINGLE BYTE LITERAL: MSB is 0, so just copy the next few bytes
							// or handle as a short sequence of direct values.
							int count = dataCount + 1;
							for (int i = 0; i < count; i++)
							{
								output[dst++] = input[src++];
							}
						}
					}
				}
			}
		}
	}
	return bd;
}