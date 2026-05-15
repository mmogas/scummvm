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

#include "AnimBase.h"
#include "Utilities.h"
#include "Globals.h"
#include "ShaderStructs.h"

/**
 * CAnimBase Class
 * This is the abstract base class for all animation and video formats (BIC, PTF, etc.).
 * It manages shared functionality such as frame timing, audio-video synchronization,
 * palette translation, and DirectX 11 vertex buffer setup for rendering.
 */
CAnimBase::CAnimBase()
{
	_pInputBuffer = NULL;
	_pVideoOutputBuffer = NULL;
	_inputBufferLength = 0;
	_videoFramePointer = 0;
	_audioFramePointer = 0;
	_pPalette = NULL;

	_width = 0;
	_height = 0;
	_rate = 0;
	_frameTime = 0;
	_lastFrameUpdate = 0;
	_frame = 0;

	_vertexBuffer = NULL;

	_sourceVoice = NULL;
	_remainingAudioLength = 0;

	_audioFramesQueued = 0;
	_audioFramesProcessed = 0;
	_videoFramesProcessed = 0;

	_framePointer = 0;
	_done = FALSE;

    // PALETTE TRANSLATION:
    // Legacy 6-bit VGA palette values (0-63) are translated to 8-bit (0-255).
	for (int i = 0; i < 64; i++)
	{
		_colourTranslationTable[i] = (BYTE)(4.04762 * i);
	}
}

/**
 * Update: Handles frame progression.
 * Returns TRUE if it's time to display a new frame based on the _frameTime.
 */
BOOL CAnimBase::Update()
{
	if (_done) return FALSE;

	DWORD currentTime = GetTickCount();
	if ((currentTime - _lastFrameUpdate) > _frameTime)
	{
		_lastFrameUpdate = currentTime;
		return TRUE;
	}

	return FALSE;
}

/**
 * CreateBuffers: Prepares the GPU resources for rendering.
 * It calculates the aspect ratio and creates a 4-vertex quad (rectangle)
 * that fills the screen while maintaining the original video proportions.
 */
void CAnimBase::CreateBuffers(int width, int height, int factor)
{
	if (_vertexBuffer == NULL)
	{
		// SCALING CALCULATIONS:
        // Ensures the 320x240 or 640x480 video fits modern high-resolution screens.
		float sx = (float)_screenWidth / (float)(_width * factor);
		float sy = (float)_screenHeight / (float)(_height * factor);
		float scale = min(sx, sy);
		float sw = _width * scale;
		float sh = _height * scale;
		float ox = (_screenWidth - sw);
		float oy = (_screenHeight - sh);

		float left, right, top, bottom;

        // COORDINATE MAPPING:
        // Centers the video quad in the middle of the screen.
		left = floor((float)(ox / 2.0f)) + 0.5f;
		right = floor(left + sw) + 0.5f;
		top = floor((float)(-oy / 2.0f)) + 0.5f;
		bottom = floor(top - sh) + 0.5f;

        // VERTEX DATA:
        // Defines the two triangles that form the screen rectangle.
		TEXTURED_VERTEX* vertices = new TEXTURED_VERTEX[4];
		if (vertices != NULL)
		{
			vertices[0].position = XMFLOAT3(right, top, 0.0f);   // Top Right
			vertices[0].texture = XMFLOAT2(1.0f, 0.0f);

			vertices[1].position = XMFLOAT3(right, bottom, 0.0f); // Bottom Right
			vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

			vertices[2].position = XMFLOAT3(left, top, 0.0f);    // Top Left
			vertices[2].texture = XMFLOAT2(0.0f, 0.0f);

			vertices[3].position = XMFLOAT3(left, bottom, 0.0f); // Bottom Left
			vertices[3].texture = XMFLOAT2(0.0f, 1.0f);

			// GPU Buffer Creation...
		}
	}
}