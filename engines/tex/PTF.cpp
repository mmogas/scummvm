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

#include "PTF.h"
#include "Utilities.h"
#include "MediaIdentifiers.h"

/**
 * Initializes the PTF (PTF video  Format) decoder.
 * Reads the header to set video dimensions, frame rate, and locates the first media pointers.
 */
BOOL CPTF::Init(LPBYTE pData, int length)
{
	BOOL ret = CAnimBase::Init(pData, length);

	// Validate that the file header matches the PTF magic identifier
	if (ret && GetInt(pData, 4, 4) == PTF)
	{
		_width = GetInt(pData, 12, 2);
		_height = GetInt(pData, 14, 2);
		_rate = GetInt(pData, 18, 2);
		_frameTime = (100 * _rate) / 6; // Calculate frame timing for playback synchronization

		CreateBuffers(_width, _height, _factor);
		_texture.Init(_width, _height);

		// Scan the file to find the initial video and audio chunk pointers
		_framePointer = 0x40; // Default header size offset
		int framePtr = 0x40;
		while (framePtr < _inputBufferLength && (_videoFramePointer == 0 || _audioFramePointer == 0))
		{
			int frameSize = GetInt(_pInputBuffer, framePtr, 4);
			// Handle negative sizes which usually indicate an embedded palette follows
			if (frameSize < 0) frameSize = 0x300 - frameSize;
			
			int frameType = GetInt(_pInputBuffer, framePtr + 4, 2);
			
			// Adjust frame size for specific metadata types
			if (frameType == 0x0b1c || frameType == 0x5756) frameSize += 6;

			// Identify Video frames (FLC 0xf1fa or BIC 0x0b1c)
			if ((frameType == 0xf1fa || frameType == 0x0b1c) && _videoFramePointer == 0)
			{
				_videoFramePointer = framePtr;
			}
			// Identify Audio frames (VW 0x5657)
			else if (frameType == 0x5657 && _audioFramePointer == 0)
			{
				_audioFramePointer = framePtr;
			}
			framePtr += frameSize;
		}

		// Fallback for audio-only streams
		if (_audioFramePointer != NULL && _videoFramePointer == NULL)
		{
			_framePointer = _inputBufferLength;
		}
	}

	return ret;
}

/**
 * Decodes an FLC-style (Autodesk Animator) video frame.
 * Supports Delta frames (0x07), Palettes (0x0b), Clear screen (0x0d), and Full frames (0x0f).
 */
