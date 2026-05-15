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
#include <Windows.h>
#include <unordered_map>
#include <string>

class CFile
{
public:
	CFile();
	~CFile();

	enum class Mode : UINT
	{
		Read = GENERIC_READ,
		Write = GENERIC_WRITE
	};

	enum class Sharing
	{
		None = 0,
		Read = FILE_SHARE_READ,
		Write = FILE_SHARE_WRITE,
		Delete = FILE_SHARE_DELETE
	};

	enum class Creation
	{
		CreateNew = CREATE_NEW,
		CreateAlways = CREATE_ALWAYS,
		OpenExisting = OPEN_EXISTING,
		OpenAlways = OPEN_ALWAYS,
		TruncateExisting = TRUNCATE_EXISTING
	};

	enum class Flags
	{
		Normal = FILE_ATTRIBUTE_NORMAL,
	};

	enum class SeekMethod
	{
		Begin = FILE_BEGIN,
		Current = FILE_CURRENT,
		End = FILE_END
	};

	BOOL Open(LPCWSTR fileName, Mode mode = Mode::Read, Sharing share = Sharing::Read);
	void Close();
	DWORD Seek(DWORD distance, SeekMethod method = SeekMethod::Begin);
	int Read(LPBYTE pBuffer, int length);
	int Write(LPBYTE pBuffer, int length);
	DWORD Size()
	{
		LARGE_INTEGER li;
		::GetFileSizeEx(_handle, &li);
		return li.LowPart;
	}

	static BOOL Exists(LPCWSTR fileName);

protected:
	HANDLE _handle;

	static std::unordered_map<std::wstring, std::wstring> FileMap;
	static std::wstring Find(std::wstring path, std::wstring file);
};
