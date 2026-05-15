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
#include "AmbientAudio.h"
#include "ModuleController.h"
#include "GameController.h"

/**
 * CAmbientAudio Class
 * This class handles ambient sounds and sound effects using XAudio2.
 * It manages sound loading, playback, volume control, and stereo panning.
 */

// Global storage for active sounds and sounds marked for cleanup
std::unordered_map<int, CAmbientAudio*> CAmbientAudio::Sounds;
std::list<CAmbientAudio*> CAmbientAudio::SoundsToDelete;

CAmbientAudio::CAmbientAudio(BinaryData bd)
{
	_sourceVoice = NULL;
	_pData = bd.Data;
	_length = bd.Length;
	_finished = TRUE;
	TimeDisposed = 0;
}

CAmbientAudio::~CAmbientAudio()
{
	if (_sourceVoice != NULL)
	{
		_sourceVoice->Stop();
		_sourceVoice->DestroyVoice();
		_sourceVoice = NULL;
	}

	if (_pData != NULL)
	{
		delete[] _pData;
		_pData = NULL;
	}

	_length = 0;
}

/**
 * Clear: Stops all currently playing ambient sounds.
 * It moves sounds to a "SoundsToDelete" list to ensure memory is 
 * released safely after the audio hardware is done with the buffers.
 */
void CAmbientAudio::Clear()
{
	for (auto it : Sounds)
	{
		it.second->Stop();
		it.second->TimeDisposed = GetTickCount64();
		SoundsToDelete.push_back(it.second);
	}

	Sounds.clear();
}

/**
 * Play: Submits the audio buffer to the XAudio2 engine.
 * It configures whether the sound should loop (e.g., background wind)
 * or play once (e.g., a door closing).
 */
void CAmbientAudio::Play(BOOL loop)
{
	if (_sourceVoice != NULL)
	{
		_finished = FALSE;

		// XAUDIO2 BUFFER SETUP:
		// Defines the memory region and playback behavior for the sound.
		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = _length;
		buf.pAudioData = _pData;
		
		// If looping, set XAUDIO2_LOOP_INFINITE
		if (loop)
		{
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		_sourceVoice->Start();
		_sourceVoice->SubmitSourceBuffer(&buf);
	}
}

/**
 * SetPan: Controls the Stereo Balance.
 * 0.0 is center, -1.0 is full left, 1.0 is full right.
 * In UAKM, this is used to make sounds feel like they come from 
 * specific directions in the 3D world.
 */
void CAmbientAudio::SetPan(float pan)
{
	if (_sourceVoice != NULL)
	{
		float volumes[2]; // [0] = Left, [1] = Right
		
		// Convert the -1 to 1 pan value into separate channel volumes
		volumes[0] = 0.5f - (pan / 2.0f);
		volumes[1] = 0.5f + (pan / 2.0f);
		
		_sourceVoice->SetOutputMatrix(NULL, 1, 2, volumes);
	}
}

/**
 * SetVolume: Adjusts the output level of the sound voice.
 */
void CAmbientAudio::SetVolume(float volume)
{
	if (_sourceVoice != NULL)
	{
		_sourceVoice->SetVolume(volume);
	}
}