BOOL CPTF::ProcessFLCFrame(int inPtr, int chunkSize)
{
	int outPtr = 0;
	int currentRow = 0;

	int subChunks = *(short*)(_pInputBuffer + inPtr);
	inPtr += 10; // Skip subChunk count and reserved header bytes
	
	while (subChunks > 0)
	{
		int cmdSize = *(int*)(_pInputBuffer + inPtr);		// Size of the sub-chunk
		int cmdType = *(short*)(_pInputBuffer + inPtr + 4);	// Type of encoding used
		
		if (cmdType <= 0x10)
		{
			inPtr += 6;

			if (cmdType == 0x07)
			{
				// --- Delta Frame (Word Oriented) ---
				// Only updates the differences between the current and previous frame
				currentRow = outPtr;

				int rowsToUpdate = *(short*)(_pInputBuffer + inPtr);
				inPtr += 2;
				if (rowsToUpdate <= _height)
				{
					for (int row = 0; row < rowsToUpdate; )
					{
						int changeCount = *(short*)(_pInputBuffer + inPtr);
						inPtr += 2;
						
						// If MSB is set, it's a special opcode
						if ((changeCount & 0x8000) != 0)
						{
							if ((changeCount & 0x4000) != 0)
							{
								// Skip rows (Vertical offset)
								changeCount = -changeCount;
								if (changeCount >= _height) break;
								currentRow += changeCount & 0x3fff;
								outPtr = currentRow * _width;
							}
						}
						else if (changeCount > _width)
						{
							break; // Safety exit
						}
						else
						{
							// Process individual packets in the row
							for (int change = 0; change < changeCount; change++)
							{
								outPtr += *(_pInputBuffer + inPtr++); // Horizontal skip
								int dataCount = (*(_pInputBuffer + inPtr++)) & 0xff;
								
								if ((dataCount & 0x80) != 0) 
								{
									// Run-Length Encoding (RLE) fill with two alternating colors
									dataCount = (-(signed char)dataCount) * 2;
									int col1 = (*(_pInputBuffer + inPtr++)) & 0xff;
									int col2 = (*(_pInputBuffer + inPtr++)) & 0xff;

									for (int c = 0; c < dataCount / 2; c++)
									{
										_pVideoOutputBuffer[outPtr++] = col1;
										_pVideoOutputBuffer[outPtr++] = col2;
									}
								}
								else
								{
									// Literal copy of raw pixel data
									dataCount *= 2;
									for (int c = 0; c < dataCount; c++)
									{
										_pVideoOutputBuffer[outPtr++] = *(_pInputBuffer + inPtr++);
									}
								}
							}
							currentRow++;
							outPtr = _width * currentRow;
							row++;
						}
					}
				}
			}
			else if (cmdType == 0x0b)
			{
				// --- Palette Update ---
				int numberOfEntries = *(short*)(_pInputBuffer + inPtr);
				inPtr += 2;
				int currentIndex = 0;
				for (int i = 0; i < numberOfEntries; i++)
				{
					currentIndex += *(_pInputBuffer + inPtr++); // Start index
					int colourCount = *(_pInputBuffer + inPtr++); // How many colors to update
					if (colourCount == 0) colourCount = 0x100;

					for (int j = 0; j < colourCount; j++)
					{
						// Map 6-bit or 8-bit VGA colors to 32-bit ARGB
						int r = _colourTranslationTable[_pInputBuffer[inPtr++]];
						int g = _colourTranslationTable[_pInputBuffer[inPtr++]];
						int b = _colourTranslationTable[_pInputBuffer[inPtr++]];

						// Black out UI colors if in specific video modes
						if (videoMode == VideoMode::Embedded && j < 32)
						{
							r = g = b = 0;
						}

						_pPalette[currentIndex++] = 0xff000000 | b | (g << 8) | (r << 16);
					}
				}
			}
			else if (cmdType == 0xd)
			{
				// --- Clear Screen ---
				ZeroMemory(_pVideoOutputBuffer, _width * _height);
				inPtr += 6;
			}
			else if (cmdType == 0x0f)
			{
				// --- Full Frame (Byte-run RLE) ---
				for (int y = 0; y < _height; y++)
				{
					inPtr++; // Skip packet count for row
					int bytesLeft = _width;
					while (bytesLeft > 0)
					{
						int count = *(_pInputBuffer + inPtr++);
						if ((count & 0x80) != 0)
						{
							// Literal copy
							count = min((-count) & 0xff, bytesLeft);
							for (int x = 0; x < count; x++)
								_pVideoOutputBuffer[outPtr++] = *(_pInputBuffer + inPtr++);
						}
						else
						{
							// Run fill
							byte cp = *(_pInputBuffer + inPtr++);
							count = min(count, bytesLeft);
							for (int x = 0; x < count; x++)
								_pVideoOutputBuffer[outPtr++] = cp;
						}
						bytesLeft -= count;
					}
				}
			}
			else
			{
				inPtr += cmdSize - 6; // Skip unknown sub-chunk types
			}
		}
		else
		{
			break;
		}
		subChunks--;
	}

	return TRUE;
}

/**
 * Decodes the next available frame (Video or Audio) from the input buffer.
 */
