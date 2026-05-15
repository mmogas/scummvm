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
#include "H2O.h"
#include "Utilities.h"
#include "MediaIdentifiers.h"

// Constants for managing the internal audio ring buffer
#define H2O_MAX_AUDIO_BUFFERS	20
#define H2O_AUDIO_BUFFER_SIZE	20000

/**
 * Constructor: Initializes the H2O video decoder.
 * Allocates large memory pools for audio buffers and the bitstream decoding table.
 */
CH2O::CH2O(int factor)
{
	_factor = factor; // Scale factor for the video playback
	_channels = 0;
	_depth = 0;
	_audioCompressed = FALSE;

	// Thread-safe allocation of audio output buffers
	if (_lock.Lock())
	{
		_ppAudioOutputBuffers = new LPBYTE[H2O_MAX_AUDIO_BUFFERS];
		for (int i = 0; i < H2O_MAX_AUDIO_BUFFERS; i++)
		{
			_ppAudioOutputBuffers[i] = new BYTE[H2O_AUDIO_BUFFER_SIZE];
			ZeroMemory(_ppAudioOutputBuffers[i], H2O_AUDIO_BUFFER_SIZE);
		}
		_audioOutputBufferIndex = 0;
		_lock.Release();
	}

	// Internal scratchpad for bitstream expansion
	_pDecodingTable = new int[65536];
	_pDecodingBuffer = new BYTE[1048576]; // 1MB buffer for decompressed frame data
	_decodedSize = 0;
}

/**
 * Main decoding entry point for a single video frame.
 * Parses the frame header and branches into different decompression sub-routines.
 */
BOOL CH2O::DecodeFrame()
{
	if (_remainingLength > 0)
	{
		// Extract the operation code from the bitstream
		int opcode = GetNextBits(4);
		
		/**
		 * H2O Opcodes:
		 * 0: Full Frame Refresh
		 * 1: Delta Frame (Inter-frame compression)
		 * 2: Audio Chunk
		 * 3: End of Stream / No data
		 */
		switch (opcode)
		{
			case 0: // Full frame update
				ProcessFullFrame();
				break;
			case 1: // Delta update (only pixels that changed)
				ProcessDeltaFrame();
				break;
			// ... other cases ...
		}
	}
	return TRUE;
}

/**
 * Efficiently writes a 32-bit pixel value to the output buffer.
 * In the 90s, this was a critical path for performance.
 */
void CH2O::SetPixel(int x, int y, int value)
{
	// Decompose the 32-bit integer into BGR/RGB bytes for the output surface
	_configuredOutputBuffer[(y + _offsetY) * _renderWidth + _offsetX + x + 0] = (byte)(value & 0xff);
	_configuredOutputBuffer[(y + _offsetY) * _renderWidth + _offsetX + x + 1] = (byte)((value >> 8) & 0xff);
	_configuredOutputBuffer[(y + _offsetY) * _renderWidth + _offsetX + x + 2] = (byte)((value >> 16) & 0xff);
	_configuredOutputBuffer[(y + _offsetY) * _renderWidth + _offsetX + x + 3] = (byte)((value >> 24) & 0xff);
}

/**
 * PatternCopy: A specialized decompression routine.
 * It uses a 4x4 or 2x2 bitmask to determine which pixels in a block 
 * should be updated from the bitstream, allowing for very efficient 
 * storage of sparse graphical data.
 */
void CH2O::PatternCopy(int val)
{
	int lineCountOrByteCount = (val & 0xfff) + 1;
	while (lineCountOrByteCount > 0 && _remainingY > 0)
	{
		int count = min(lineCountOrByteCount, _remainingX);
		_remainingX -= count;
		lineCountOrByteCount -= count;

		while (count > 0 && _remainingY > 0)
		{
			// Read bitmask (functions) for the current 4x4 block
			int functions = GetInt(_pDecodingBuffer, _inputOffset, 2) & 0xffff;
			_inputOffset += 2;

			for (int i = 0; i < 4; i++)
			{
				// Extract the 4-bit nibble for each row in the block
				int function = (functions >> (i * 4)) & 0xf;
				for (int b = 0; b < 4; b++)
				{
					// If the bit is set, read a new byte from the buffer and update output
					if ((function & (1 << b)) != 0)
					{
						_configuredOutputBuffer[(_y + i + _offsetY) * _renderWidth + _offsetX + _x + b] = _pDecodingBuffer[_inputOffset++];
					}
				}
			}
			_x += 4; // Advance to next horizontal block
			count--;
		}
	}
}