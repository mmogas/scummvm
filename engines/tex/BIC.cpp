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
#include "BIC.h"
#include "Utilities.h"
#include "MediaIdentifiers.h"

/**
 * Initializes the BIC decoder by reading the file header.
 * Sets up video dimensions, palette, and initializes the audio engine.
 */
BOOL CBIC::Init(LPBYTE pData, int length)
{
	_embeddedAudioSize = 0;
	_firstAudioFrame = 0;
	_chunkTest = 0;

	CAnimBase::Init(pData, length);

	// Validate that the file is a BIC format
	if (GetInt(pData, 0, 4) == BIC)
	{
		_width = GetInt(pData, 10, 2);
		_height = GetInt(pData, 12, 2);
		_rate = 6;      // Fixed frame rate for BIC
		_frameTime = 100;

		// Correction for specific embedded video resolutions found in certain versions
		if (videoMode == VideoMode::Embedded && _width == 640)
		{
			_width = 432;
			_height = 324;
		}

		CreateBuffers(_width, _height, _factor);
		_texture.Init(_width, _height);

		// --- Palette Initialization ---
		// Converts 6-bit VGA palette data (0-63) to 8-bit ARGB (0-255)
		for (int c = 0; c < 256; c++)
		{
			double r = pData[0x40 + c * 3 + 0];
			double g = pData[0x40 + c * 3 + 1];
			double b = pData[0x40 + c * 3 + 2];
			int ri = (byte)((r * 255.0) / 63.0);
			int gi = (byte)((g * 255.0) / 63.0);
			int bi = (byte)((b * 255.0) / 63.0);
			int col = 0xff000000 | bi | (gi << 8) | (ri << 16);
			_pPalette[c] = col;
		}

		// Initial frame pointers (Header usually ends at 0x340)
		_videoFramePointer = 0x340;
		_audioFramePointer = 0x340;

		_chunkTest = 10 - GetInt(pData, 0x12, 2);
		_remainingAudioLength = GetInt(pData, 0x3c, 4);
		_embeddedAudioSize = GetInt(pData, 0x10, 2);
		_firstAudioFrame = GetInt(pData, 0x12, 2);

		// Check for preloaded/cached wave data before the first video frame
		if (_chunkTest > 0)
		{
			int preload = _chunkTest * _embeddedAudioSize;
			_videoFramePointer += preload;
		}

		if (_audioFramePointer != NULL)
		{
			// Initialize XAudio2 source voice based on the header's WAVE specifications
			_remainingAudioLength = GetInt(_pInputBuffer, 0x3c, 4);

			char formatBuff[64];
			WAVEFORMATEX* pwfx = reinterpret_cast<WAVEFORMATEX*>(&formatBuff);
			pwfx->wFormatTag = GetInt(_pInputBuffer, 0x28, 2);
			pwfx->nChannels = GetInt(_pInputBuffer, 0x2a, 2);
			pwfx->nSamplesPerSec = GetInt(_pInputBuffer, 0x2c, 4);
			pwfx->nAvgBytesPerSec = GetInt(_pInputBuffer, 0x30, 4);
			pwfx->nBlockAlign = 2;
			pwfx->wBitsPerSample = GetInt(_pInputBuffer, 0x36, 2);
			pwfx->cbSize = 0;

			_sourceVoice = CDXSound::CreateSourceVoice(pwfx, 0, 1.0f, this);
			if (_sourceVoice != NULL)
			{
				if (_chunkTest > 0)
				{
					// Submit the preloaded audio buffer immediately
					XAUDIO2_BUFFER buf = { 0 };
					int audioBytes = _videoFramePointer - 0x340;
					buf.AudioBytes = audioBytes;
					buf.pAudioData = _pInputBuffer + 0x340;
					_remainingAudioLength -= audioBytes;
					_sourceVoice->SubmitSourceBuffer(&buf);
					_audioFramesQueued++;
				}
			}
		}

		_framePointer = _videoFramePointer;
		return TRUE;
	}

	return FALSE;
}

/**
 * Decodes a BIC video frame using 4x4 block-based decompression.
 * The 'type' byte acts as a bitmask for different encoding modes per block row.
 */
