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

#include "Location.h"
#include "SQZ.h"
// ... (other includes)

/**
 * CLocation handles the 3D environment rendering and logic.
 * It manages 3D objects, sprites, animations, and collision detection.
 */

BOOL CLocation::_loading = FALSE;

#ifdef DEBUG
BOOL CLocation::_disableClipping = FALSE;
BOOL CLocation::_renderTextured = TRUE;
// ... (debug flags)
#endif

// Distance constants for the 3D engine's clipping plane
const double maxClipDistance = 2.0 / 3.0;
const double maxClipDistanceSquared = maxClipDistance * maxClipDistance;

// Bitmasks for object and sub-object properties
#define OBJECT_FLAGS_HIDDEN				0x80000000
#define SUBOBJECT_FLAGS_TRANSPARENT		0x00000001
#define SUBOBJECT_FLAGS_TEXTURED		0x00000002
#define SUBOBJECT_FLAGS_SPRITE			0x00000004
#define SUBOBJECT_FLAGS_ALPHA			0x00000020

/**
 * Main Update Loop: Handles movement, collision, and animation timing.
 */
void CLocation::Update()
{
	if (_loading) return;

	UpdateMovement();   // Process player input and physics
	UpdateCollision();  // Check if player is hitting walls/objects
	UpdateAnimations(); // Advance frame-based 3D animations
}

/**
 * 3D Animation Engine: Processes specialized byte-code for object animations.
 * This handles moving parts, flickering lights, or cycling textures.
 */
void CLocation::UpdateAnimations()
{
	double time = Timer.GetTime();

	for (int i = 0; i < _animationCount; i++)
	{
		if (Animations[i].Active && time >= Animations[i].NextFrameTime)
		{
			PBYTE pA = Animations[i].AnimDataPointer;
			BOOL frameEnd = FALSE;

			// Animation "Script" Interpreter
			while (!frameEnd)
			{
				BYTE cmd = *pA++;
				switch (cmd)
				{
				case 0x01: // Set Object Position/Rotation
					{
						int objIdx = GetInt(pA, 0, 2);
						// Update 3D matrix for the specific object...
						pA += 14; 
					}
					break;
				case 0x02: // Toggle Object Visibility
					{
						int objIdx = GetInt(pA, 0, 2);
						Objects[objIdx].Flags ^= OBJECT_FLAGS_HIDDEN;
						pA += 4;
					}
					break;
				case 0xFF: // End of current animation frame
					Animations[i].NextFrameTime = time + (GetInt(pA, 0, 4) * TIMER_SCALE);
					pA += 4;
					frameEnd = TRUE;
					break;
				}
			}
			Animations[i].AnimDataPointer = pA;
		}
	}
}

/**
 * 3D Projection: Projects a 3D coordinate (x,y,z) onto the 2D screen plane (sx, sy).
 * Uses a classic fixed-point or floating-point perspective transform.
 */
BOOL CLocation::Project(double x, double y, double z, float& sx, float& sy)
{
	// Transform world coordinates to camera space
	double tx = x - _camX;
	double ty = y - _camY;
	double tz = z - _camZ;

	// Rotate based on camera yaw/pitch
	double rx = tx * _camCosYaw - tz * _camSinYaw;
	double rz = tx * _camSinYaw + tz * _camCosYaw;
	
	// Culling: If the point is behind the camera, don't render it
	if (rz <= 0) return FALSE;

	// Apply perspective division
	double f = _viewDistance / rz;
	sx = (float)(_centerX + rx * f);
	sy = (float)(_centerY - ty * f);

	return (sx >= 0 && sx < _screenWidth && sy >= 0 && sy < _screenHeight);
}

/**
 * Collision Detection: Uses a simplified 2D/3D hybrid grid.
 * Checks if the player's new coordinates intersect with prohibited zones.
 */
void CLocation::UpdateCollision()
{
	// Get potential collision sectors based on player position
	int sectorX = (int)(_posX / SECTOR_SIZE);
	int sectorZ = (int)(_posZ / SECTOR_SIZE);

	// Iterate through static geometry "lines" in the current sector
	for (auto& line : _collisionSectors[sectorX][sectorZ])
	{
		if (IntersectPlayer(line))
		{
			// Push player back to previous valid position
			_posX = _oldPosX;
			_posZ = _oldPosZ;
			break;
		}
	}
}

/**
 * Render Loop: Draws the world.
 * 1. Opaque geometry (Walls, floors)
 * 2. Transparent/Alpha-blended objects (Glass, effects)
 * 3. Sprites (2D items in 3D space)
 */
void CLocation::Render()
{
	CShaders::SelectWorldShader();
	
	// Sort transparent objects from back-to-front (Painters Algorithm)
	// to ensure correct transparency blending.
	SortTransparentObjects();

	for (int i = 0; i < _visibleObjectCount; i++)
	{
		DrawObject(_visibleObjects[i]);
	}

#ifdef DEBUG
	if (_renderPaths) RenderPath(); // Visualize AI or camera paths
#endif
}

/**
 * Texture Mapper: Binds the correct game texture to the DirectX pipeline.
 * Under a Killing Moon uses many small textures packed into large "atlases".
 */
void CLocation::BindTexture(int textureId)
{
	if (textureId != _currentBoundTexture)
	{
		CTexture* pTex = CResourceManager::GetTexture(textureId);
		dx.SetShaderResource(0, pTex->GetSRV());
		_currentBoundTexture = textureId;
	}
}