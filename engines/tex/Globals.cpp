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
#include <list>
#include "Caption.h"
#include "GameBase.h"
#include "ModuleBase.h"

// Link the DirectX GUID library for COM interfaces
#pragma comment( lib, "dxguid.lib")

/**
 * GLOBAL ENGINE OBJECTS
 */

// Central DirectX wrapper for rendering
CDirectX dx;

// Tracks if videos should play in FullScreen or Windowed mode
VideoMode videoMode = VideoMode::FullScreen;

// Current state of dialogue interactions
ConversationOption conversationOption = ConversationOption::None;

/**
 * CAPTION SYSTEM (Double-Buffering)
 * The game uses two lists for captions to allow updating one 
 * while the other is being rendered, preventing flickering.
 */
std::list<CCaption*> captions1;
std::list<CCaption*> captions2;
std::list<CCaption*>* pDisplayCaptions = &captions1; // Current visible captions
std::list<CCaption*>* pAddCaptions = &captions2;     // Buffer for next captions

// The filesystem path where the game is installed
PWSTR gamePath = NULL;

float twopi = XM_PI * 2;
HWND _hWnd = NULL; // Main Win32 Window Handle

// Pointer to the user's settings (Volume, Resolution, Keybinds)
CConfiguration* pConfig = NULL;

// Global Mutex for thread-safe access to shared game state
CMutex _lock;

/**
 * DIALOGUE SYSTEM
 * Under a Killing Moon uses a 3-choice system (A, B, C) for Tex Murphy's responses.
 */
CDXDialogueOption DialogueOptions[3];
int DialogueOptionsCount = 0;
CDXFont TexFont; // The specific font used for Tex's internal monologue/speech

// Global Music Player (MIDI-based for the original 1994 experience)
CMIDIPlayer* pMIDI = NULL;

// Flag to differentiate behavior between UAKM and its sequel (Pandora Directive)
BOOL isUAKM = TRUE;

/**
 * OVERLAYS
 * Specialized UI layers for specific interactive mini-scenes.
 */
COverlay* pOverlay = NULL;              // General purpose interaction overlay
COverlay* pClimbLadderOverlay = NULL;    // Specific logic for climbing interactions
COverlay* pConvertPointsOverlay = NULL; // Logic for the game's point/scoring system
COverlay* pElevationModOverlay = NULL;  // Used for looking up/down logic

/**
 * CAPTION COLOURS
 * Default ARGB color values for the subtitle system.
 */
int DefaultCaptionColour1 = 0;
int DefaultCaptionColour2 = -1; // Usually represents white/opaque
int DefaultCaptionColour3 = -1;
int DefaultCaptionColour4 = 0;