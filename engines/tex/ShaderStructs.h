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

// #include "D3DX11-NoWarn.h"
// #include <DirectXPackedVector.h>

using namespace DirectX::PackedVector;
using namespace DirectX;

struct TEXTURED_VERTEX_ORTHO
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct TEXTURED_VERTEX
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT2 object;			// Use for visibility, then create a visibility buffer for the objects and lookup in the shader
	XMFLOAT4 objectParameters;	// Use for triangle transparency indicator in shader (some textures are used both as opaque and transparent)
};

struct MULTICOLOURED_FONT_VERTEX
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT4 colour;
};

struct COLOURED_VERTEX
{
	XMFLOAT4 position;
	XMFLOAT4 colour;
	XMFLOAT4 object;
};

struct COLOURED_VERTEX_ORTHO
{
	XMFLOAT4 position;
	XMFLOAT4 colour;
};

struct VOPBufferType
{
	XMMATRIX view;
	XMMATRIX ortho;
	XMMATRIX projection;
};

struct WorldBufferType
{
	XMMATRIX world;
};

struct MultiColouredFontBufferType
{
	XMVECTOR colour1;
	XMVECTOR colour2;
	XMVECTOR colour3;
	XMVECTOR colour4;
	XMVECTOR colour5;
	XMVECTOR colour6;
};

struct TexFontBufferType
{
	XMVECTOR colour1;
	XMVECTOR colour2;
	XMVECTOR colour3;
	XMVECTOR colour4;
};

struct VisibilityBufferType
{
	XMFLOAT4 visibility[4096];
};

struct TranslationBufferType
{
	XMFLOAT4 translation[256];
};
