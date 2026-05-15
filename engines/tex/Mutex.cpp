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

#include "Mutex.h"
#include "Utilities.h"

CMutex::CMutex()
{
	_hMutex = CreateMutex(NULL, FALSE, NULL);

	_currentThreadId = 0;
	_lockCount = 0;
}

CMutex::~CMutex()
{
	CloseHandle(_hMutex);
}

BOOL CMutex::Lock(int timeout)
{
	DWORD currentThread = GetCurrentThreadId();
	if (_currentThreadId == currentThread)
	{
		_lockCount++;
		return TRUE;
	}

	//Trace(L"Lock requested by thread ");
	//Trace((int)currentThread);
	//Trace(L"\r\n");

	DWORD dwWaitResult = WaitForSingleObject(_hMutex, timeout);
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		//_currentThreadId = currentThread;
		_lockCount++;

		//Trace(L"Lock acquired by thread ");
		//Trace((int)currentThread);
		//Trace(L"\r\n");

		return TRUE;
	}

	return FALSE;
}

void CMutex::Release()
{
	_lockCount--;
	if (_lockCount == 0)
	{
		_currentThreadId = 0;

		//Trace(L"Lock released by thread ");
		//Trace((int)currentThread);
		//Trace(L"\r\n");
	}

	ReleaseMutex(_hMutex);
}
