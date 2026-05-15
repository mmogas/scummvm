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

// #include "DXAdapter.h"
#include <vector>

class CConfiguration
{
public:

	CConfiguration();
	CConfiguration(LPWSTR gameName);

	int Width				{640};
	int Height				{480};
	BOOL FullScreen			{FALSE};
	int ScreenMode			{-1};
	CDXAdapter* pAdapter    {nullptr};

	int MinAcceptedMode		{0};
	BOOL Captions			{TRUE};
	BOOL AlternativeMedia	{FALSE};
	BOOL PlayMIDI			{TRUE};
	int MIDIDeviceId		{-1};

	BOOL InvertY			{FALSE};
	float MouselookScaling  {1.0f};
	float FontScale			{1.0f};

	BOOL AnisotropicFilter  {TRUE};
	float Volume			{ 100.0f };
	float MIDIVolume		{ 100.0f };

	int NumberOfMIDIOutDevices {0};
	std::vector<MIDIOUTCAPSA> MIDIDevices{};

	void Save();

	std::wstring GetGameName() { return _gameName; }

protected:
	std::wstring _gameName{};
};
