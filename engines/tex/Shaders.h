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
// #include "DXShader.h"

class CShaders
{
public:
	static void SelectOrthoShader();
	static void SelectTextureShader();
	static void SelectTexFontShader();
	static void SelectMultiColouredFontShader();
	static void SelectMultiColouredFontShaderPD();
	static void SelectColourShader();
	static void SelectTransparentColourShader();
	static void SelectYUVShader();
	static void SelectBasicShader();

	static void Dispose();

	static CDXShader* _orthoShader;
	static CDXShader* _textureShader;
	static CDXShader* _texFontShader;
	static CDXShader* _multiColouredFontShader;
	static CDXShader* _colourShader;
	static CDXShader* _transparentColourShader;
	static CDXShader* _texFontShader_AA;
	static CDXShader* _yuvShader;
	static CDXShader* _basicShader;
};
