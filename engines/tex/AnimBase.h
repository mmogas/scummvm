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

#include "Globals.h"
// #include "D3D11-NoWarn.h"
#include <dsound.h>
#include "LZ.h"
#include <xaudio2.h>
// #include "DirectX.h"
#include "Texture.h"
#include "Mutex.h"

class CAnimBase : public CDXBase, public IXAudio2VoiceCallback
{
public:
	CAnimBase();
	~CAnimBase();

	virtual BOOL Init(LPBYTE pData, int length);
	BOOL Init(BinaryData bd);

	void Render();
	virtual BOOL Update();

	virtual BOOL IsWave() { return FALSE; }
	virtual BOOL HasVideo() { return TRUE; }

	virtual BOOL ShouldClearDXBuffer() { return TRUE; }

	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) { }
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() { }
	STDMETHOD_(void, OnStreamEnd)() { }
	STDMETHOD_(void, OnBufferStart)(void*) { }
	STDMETHOD_(void, OnBufferEnd)(void*)
	{
		_audioFramesProcessed++;

		if (_lock.Lock())
		{
			if (_sourceVoice != NULL)
			{
				if (_audioBuffers.size() > 0)
				{
					// Enqueue the next buffer
					Buffer ab = _audioBuffers.front();
					_audioBuffers.pop_front();

					XAUDIO2_BUFFER buf = { 0 };
					buf.AudioBytes = ab.Size;
					buf.pAudioData = ab.pData;
					_sourceVoice->SubmitSourceBuffer(&buf);

					_audioFramesQueued++;
				}
			}

			_lock.Release();
		}
	}
	STDMETHOD_(void, OnLoopEnd)(void*) { }
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) { }

	virtual BOOL IsDone() { return _done; }
	virtual void Skip();
	int Frame() { return _frame; }

	int Width() { return _width; }
	int Height() { return _height; }

	void Resize(int width, int height);

protected:
	std::list<Buffer> _audioBuffers;
	LPBYTE _pInputBuffer;
	LPBYTE _pVideoOutputBuffer;
	int _inputBufferLength;
	int _videoFramePointer;
	int _audioFramePointer;
	LPINT _pPalette;

	int _width;
	int _height;
	int _rate;
	int _frameTime;
	int _frame;

	ULONGLONG _lastFrameUpdate;

	virtual BOOL DecodeFrame() { return FALSE; }
	int _framePointer;

	virtual void CreateBuffers(int width, int height, int factor = 1);

	ID3D11Buffer* _vertexBuffer;

	IXAudio2SourceVoice* _sourceVoice;
	int _remainingAudioLength;

	int _audioFramesQueued;
	int _audioFramesProcessed;
	int _videoFramesProcessed;

	int _screenWidth;
	int _screenHeight;

	CTexture _texture;

	BOOL _done;

	BYTE _colourTranslationTable[64];

	CMutex _lock;
};
