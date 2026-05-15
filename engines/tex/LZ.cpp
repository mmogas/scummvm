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

#include "LZ.h"
#include <Windows.h>
#include "Utilities.h"

BinaryData CLZ::Decompress(LPBYTE pInput, int length)
{
	return Decompress(pInput, 0, length);
}

BinaryData CLZ::Decompress(LPBYTE pInput, int offset, int length)
{
	BinaryData ret;
	ret.Data = NULL;
	ret.Length = 0;

	if (pInput != NULL)
	{
		if (IsCompressed(pInput, offset, length))
		{
			int decompressedLength = GetInt(pInput, offset + 4, 4);
			if (decompressedLength > 0)
			{
				LPBYTE pOutput = new byte[decompressedLength];
				if (pOutput != NULL)
				{
					ret.Length = decompressedLength;
					ret.Data = pOutput;

					int outputPtr = 0;
					int bitsRead = 128;		// Skip headers
					int bitsToRead = 9;
					int mask = 0x1ff;
					int nextCode = 0x102;
					int lastCode = -1;

					int codeOffset[8192];	// Points to where to copy from
					int codeLength[8192];	// Length of code, including "next"
					ZeroMemory(&codeOffset, sizeof(codeOffset));
					ZeroMemory(&codeLength, sizeof(codeLength));

					while (outputPtr < decompressedLength)
					{
						int code = -1;

						int bitOffset = bitsRead / 8;
						int bitShift = bitsRead & 7;
						int mask = 0x1fff >> (13 - bitsToRead);

						int bytesToRead = min(length - bitOffset, 4);
						if (bytesToRead > 0)
						{
							code = GetInt(pInput, offset + bitOffset, bytesToRead);
							code >>= bitShift;
							bitsRead += bitsToRead;

							code &= mask;
						}

						if (code == 0x100)
						{
							// Reset
							bitsToRead = 9;
							mask = 0x1ff;
							nextCode = 0x102;
							lastCode = -1;
						}
						else if (code == 0x101 || code < 0)
						{
							// End of data
							break;
						}
						else if (code >= 0)
						{
							// Decode
							int tmp = outputPtr;

							if (outputPtr < decompressedLength)
							{
								if (code >= 0x102 && code < nextCode)
								{
									int codePtr = codeOffset[code];
									for (int c = 0; c < codeLength[code]; c++)
									{
										if ((outputPtr + c) < decompressedLength)
										{
											pOutput[outputPtr + c] = pOutput[codePtr + c];
										}
									}

									outputPtr += codeLength[code];
								}
								else if (code >= 0 && code <= 0xff)
								{
									pOutput[outputPtr++] = (byte)code;
								}
							}

							// Add code to dictionary
							if (nextCode < 0x2000)
							{
								codeOffset[nextCode] = tmp;
								codeLength[nextCode++] = outputPtr - tmp + 1;
							}

							if ((nextCode - 1) > mask && bitsToRead < 13)
							{
								bitsToRead++;
								mask <<= 1;
								mask |= 1;
							}
						}
					}

					if (outputPtr != decompressedLength)
					{
						int debug = 0;
					}
				}
			}
		}
	}

	return ret;
}

BinaryData CLZ::Decompress(LPWSTR pFileName)
{
	std::wstring path = gamePath;
	path += pFileName;

	BinaryData ret;
	ZeroMemory(&ret, sizeof(ret));

	// Open file
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// Get file size
		int fileSizeHigh;
		int fileSizeLow = GetFileSize(hFile, (LPDWORD)&fileSizeHigh);
		if (fileSizeLow > 0)
		{
			// Allocate memory
			LPBYTE pInput = new byte[fileSizeLow];
			if (pInput != NULL)
			{
				// Read file
				int bytesRead;
				if (ReadFile(hFile, pInput, fileSizeLow, (LPDWORD)&bytesRead, NULL))
				{
					if (fileSizeLow == bytesRead)
					{
						// Call decompress
						ret = Decompress(pInput, bytesRead);
					}
				}

				// Release memory
				delete[] pInput;
			}
		}

		// Close file
		CloseHandle((HANDLE)hFile);
	}
	else
	{
		std::wstring err = L"Could not open file ";
		err += pFileName;
		MessageBox(NULL, err.c_str(), L"Decompress LZ", MB_OK);
	}

	return ret;
}

BOOL CLZ::IsCompressed(LPBYTE pInput, int length)
{
	return IsCompressed(pInput, 0, length);
}

BOOL CLZ::IsCompressed(LPBYTE pInput, int offset, int length)
{
	int type = GetInt(pInput, offset, 4);
	return (type == 0x01454244 || type == 0x01434341);
}
