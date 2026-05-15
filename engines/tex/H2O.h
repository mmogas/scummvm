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

#include "AnimBase.h"

class CH2O : public CAnimBase
{
public:
	CH2O(int factor = 1);
	~CH2O();

	virtual BOOL Init(LPBYTE pData, int length);
	virtual BOOL HasVideo() { return (_videoFramePointer != NULL); }

	void SetOutputBuffer(LPBYTE pBuffer, int width, int height, int offsetX, int offsetY, LPINT pPalette, int minColAllowChange, int maxColAllowChange);

protected:
	int _factor;

	virtual BOOL DecodeFrame();

	virtual int DecodeH2OAudio(LPBYTE source, LPBYTE destination, int chunkLength);

	BOOL ProcessFrame(int& offset, BOOL video);

	int _channels;
	int _depth;
	int _remainingLength;
	BOOL _audioCompressed;

	LPBYTE* _ppAudioOutputBuffers;
	int _audioOutputBufferIndex;

	int _minimumBitCount;
	int* _pDecodingTable;
	LPBYTE _pDecodingBuffer;
	int _decodedSize;

	void Unpack(int offset, int size);

	void SkipOrFill(int val);
	void PatternFill(int val);
	int GetPattern(int function, int input);
	void Write(int x, int y, int value);
	void PatternCopy(int val);
	void NewLine();

	int _inputOffset;
	int _x;
	int _y;
	int _remainingX;
	int _remainingY;
	int _qw;

	int _startAudioOnFrame;

	LPBYTE _configuredOutputBuffer;
	LPINT _configuredPalette;
	int _minColAllowChange;
	int _maxColAllowChange;
	int _renderWidth;
	int _renderHeight;
	int _offsetX;
	int _offsetY;

	int _h2oWidth;
	int _h2oHeight;
};
