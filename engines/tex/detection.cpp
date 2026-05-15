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

#include "base/plugins.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/md5.h"
#include "common/str-array.h"
#include "common/translation.h"
#include "common/util.h"
#include "tex/detection.h"
#include "tex/detection_tables.h"
#include "tex/tex.h"

static const PlainGameDescriptor TexGames[] = {
	{"uakm", "Under a Killing Moon"},
	{"pandora", "Pandora Directive"},
        {"overseer", "Tex Murphy: Overseer"},
	{nullptr, nullptr}
};

TexMetaEngineDetection::TexMetaEngineDetection()
	: AdvancedMetaEngineDetection(Tex::gameDescriptions, TexGames) {
}

const DebugChannelDef TexMetaEngineDetection::debugFlagList[] = {
	{ Tex::kDebugGraphics, "Graphics", "Graphics debug level" },
	{ Tex::kDebugPath, "Path", "Pathfinding debug level" },
	{ Tex::kDebugFilePath, "FilePath", "File path debug level" },
	{ Tex::kDebugScan, "Scan", "Scan for unrecognised games" },
	{ Tex::kDebugScript, "Script", "Enable debug script dump" },
	DEBUG_CHANNEL_END
};





REGISTER_PLUGIN_STATIC(TEX_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, TexMetaEngineDetection);
