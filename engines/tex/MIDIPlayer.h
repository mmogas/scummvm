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

// #include <windows.h>
#include <mmsystem.h>
#include "LZ.h"
#include "Mutex.h"

class CMIDIPlayer
{
public:
	CMIDIPlayer();
	virtual ~CMIDIPlayer();

	void CloseDevice();
	void OpenDevice(UINT deviceId);

	virtual void Init(BinaryData data);
	void Stop();
	void Start();
	void Pause();
	void Resume();
	void SetVolume(float volume);

protected:
	static HMIDIOUT _handle;
	static BinaryData _data;

	HANDLE _hMIDIThread;
	DWORD _midiThreadId;

	static BOOL _midiEnabled;
	virtual DWORD Player();
	static DWORD WINAPI PlayerThread(LPVOID lpParameter);

	static LPBYTE _channels[16];
	static int _delays[16];
	static int _division;
	static int _duration;
	static int _volumes[16];

	static CMutex _midiMutex;
	static BOOL _changed;
};
