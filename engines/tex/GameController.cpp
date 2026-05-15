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

#include "GameController.h"
#include "Utilities.h"

CGameBase* CGameController::Game = NULL;

std::unordered_map<int, std::wstring> CGameController::FileMap;

BOOL CGameController::CanCancelTravel = TRUE;

std::unordered_map<int, std::wstring> CGameController::AskAboutMap;
std::unordered_map<int, std::wstring> CGameController::ItemMap;
std::unordered_map<int, std::wstring> CGameController::BuyableItemMap;

BOOL CGameController::ItemsChanged = FALSE;
BOOL CGameController::AskAboutChanged = FALSE;
BOOL CGameController::BuyChanged = FALSE;

std::unordered_map<int, std::wstring> CGameController::_lSituations;
std::unordered_map<int, std::wstring> CGameController::_dSituations;

void CGameController::Init()
{
}

BOOL CGameController::StartGame(CGameBase* pGame)
{
	Game = pGame;

	// Play title animation
	Game->Start();

	return (Game != NULL);
}

BYTE CGameController::GetParameter(int index)
{
	return Game->GetParameter(index);
}

void CGameController::SetParameter(int index, BYTE value)
{
	Game->SetParameter(index, value);
}

BYTE CGameController::GetAskAboutState(int index)
{
	return Game->GetAskAboutState(index);
}

void CGameController::SetAskAboutState(int index, BYTE state)
{
	AskAboutChanged = TRUE;

	Game->SetAskAboutState(index, state);
}

BYTE CGameController::GetHintState(int index)
{
	return Game->GetHintState(index);
}

void CGameController::SetHintState(int index, BYTE state, int score)
{
	Game->SetHintState(index, state, score);
}

BYTE CGameController::GetHintCategoryState(int index)
{
	return Game->GetHintCategoryState(index);
}

void CGameController::SetHintCategoryState(int index, BYTE state)
{
	Game->SetHintCategoryState(index, state);
}

int CGameController::GetItemState(int item)
{
	return Game->GetItemState(item);
}

void CGameController::SetItemState(int item, int state)
{
	Game->SetItemState(item, state);
	ItemsChanged = TRUE;
}

void CGameController::SetItemState(int base, int item, int state)
{
	Game->SetItemState(base, item, state);
}

int CGameController::GetItemState(int base, int item)
{
	return Game->GetItemState(base, item);
}

void CGameController::SetFileName(int index, std::wstring name)
{
	FileMap[index] = name;
}

std::wstring CGameController::GetFileName(int index)
{
	return FileMap[index];
}

void CGameController::SetItemName(int index, std::wstring name)
{
	ItemMap[index] = name;
}

std::wstring CGameController::GetItemName(int index)
{
	return ItemMap[index];
}

void CGameController::SetBuyableItemName(int index, std::wstring name)
{
	BuyableItemMap[index] = name;
}

std::wstring CGameController::GetBuyableItemName(int index)
{
	return BuyableItemMap[index];
}

void CGameController::SetAskAboutName(int index, std::wstring name)
{
	AskAboutMap[index] = name;
}

std::wstring CGameController::GetAskAboutName(int index)
{
	return AskAboutMap[index];
}

void CGameController::SetTimer(int timer, int duration)
{
	Game->SetTimer(timer, duration);
}

int CGameController::GetTimerState(int timer)
{
	return Game->GetTimerState(timer);
}

void CGameController::Tick(int ticks)
{
	Game->Tick(ticks);
}

void CGameController::ResetTimers()
{
	Game->ResetTimers();
}

void CGameController::SetSituationDescriptionL(int ix, std::wstring value)
{
	_lSituations[ix] = value;
}

std::wstring CGameController::GetSituationDescriptionL(int ix)
{
	if (_lSituations.find(ix) == _lSituations.end())
	{
		ix = -1;
	}

	if (_lSituations.find(ix) != _lSituations.end())
	{
		std::wstring description = _lSituations[ix];
		if (description == L"")
		{
			description = _lSituations[-1];
		}

		return description;
	}

	return L"";
}

void CGameController::SetSituationDescriptionD(int ix, std::wstring value)
{
	_dSituations[ix] = value;
}

std::wstring CGameController::GetSituationDescriptionD(int ix)
{
	if (_dSituations.find(ix) == _dSituations.end())
	{
		ix = -1;
	}

	if (_dSituations.find(ix) != _dSituations.end())
	{
		std::wstring description= _dSituations[ix];
		if (description == L"")
		{
			description = _dSituations[-1];
		}

		return description;
	}

	return L"";
}

void CGameController::TransformItem(int fromId, int toId)
{
	SetItemState(fromId, 2);
	SetItemState(toId, 1);
}