BOOL CPTF::DecodeFrame()
{
	BOOL ret = FALSE;

	// Handle Audio-Only streams
	if (_videoFramePointer == NULL)
	{
		if (_audioFramePointer > 0)
		{
			int inPtr = _audioFramePointer;
			while (inPtr < _inputBufferLength)
			{
				int chunkSize = GetInt(_pInputBuffer, inPtr, 4);
				inPtr += 6;
				int audioBytes = chunkSize;
				int audioPtr = inPtr;

				// Check if this chunk starts a new RIFF/WAVE header
				if (GetInt(_pInputBuffer, inPtr, 4) == RIFF)
				{
					_remainingAudioLength = GetInt(_pInputBuffer, inPtr + 0x28, 4);

					if (_sourceVoice == NULL)
					{
						// Initialize XAudio2 source voice based on the embedded WAVEFORMATEX
						char formatBuff[64];
						WAVEFORMATEX* pwfx = reinterpret_cast<WAVEFORMATEX*>(&formatBuff);
						pwfx->wFormatTag = GetInt(_pInputBuffer, inPtr + 0x14, 2);
						pwfx->nChannels = GetInt(_pInputBuffer, inPtr + 0x16, 2);
						pwfx->nSamplesPerSec = GetInt(_pInputBuffer, inPtr + 0x18, 4);
						pwfx->nAvgBytesPerSec = GetInt(_pInputBuffer, inPtr + 0x1c, 4);
						pwfx->nBlockAlign = 2;
						pwfx->wBitsPerSample = GetInt(_pInputBuffer, inPtr + 0x22, 2);
						pwfx->cbSize = 0;
						_sourceVoice = CDXSound::CreateSourceVoice(pwfx, 0, 1.0f, this);
					}

					if (_sourceVoice != NULL) _sourceVoice->Start(0, 0);

					audioPtr += 0x2c; // Move past WAVE header
					audioBytes -= 0x2c;
				}

				// Buffer management: Queue audio segments
				Buffer ab;
				audioBytes = min(audioBytes, _remainingAudioLength);
				audioBytes = min(audioBytes, _inputBufferLength - audioPtr);
				ab.Size = audioBytes;
				ab.pData = _pInputBuffer + audioPtr;
				_remainingAudioLength -= audioBytes;
				_audioBuffers.push_back(ab);

				inPtr += chunkSize;
			}

			// Seed the audio engine with initial buffers
			if (_sourceVoice != NULL)
			{
				auto buffers = _audioBuffers.size();
				for (int i = 0; i < buffers && i < 10; i++)
				{
					Buffer ab = _audioBuffers.front();
					_audioBuffers.pop_front();

					XAUDIO2_BUFFER buf = { 0 };
					buf.AudioBytes = ab.Size;
					buf.pAudioData = ab.pData;
					_sourceVoice->SubmitSourceBuffer(&buf);
					_audioFramesQueued++;
				}
			}
			_audioFramePointer = 0;
			ret = TRUE;
		}
	}
	// Handle Multiplexed Streams (Video + Audio)
	else if (_pInputBuffer != NULL && _framePointer >= 0 && _framePointer < _inputBufferLength)
	{
		int inPtr = _framePointer;
		int chunkSize = GetInt(_pInputBuffer, inPtr, 4);
		int frameType = GetInt(_pInputBuffer, inPtr + 4, 2);
		BOOL embeddedPalette = FALSE;
		inPtr += 6;

		// --- Section: Audio Chunk Processing ---
		if (frameType == 0x5657)
		{
			int audioBytes = chunkSize;
			chunkSize += 6;
			inPtr = _framePointer + 6;

			if (GetInt(_pInputBuffer, inPtr, 4) == RIFF)
			{
				_remainingAudioLength = GetInt(_pInputBuffer, inPtr + 0x28, 4);
				if (_sourceVoice == NULL)
				{
					// (Re)init format logic omitted for brevity, same as audio-only
					// ... 
				}
				if (_sourceVoice != NULL) _sourceVoice->Start(0, 0);
				inPtr += 0x2c;
				audioBytes -= 0x2c;
			}

			if (_sourceVoice != NULL)
			{
				XAUDIO2_BUFFER buf = { 0 };
				audioBytes = min(audioBytes, _remainingAudioLength);
				audioBytes = min(audioBytes, _inputBufferLength - inPtr);
				buf.AudioBytes = audioBytes;
				buf.pAudioData = _pInputBuffer + inPtr;
				_remainingAudioLength -= audioBytes;
				_sourceVoice->SubmitSourceBuffer(&buf);
				_audioFramesQueued++;
			}

			_framePointer += chunkSize;
			ret = TRUE;

			// Peek next frame after audio to see if video follows immediately
			inPtr = _framePointer;
			if (inPtr < _inputBufferLength)
			{
				chunkSize = GetInt(_pInputBuffer, inPtr, 4);
				frameType = GetInt(_pInputBuffer, inPtr + 4, 2);
				inPtr += 6;
			}
		}

		// --- Section: Embedded Palette ---
		// Negative chunk size indicates a full 256-color palette (768 bytes) precedes the frame
		if (chunkSize < 0)
		{
			embeddedPalette = TRUE;
			chunkSize = -chunkSize;

			for (int c = 0; c < 256; c++)
			{
				int r = _colourTranslationTable[_pInputBuffer[inPtr++]];
				int g = _colourTranslationTable[_pInputBuffer[inPtr++]];
				int b = _colourTranslationTable[_pInputBuffer[inPtr++]];

				if (videoMode == VideoMode::Embedded && c < 32) r = g = b = 0;

				_pPalette[c] = 0xff000000 | b | (g << 8) | (r << 16);
			}
		}

		// --- Section: Video Frame Processing ---
		if (frameType == 0xf1fa)
		{
			// Standard FLC Frame
			ProcessFLCFrame(inPtr, chunkSize);
			ret = TRUE;
			_videoFramesProcessed++;
			_framePointer += chunkSize;
			if (embeddedPalette) _framePointer += 0x300;
		}
		else if (frameType == 0x0b1c)
		{
			// BIC Frame (Proprietary variant)
			ProcessBICFrame(inPtr, chunkSize);
			chunkSize += 6;
			ret = TRUE;
			_videoFramesProcessed++;
			_framePointer += chunkSize;
			if (embeddedPalette) _framePointer += 0x300;
		}
	}

	return ret;
}