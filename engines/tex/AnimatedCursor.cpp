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
#include "AnimatedCursor.h"
#include "Globals.h"

/**
 * CAnimatedCursor Class
 * Handles the logic, animation, and rendering of the game's interactive cursors.
 * Each cursor (Arrow, Look, Move, etc.) can be a single static image or 
 * a multi-frame animation with specific hotspots and timing.
 */
CAnimatedCursor::CAnimatedCursor()
{
	_x = 0.0f;
	_y = 0.0f;
	_hotspotX = 0.0f;
	_hotspotY = 0.0f;
	_lastChange = 0;
	_ppIcons = NULL;
	_iconCount = 0;
	_currentIcon = 0;
	_direction = 0;
	_interval = 50;           // Standard animation speed in milliseconds
	_forwardInterval = 50;
	_reverseInterval = 50;
	_loopDelay = 0;           // Pause at the end of an animation cycle
	_loopDelayCounter = 0;
}

CAnimatedCursor::~CAnimatedCursor()
{
	Dispose();
}

/**
 * Dispose: Safely releases the memory used by the cursor's bitmaps.
 */
void CAnimatedCursor::Dispose()
{
	if (_ppIcons != NULL)
	{
		for (int i = 0; i < _iconCount; i++)
		{
			delete _ppIcons[i]; // Delete each individual frame
		}

		delete[] _ppIcons;      // Delete the pointer array
		_ppIcons = NULL;
	}
}

/**
 * Render: Draws the current frame of the cursor at its screen position.
 * It also calculates the next frame based on time and animation direction.
 */
void CAnimatedCursor::Render()
{
	if (_ppIcons != NULL)
	{
		// Draw the current frame
		CDXBitmap* currentCursor = _ppIcons[_currentIcon];
		if (currentCursor != NULL)
		{
			// The cursor is rendered at (x, y) but usually adjusted by 
			// the hotspot so the click point matches the visual tip.
			currentCursor->SetPosition(_x, _y);
			currentCursor->Render();
		}

		// ANIMATION LOGIC:
		// If the cursor has multiple frames, cycle through them.
		if (_iconCount > 1)
		{
			DWORD currentTime = GetTickCount();
			if ((currentTime - _lastChange) > _interval)
			{
				_lastChange = currentTime;

				// 'Ping-pong' animation logic: 
				// Moves frame index up or down based on current direction.
				if (_direction != 0)
				{
					_currentIcon += _direction;

					// Check for end or start of animation sequence
					if (_currentIcon >= (_iconCount - 1) || _currentIcon <= 0)
					{
						// Reverse direction for the next cycle
						_direction *= -1;
					}
				}
			}
		}
	}
}

/**
 * SetIcons: Initializes a cursor type with its visual assets and hotspots.
 */
void CAnimatedCursor::SetIcons(CursorType type, std::list<CDXBitmap*> icons)
{
	_type = type;
	_iconCount = static_cast<int>(icons.size());
	_ppIcons = new CDXBitmap * [_iconCount];
	
	// Transfer frames from the list to the internal array
	for (int i = 0; i < _iconCount; i++)
	{
		_ppIcons[i] = icons.front();
		icons.pop_front();
	}

	_direction = 1;

	// HOTSPOT DEFINITION:
	// The hotspot is the (X,Y) pixel within the cursor image that acts 
	// as the actual point of interaction.
	if (type == CursorType::Arrow)
	{
		_hotspotX = 0; // Top-left for the arrow
		_hotspotY = 0;
	}
	else if (type == CursorType::Crosshair)
	{
		// Different versions of the game (UAKM vs others) used 
		// slightly different crosshair offsets.
		if (isUAKM)
		{
			_hotspotX = 7; // Centered crosshair
			_hotspotY = 7;
		}
		else
		{
			_hotspotX = 2;
			_hotspotY = 3;
		}
	}
	else if (type == CursorType::Look)
	{
		_loopDelay = 10;
		_hotspotX = 14;
		_hotspotY = isUAKM ? 4 : 0;
	}
}