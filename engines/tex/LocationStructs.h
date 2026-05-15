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

struct LocationObject
{
	float BoundingBoxX1;
	float BoundingBoxX2;
	float BoundingBoxY1;
	float BoundingBoxY2;
	float BoundingBoxZ1;
	float BoundingBoxZ2;
	int SubObjectIndex;
	int SubObjectCount;
};

struct LocationSubObject
{
	float BoundingBoxX1;
	float BoundingBoxX2;
	float BoundingBoxY1;
	float BoundingBoxY2;
	float BoundingBoxZ1;
	float BoundingBoxZ2;

	int ID;
	int Texture;
	int TriangleIndex;
	int TriangleCount;

	int VertexIndex;
	int VertexCount;
};

struct LocationTriangle
{
	int P1;
	int P2;
	int P3;
};
