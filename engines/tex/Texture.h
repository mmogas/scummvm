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


// #include "DXBase.h" */
// #include "DirectX.h" */

class CTexture : public CDXBase
{
public:
	CTexture();
	CTexture(CDirectX* pDX, int w, int h, PBYTE input, PINT palette, int rotate);

	~CTexture();

	BOOL Init(int width, int height, D3D11_USAGE usageFlags = D3D11_USAGE_DYNAMIC, DWORD miscFlags = 0, ID3D11Device* pD3D = NULL);
	BOOL Init(PCWSTR file);
	BOOL Init(PBYTE pImage, DWORD size, char* name);
	BOOL Init(PBYTE pImage, DWORD size, DWORD offset, PINT pPalette, int transparentIndex, char* name, int sx = 0, int sy = 0, int sw = -1, int sh = -1, bool rawImage = false, int rawWidth = 0, int rawHeight = 0);
	void Dispose();

	ID3D11Texture2D* GetTexture() { return _texture; }
	ID3D11ShaderResourceView* GetTextureRV() { return _textureRV; }

	int Width() { return _width; }
	int Height() { return _height; }

protected:
	int _width;
	int _height;

	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureRV;
};