int CBIC::ProcessBICFrame(int inPtr, int chunkSize)
{
	int outPtr = 0;
	int end = inPtr + chunkSize;
	int currentRow = 0;
	BOOL good = TRUE;

	while (inPtr < end && good)
	{
		int type = *(_pInputBuffer + inPtr++); // Encoding bitmask for this block row
		
		// --- Type Bit 1: Solid Color Fill ---
		// Fills 4x4 blocks with a single color index
		if ((type & 1) != 0)
		{
			outPtr = currentRow * _width;
			BOOL readOffset = FALSE;
			int chunks = *(short*)(_pInputBuffer + inPtr);
			inPtr += 2;
			if (chunks < 0) { chunks = -chunks; readOffset = TRUE; }

			if (chunks >= 160) { good = FALSE; break; }

			while (chunks > 0)
			{
				if (readOffset) { outPtr += *(_pInputBuffer + inPtr++) * 4; chunks--; }
				int count = *(_pInputBuffer + inPtr++);
				for (int i = 0; i < count; i++)
				{
					byte b = *(_pInputBuffer + inPtr++);
					int copyOut = outPtr;
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) _pVideoOutputBuffer[copyOut + x] = b;
						copyOut += _width;
					}
					outPtr += 4;
				}
				chunks--;
				readOffset = TRUE;
			}
		}

		// --- Type Bit 2: 2-Color Pattern (Standard) ---
		// Uses two full 8-bit color indices and a 16-bit pattern (1 bit per pixel in 4x4 block)
		if (good && (type & 2) != 0)
		{
			outPtr = currentRow * _width;
			// ... (Offset handling same as above) ...
			// [Code logic omitted for brevity, same skip/count pattern]
			while (chunks > 0)
			{
				// ...
				for (int i = 0; i < count; i++)
				{
					int c1 = *(_pInputBuffer + inPtr++);
					int c2 = *(_pInputBuffer + inPtr++);
					int pattern = *(short*)(_pInputBuffer + inPtr);
					inPtr += 2;
					int copyOut = outPtr;
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) 
							_pVideoOutputBuffer[copyOut + x] = (byte)(((pattern & (1 << x)) != 0) ? c2 : c1);
						copyOut += _width;
						pattern >>= 4;
					}
					outPtr += 4;
				}
				// ...
			}
		}

		// --- Type Bit 4: Raw Block Data ---
		// Copies 16 bytes directly for a 4x4 block
		if (good && (type & 4) != 0)
		{
			outPtr = currentRow * _width;
			// ... 
			while (chunks > 0)
			{
				// ...
				for (int i = 0; i < count; i++)
				{
					int copyOut = outPtr;
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) _pVideoOutputBuffer[copyOut + x] = *(_pInputBuffer + inPtr++);
						copyOut += _width;
					}
					outPtr += 4;
				}
				// ...
			}
		}

		// --- Type Bit 8: 2-Color Pattern (Compressed Indices) ---
		// Uses 4-bit indices (nibbles) for the two colors and a 16-bit pattern
		if (good && (type & 8) != 0)
		{
			outPtr = currentRow * _width;
			// ...
			while (chunks > 0)
			{
				// ...
				for (int i = 0; i < count; i++)
				{
					int c = *(_pInputBuffer + inPtr++);
					int c2 = (c >> 4) & 0xf; // High nibble
					int c1 = c & 0xf;        // Low nibble
					int pattern = *(short*)(_pInputBuffer + inPtr);
					inPtr += 2;

					int copyOut = outPtr;
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++)
							_pVideoOutputBuffer[copyOut + x] = (byte)(((pattern & (1 << x)) != 0) ? c2 : c1);
						copyOut += _width;
						pattern >>= 4;
					}
					outPtr += 4;
				}
				// ...
			}
		}

		currentRow += 4; // Advance 4 pixels vertically for the next set of blocks
	}

	return end;
}

/**
 * Orchestrates the decoding of a single frame, including interleaved audio.
 */
BOOL CBIC::DecodeFrame()
{
	BOOL ret = FALSE;

	if (_pInputBuffer != NULL && _framePointer >= 0 && _framePointer < _inputBufferLength)
	{
		int chunkSize = *(int*)(_pInputBuffer + _framePointer);
		int end = ProcessBICFrame(_framePointer + 4, chunkSize);

		if (end > _framePointer)
		{
			_framePointer += chunkSize + 4;

			// --- Audio Interleaving ---
			// BIC files often interleave audio chunks immediately following the video frame data
			_chunkTest++;
			if (_chunkTest >= 1 && _remainingAudioLength > 0)
			{
				int remainingBufferLength = _inputBufferLength - _framePointer;
				int waveChunkSize = (remainingBufferLength == _remainingAudioLength) ? 
									 _remainingAudioLength : min(_embeddedAudioSize, _remainingAudioLength);
				
				_remainingAudioLength -= waveChunkSize;

				if (_sourceVoice != NULL)
				{
					XAUDIO2_BUFFER buf = { 0 };
					buf.AudioBytes = waveChunkSize;
					buf.pAudioData = _pInputBuffer + _framePointer;
					_sourceVoice->SubmitSourceBuffer(&buf);
					_audioFramesQueued++;
				}

				_framePointer += waveChunkSize;
			}

			// Trigger the audio start if we reached the designated sync frame
			if (_frame == _firstAudioFrame && _sourceVoice != NULL) _sourceVoice->Start(0, 0);

			ret = TRUE;
		}
	}

	return ret;
}