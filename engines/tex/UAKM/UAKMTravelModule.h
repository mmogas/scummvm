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

#include "TravelModule.h"

class CUAKMTravelModule : public CTravelModule
{
public:
	CUAKMTravelModule();
	virtual ~CUAKMTravelModule();

	virtual void Render();

protected:
	virtual void Initialize();

	static constexpr short coordinates[] = { 8, 6, 245, 71, 26, 272, 168, 188, 349, 178, 335, 58, 26, 272, 171, 260, 276, 139, 349, 75, 168, 188, 18, 144, 130, 37, 307, 231, 36, 229, 19, 27, 266, 45, -1, -1 };
	static constexpr short hotspots[] = { 0, 0, 340, 74, 148, 275, 171, 191, 352, 181, 338, 61, 148, 275, 292, 263, 376, 142, 352, 78, 171, 191, 21, 147, 231, 40, 334, 234, 39, 232, 22, 30, 340, 48, -1, -1 };
	static constexpr signed char resultTable[] = { -1, -1, 2, -1, -1, 12, -1, 31, -1, 14, 1, -1, 15, -1, -1, 13, -1, 43, -1, 5, 21, -1, -1, 15, -1, 16, 26, -1, 33, -1, -1, 18, -1, 9, 2, -1, -1, 2, -1, 4, -1, 3, 14, -1, 5, -1, 8, -1, 9, -1, -1, 43, 20, -1, 16, -1, 19, -1, -1, 5, 22, -1, 23, -1, 25, -1, 24, -1, 26, -1, 27, -1, 30, -1, 28, -1, 29, -1,  };
};
