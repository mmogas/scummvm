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

// #include "DirectX.h"
// #include "D3D11-NoWarn.h"
// #include <D3DX10math.h>
#include "ShaderStructs.h"

class CConstantBuffers
{
public:
	static void SetVOP(CDirectX& dx, XMMATRIX* view, XMMATRIX* ortho, XMMATRIX* projection);
	static void SetWorld(CDirectX& dx, XMMATRIX* world);
	static void SetTexFont(CDirectX& dx, XMVECTOR* colour1, XMVECTOR* colour2, XMVECTOR* colour3, XMVECTOR* colour4);
	static void SetVisibility(CDirectX& dx, VisibilityBufferType visibility);
	static void SetTranslation(CDirectX& dx, TranslationBufferType translation);
	static void SetMultiColouredFont(CDirectX& dx, XMVECTOR* colour1, XMVECTOR* colour2, XMVECTOR* colour3, XMVECTOR* colour4, XMVECTOR* colour5, XMVECTOR* colour6);

	static void Setup2D(CDirectX& dx);
	static void Setup3D(CDirectX& dx);

	static void Dispose();

	static ID3D11Buffer* _vop;
	static ID3D11Buffer* _world;
	static ID3D11Buffer* _texFont;
	static ID3D11Buffer* _multiColouredFont;

	static ID3D11Buffer* _visibility;
	static ID3D11Buffer* _translation;

private:
	static void SetupVOP(CDirectX& pDX, float w, float h, float camera_x, float camera_y, float camera_z);
};
