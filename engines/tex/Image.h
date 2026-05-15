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
#include "DoubleData.h"

class CImage : public CAnimBase
{
public:
	CImage(DoubleData dd, int width, int height, int factor = 1);
	CImage(LPBYTE palette, BinaryData bd, int width, int height, int factor = 1);
	virtual ~CImage();

	virtual BOOL Update();

protected:
	virtual BOOL DecodeFrame() { return (_frame == 0); }
	virtual void Init(LPBYTE palette, BinaryData bd, int width, int height, int factor);
};
