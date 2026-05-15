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

#include "LocationSprite.h"

/**
 * LocationSprite.cpp
 * * In the UAKM engine, "Sprites" are 2D images rendered within the 3D space.
 * Unlike LocationObjects (which are 3D meshes), Sprites always face the camera
 * (billboarding) or are used for flat decorative elements.
 * * This implementation file typically handles the specific behavior of 
 * sprite-based entities, such as scaling based on distance or 
 * frame-based animation for 2D objects.
 */

// Implementation of CLocationSprite methods would go here.
// In early 3D engines, sprites were heavily used to save CPU cycles 
// by avoiding complex polygon calculations for small details.