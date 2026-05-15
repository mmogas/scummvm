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

#ifndef __ENUMS__
#define __ENUMS__

enum class VideoMode
{
	Embedded = 0,
	FullScreen = 1
};

enum class ConversationOption
{
	None = 0,
	A = 1,
	B = 2,
	C = 3,

	AskAbout = 4,
	Offer = 7,
	Abort = 99,
};

enum class InteractionMode
{
	None = 0,
	AskAbout = 1,
	Offer = 2,
	Buy = 3
};

enum class ModuleType
{
	Video = 1,
	Location = 2,
	Picture = 3,
	CrimeLink = 4,
	Inventory = 5,
	NewsPaper = 6,
	Travel = 7,
	MainMenu = 8,
	UltraSafe = 9,
	TornNote = 10,
	EncodedMessage = 11,
	ShreddedNote = 12,
	CodePanel = 13,
	AAASafe = 14,
	ColonelsComputer = 15,
	GRSComputer = 16,
	Ferrelette = 17,
	Stasis = 18,
	ResumeGame = 19,
	Hints = 20,
	VidPhone = 21,
	Crossword = 22,
	NewGame = 23,
	Laptop = 24,
	DragDropPuzzle = 25,
};

enum class ActionType
{
	None = 0x00000000,
	Look = 0x00000001,
	Move = 0x00000002,
	Get = 0x00000004,
	OnOff = 0x00000008,
	Talk = 0x00000010,
	Open = 0x00000020,
	Use = 0x00000040,
	Terminate = 0x00008000,
};

enum class AnimationStatus
{
	NotStarted = 0,
	Running = 1,
	OnHold = 2,
	Completed = 4
};

enum class Alignment
{
	Default = 0x00,
	Left = 0x01,
	Right = 0x02,
	CenterX = 0x04,
	Top = 0x08,
	Bottom = 0x10,
	CenterY = 0x20,
	ScaleX = 0x40,
	ScaleY = 0x80,
	Scale = ScaleX | ScaleY,
	Crop = 0x100
};

#endif
