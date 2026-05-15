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

#include <vector>

struct Fixed
{
	unsigned short Low;
	short High;
};

struct SubSprite
{
	Fixed XAdjust;
	Fixed YAdjust;
	int TextureIndex;
	int Flags;
	Fixed U1;
	Fixed V1;
	Fixed U2;
	Fixed V2;
};

struct FixedPoint
{
	Fixed X;
	Fixed Y;
	Fixed Z;
};

struct LocationSprite
{
	int TextureIndex;
	FixedPoint* Point;
	std::vector<SubSprite> Sprites;
};
