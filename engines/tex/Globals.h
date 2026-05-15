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


#include "Caption.h"
#include "ScriptBase.h"
#include <list>
#include <iterator>
#include "ConstantBuffers.h"
#include "Shaders.h"
#include "Configuration.h"
#include "Mutex.h"
#include "ModuleController.h"
#include "MIDIPlayer.h"
// #include "PDMIDIPlayer.h"
#include "Enums.h"
#include "Overlay.h"

extern float twopi;

class CAnimBase;

extern CDirectX dx;
extern VideoMode videoMode;
extern ConversationOption conversationOption;
extern std::list<CCaption*> captions1;
extern std::list<CCaption*> captions2;
extern std::list<CCaption*>* pDisplayCaptions;
extern std::list<CCaption*>* pAddCaptions;
extern PWSTR gamePath;

extern CMutex _lock;

inline Alignment operator|(Alignment a, Alignment b)
{
	return static_cast<Alignment>(static_cast<int>(a) | static_cast<int>(b));
}

inline Alignment operator&(Alignment a, Alignment b)
{
	return static_cast<Alignment>(static_cast<int>(a) & static_cast<int>(b));
}

extern HWND _hWnd;

extern CConfiguration* pConfig;

extern CDXDialogueOption DialogueOptions[3];
extern int DialogueOptionsCount;
extern CDXFont TexFont;

extern CMIDIPlayer* pMIDI;

extern BOOL isUAKM;

extern COverlay* pOverlay;
extern COverlay* pClimbLadderOverlay;
extern COverlay* pConvertPointsOverlay;
extern COverlay* pElevationModOverlay;

extern int DefaultCaptionColour1;
extern int DefaultCaptionColour2;
extern int DefaultCaptionColour3;
extern int DefaultCaptionColour4;
