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

#ifndef TEX_DETECTION_H
#define TEX_DETECTION_H

#include "engines/advancedDetector.h"

namespace Tex {

enum TexGameType {
	kGameKillingMoon = 1,
	kGamePandoraDirective = 2,
	kGameOverseer = 3
};

struct TexGameDescription {
	AD_GAME_DESCRIPTION_HELPERS(desc);

	ADGameDescription desc;

	int gameID;
	uint32 features;
};

enum TexDebugChannels {
	kDebugGraphics = 1,
	kDebugPath,
	kDebugScan,
	kDebugFilePath,
	kDebugScript,
};

extern const PlainGameDescriptor texGames[];

extern const ADGameDescription TexDescriptions[];

#define GAMEOPTION_ORIGINAL_SAVELOAD GUIO_GAMEOPTIONS1

} // End of namespace Tex

class TexMetaEngineDetection : public AdvancedMetaEngineDetection<Tex::TexGameDescription> {
	static const DebugChannelDef debugFlagList[];

public:
	TexMetaEngineDetection();
	~TexMetaEngineDetection() override {}

	const char *getName() const override {
		return "tex";
	}

	const char *getEngineName() const override {
		return "Tex";
	}

	const char *getOriginalCopyright() const override {
		return "Tex (C)";
	}

	const DebugChannelDef *getDebugChannels() const override {
		return debugFlagList;
	}
};

#endif // TEX_DETECTION_H
