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

#include "ModuleBase.h"
#include <unordered_map>
#include "D3D11-NoWarn.h"
#include "Texture.h"
#include "DXButton.h"

class CUAKMNewsPaperModule : public CModuleBase
{
public:
	CUAKMNewsPaperModule();
	virtual ~CUAKMNewsPaperModule();

	virtual void Resize(int width, int height);

	virtual void Dispose();
	virtual void Render();

	static CUAKMNewsPaperModule* pUAKMNPM;

protected:
	virtual void Initialize();

	int _display;
	int _highLight;

	float _left;
	float _right;
	float _top;
	float _bottom;
	float _scale;

	int _palette[256];

	CDXButton* _pBtnResume;
	static void OnResume(LPVOID data);

	class CNewsPaperView
	{
	public:
		CNewsPaperView()
		{
			Data = NULL;
			Buffer = NULL;
		}

		~CNewsPaperView()
		{
			if (Data != NULL)
			{
				delete[] Data;
				Data = NULL;
			}

			if (Buffer != NULL)
			{
				Buffer->Release();
				Buffer = NULL;
			}
		}

		LPBYTE Data = NULL;
		int Width = 0;
		int Height = 0;
		CTexture Texture;
		ID3D11Buffer* Buffer = NULL;
	};

	std::unordered_map<int, CNewsPaperView*> _newsPaper;

	void UpdateTexture(CNewsPaperView* np);

	// Input related
	virtual void Cursor(float x, float y, BOOL relative);
	virtual void BeginAction();
	virtual void Back();
};
