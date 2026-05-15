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

#include "File.h"

// Static map to cache the actual disk paths of requested files
std::unordered_map<std::wstring, std::wstring> CFile::FileMap;

CFile::CFile()
{
	_handle = INVALID_HANDLE_VALUE;
}

CFile::~CFile()
{
	Close();
}

/**
 * Find: Recursively searches for a file within a directory tree.
 * This is used to locate assets that might not be in the root directory.
 * @param path - The starting directory for the search.
 * @param file - The target filename to find.
 * @return The full path to the file if found, otherwise an empty string.
 */
std::wstring CFile::Find(std::wstring path, std::wstring file)
{
	std::wstring foundFile;
	WIN32_FIND_DATA fd;
	
	// Start enumeration in the current path
	HANDLE hFF = FindFirstFile((path + L"*").c_str(), &fd);
	if (hFF == INVALID_HANDLE_VALUE) return L"";

	do
	{
		std::wstring enumeratedFile = fd.cFileName;

		// If the entry is a directory, descend into it (excluding '.' and '..')
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((enumeratedFile != L".") && (enumeratedFile != L".."))
			{
				foundFile = Find(path + enumeratedFile + L"\\", file);
				if (foundFile.size() > 0) break; // File found in subfolder
			}
		}
		else
		{
			// If it's a file, check if it matches the target name
			if (enumeratedFile == file)
			{
				foundFile = path + file;
				break;
			}
		}
	} while (FindNextFile(hFF, &fd));

	FindClose(hFF);
	return foundFile;
}

/**
 * Open: Opens a file handle using Win32 API.
 * It first checks the FileMap cache to avoid expensive recursive disk searches.
 */
BOOL CFile::Open(LPCWSTR fileName, Mode mode, Sharing share)
{
	std::wstring realFile = fileName;
	
	if (mode == Mode::Read)
	{
		// Check if we already know the path to this file from a previous search
		if (FileMap.find(fileName) != FileMap.end())
		{
			realFile = FileMap[fileName];
		}
		else
		{
			// If not in cache, check if it exists at the provided path
			if (GetFileAttributes(fileName) == INVALID_FILE_ATTRIBUTES)
			{
				// Perform recursive search if the file isn't where expected
				std::wstring path = L"";
				std::wstring file = L"";
				size_t lastSeparator = realFile.find_last_of('\\');
				
				if (lastSeparator != -1)
				{
					path = realFile.substr(0, lastSeparator + 1);
					file = realFile.substr(lastSeparator + 1);
				}
				else
				{
					path = L".\\";
					file = realFile;
				}

				realFile = Find(path, file);
				FileMap[fileName] = realFile; // Cache the result for future use
			}
		}
	}

	// Determine creation disposition based on the requested mode
	Creation c = (mode == Mode::Read) ? Creation::OpenExisting : 
	             (mode == Mode::Write) ? Creation::CreateAlways : Creation::OpenAlways;
	
	// Native Windows call to obtain the file handle
	_handle = CreateFile(realFile.c_str(), (DWORD)mode, (DWORD)share, NULL, (DWORD)c, (DWORD)Flags::Normal, NULL);
	
	return (_handle != INVALID_HANDLE_VALUE);
}

/**
 * Basic File Operations Wrappers
 * These wrap standard Win32 file functions (Read, Write, Seek, Close).
 */
void CFile::Close()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}

DWORD CFile::Seek(DWORD distance, SeekMethod method)
{
	// Moves the file pointer (beginning, current, or end)
	return SetFilePointer(_handle, distance, NULL, (DWORD)method);
}

int CFile::Read(LPBYTE pBuffer, int length)
{
	int read = 0;
	ReadFile(_handle, pBuffer, length, (LPDWORD)&read, NULL);
	return read;
}

int CFile::Write(LPBYTE pBuffer, int length)
{
	int written = 0;
	WriteFile(_handle, pBuffer, length, (LPDWORD)&written, NULL);
	return written;
}