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

#include "UAKMScript.h"
#include "Globals.h"
#include "Utilities.h"
#include "AnimationController.h"
#include "GameBase.h"
// ... (other includes)

Point _lastKnownEyeBotPosition;

/**
 * Constructor: Initializes the opcode function table.
 * Each index in the array corresponds to a specific instruction in the game's bytecode.
 */
CUAKMScript::CUAKMScript()
{
	_functions[0x00] = &CUAKMScript::Function_80;
	_functions[0x01] = &CUAKMScript::Function_81;
	// ... (mapping continues)
	_functions[0x7f] = &CUAKMScript::Function_FF;
}

/**
 * Main Execution Loop for the Script Engine.
 * It reads opcodes from the execution pointer and dispatches them to the corresponding handlers.
 */
void CUAKMScript::Resume(CScriptState* pState, BOOL pause)
{
	// Ensure we have a valid script loaded for the current map
	if (pState->Script.Data != NULL && pState->ExecutionPointer < pState->Script.Length)
	{
		pState->ExitScript = FALSE;

		// Continue execution until an exit command is found or a media event (video/dialogue) starts
		while (pState->ExecutionPointer < pState->Script.Length && !pState->ExitScript && !pState->WaitingForMediaToFinish)
		{
			// Read the next opcode (instruction)
			int opCode = pState->Script.Data[pState->ExecutionPointer++];

			// Opcodes 0x80 to 0xFF are mapped to the first 128 indices of the function table
			if (opCode >= 0x80)
			{
				ScriptFunction f = _functions[opCode - 0x80];
				if (f != NULL)
				{
					// Execute the command (this pointer is the script engine, passed the current state)
					(this->*f)(pState);
				}
			}
			else
			{
				// Opcodes below 0x80 usually handle variable assignments or conditional branches
				ProcessStandardOpcode(pState, opCode);
			}
		}
	}
}

/**
 * Logic for standard opcodes (0x00 - 0x7F).
 * These typically handle the game's internal variables (the 'A' array) and flow control.
 */
void CUAKMScript::ProcessStandardOpcode(CScriptState* pState, int opCode)
{
	// Opcode bitmasking is used to determine if the instruction is a:
	// - Variable assignment (A[x] = y)
	// - Comparison (if A[x] == y)
	// - Jump (goto offset)
	
	// Implementation details depend on the specific byte-stream structure of UAKM
	// (Logic omitted for brevity as it follows the pattern above)
}

/**
 * Example Function Handler: Triggers a dialogue sequence.
 * 0x80 category functions usually involve interacting with the game world.
 */
void CUAKMScript::Function_80(CScriptState* pState)
{
	// Read parameters for the dialogue from the script stream
	int dialogueID = GetInt(pState->Script.Data, pState->ExecutionPointer, 2);
	pState->ExecutionPointer += 2;

	// Update the UI state and start the conversation module
	pState->WaitingForMediaToFinish = TRUE;
	CGameController::StartDialogue(dialogueID);
}

/**
 * Determines which actions are available for an object (Look, Take, Open, etc.)
 * by dry-running the script in "Query Mode".
 */
ActionType CUAKMScript::GetCurrentActions(CScriptState* pState, int currentObjectIndex)
{
	if (currentObjectIndex >= 0)
	{
		// Find the script sub-routine associated with the 3D object index
		pState->ExecutionPointer = pState->GetScript(currentObjectIndex);
		
		if (pState->ExecutionPointer >= 0)
		{
			// QueryAction = TRUE tells the opcodes to report availability 
			// instead of actually executing the visual effects.
			pState->QueryAction = TRUE;

			Resume(pState, TRUE);

			pState->QueryAction = FALSE;
		}
	}

	return pState->AllowedAction;
}

/**
 * Displays a full-screen image (Picture Module).
 * Often used for "Inspect" actions or finding clues.
 */
void CUAKMScript::Show(CScriptState* pState, int index)
{
	if (_mapEntry->ScriptFileEntry != 0 || _mapEntry->ScriptFileIndex != 0)
	{
		// Look up the image resource mapping from the MAP file
		FileMap fm = _mapEntry->ImageMap.at(index);
		
		// Set the flag to pause script execution until the user closes the image
		pState->WaitingForMediaToFinish = TRUE;
		
		// Push the image module to the top of the controller stack
		CModuleController::Push(new CPictureModule(fm.File, fm.Entry, this, pState));
	}
}