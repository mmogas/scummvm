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

#include "AnimationController.h"
#include "Globals.h"
#include "PTF.h"
#include "BIC.h"
#include "SilentBIC.h"
#include "Wave.h"
#include "Utilities.h"
#include "Image.h"
#include "MediaIdentifiers.h"

/**
 * CAnimationController Class
 * Acts as a Factory and Manager for all animation and video formats in the game.
 * It detects the file type from binary headers and instantiates the 
 * correct player (BIC for video, PTF for interactive paths, etc.).
 */

CAnimBase* CAnimationController::_anim = NULL;
CDXText* CAnimationController::_pCaption = NULL;

/**
 * Clear: Stops any running animation and frees the associated memory.
 * This is called whenever the game switches scenes or stops a movie.
 */
void CAnimationController::Clear()
{
	if (_anim != NULL)
	{
		delete _anim; // Polymorphic delete: calls the specific subclass destructor
		_anim = NULL;
	}

	if (_pCaption != NULL)
	{
		_pCaption->SetText(""); // Clear subtitles
	}
}

/**
 * Load: The core Factory Method.
 * Examines the "Magic Number" (identifier) at the start of the binary data 
 * to determine which decoder to use.
 */
CAnimBase* CAnimationController::Load(BinaryData bd, int factor)
{
	CAnimBase* pAnim = NULL;

	// MEDIA IDENTIFICATION:
	// Access Software used various custom formats for UAKM.
	// Identifiers are usually stored in the first few bytes of the file.
	int id = GetInt(bd.Data, 0, 4);

	if (id == MEDIA_ID_PTF)
	{
		// PTF (Path Table File): Used for interactive movement and branching paths.
		pAnim = new CPTF();
	}
	else if (id == MEDIA_ID_BIC)
	{
		// BIC: The standard full-motion video (FMV) format for the game.
		pAnim = new CBIC(factor);
	}
	else if (id == MEDIA_ID_SBIC)
	{
		// Silent BIC: Video files without an embedded audio track.
		pAnim = new CSilentBIC();
	}
	else if (id == MEDIA_ID_WAVE)
	{
		// WAVE: Standard audio-only resources.
		pAnim = new CWave();
	}

	// Initialization: Once the correct object is created, load the data into it.
	if (pAnim != NULL) 
	{
		pAnim->Init(bd);
	}

	return pAnim;
}

/**
 * Render: Delegates the drawing task to the currently active animation.
 */
void CAnimationController::Render()
{
	if (_anim != NULL)
	{
		_anim->Render();
	}
}

/**
 * Update: Updates the frame state.
 * Returns TRUE if a new frame was processed, FALSE if the animation is finished.
 */
BOOL CAnimationController::Update()
{
	if (_anim != NULL)
	{
		return _anim->Update();
	}

	return FALSE;
}

/**
 * SetCaptionColours: Manages the styling of subtitles (captions).
 * UAKM uses specific color schemes for different characters or UI states.
 */
void CAnimationController::SetCaptionColours(int texColour1, int texColour2, int texColour3, int texColour4, int otherColour1, int otherColour2, int otherColour3, int otherColour4)
{
	_texCaptionColour1 = texColour1;
	_texCaptionColour2 = texColour2;
	_texCaptionColour3 = texColour3;
	_texCaptionColour4 = texColour4;
	
	// 'Other' colors are often used for shadows or outlines in the text
	_otherCaptionColour1 = otherColour1;
	_otherCaptionColour2 = otherColour2;
	_otherCaptionColour3 = otherColour3;
	_otherCaptionColour4 = otherColour4;
}