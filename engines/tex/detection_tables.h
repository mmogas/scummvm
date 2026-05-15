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


namespace Tex {

static const TexGameDescription gameDescriptions[] = {

	{
		// Under a Killing Moon Original CD EN
		// 4 x CDROM

		{
			"uakm",
			nullptr,
                        {
				{ "CFG_SND.AP", 0, "82afc150583a8d0516d3ecbc4b8bd49b", 92604 },
				{ "EXAM.LZ", 0, "d0d6c43ebf3e6e64d9e5652f09a67cec", 8242 },
				{ "GRAPHICS.AP", 0, "d2c553fd8ecbecc2161e0dac926a703a", 411228 },
				{ "HELP.AP", 0, "8af29373b7700d7e0a77b7b0956d811e", 32851 },
				{ "HELP.LZ", 0, "22deedf0293677b5b679d259bf74e9d8", 30912 },
				{ "HINT.AP", 0, "2fc5dd6a262375fa33c2d742ede7c6bf", 45416 },
				{ "ICONS.LZ", 0, "d7f5d6ccaf8a24bd8d983810e62c050c", 36185 },
				{ "LSGRPH.AP", 0, "c6132430d02ae4c082d93e162e1b7e13", 77074 },
				{ "PANEL.AP", 0, "965de7c0ba8c108536104991751f4e8b", 608518 },
				{ "ROOMTEXT.AP", 0, "ac0c8f7b54f7c4790700fea85a060523", 113943 },
				{ "TRAVEL.AP", 0, "8ec4c8ee30165214ae8ceb98a312e448", 759951 },
				AD_LISTEND
			},
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGameKillingMoon,
		0
	},

	{
		// Under a Killing Moon Original CD ES by DRO-SOFT
		// 4 x CDROM
		{
			"uakm",
			nullptr,
                        {
				{ "CFG_SND.AP", 0, "2176cec85c163bea7aefdb6eeb022071", 96651 },
				{ "EXAM.LZ", 0, "96ee713f89dc287cad145ae1c2378b74", 8707 },
				{ "GRAPHICS.AP", 0, "4578c76d40fae12d79cfefcc11ea3921", 415071 },
				{ "HELP.AP", 0, "01c7b936b3e7c3a3732ecb7d996a4415", 37165 },
				{ "HELP.LZ", 0, "81c863f0a8296cf421d23a287f69e082", 30587 },
				{ "HINT.AP", 0, "4f6d10cb5c91bd82117e18c2cf1445ef", 47832 },
				{ "ICONS.LZ", 0, "8f35638c6d36c25bac9c16ed8540b27f", 36195 },
				{ "LSGRPH.AP", 0, "a0808a30ffa4659a39faf7d0bc4eda3b", 78583 },
				{ "PANEL.AP", 0, "40e993a43ae5aa779c206c51bfe455b1", 883263 },
				{ "ROOMTEXT.AP", 0, "f53f84d57454d0cb067cfeed432ec925", 115928 },
				{ "TRAVEL.AP", 0, "7f519775950e22a2bb9c8037beb91be2", 760973 },
				AD_LISTEND
			},
			Common::ES_ESP,
			Common::kPlatformDOS,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGameKillingMoon,
		0
	},

	{
		// Under a Killing Moon Original CD DE (German)
		// 4 x CDROM
		{
			"uakm",
			nullptr,
                        {
 			 	{"CFG_SND.AP", 0, "abed4594afa3ed674cb4d8e98277023d", 103082},
  				{"EXAM.LZ", 0, "93556c9cfe3516e00028ab0fe2d518c8", 9656},
 				{"GRAPHICS.AP", 0, "7bf1db77a8fa2a09e0082a6e094362e0", 408472},
  				{"HELP.AP", 0, "e759e8a73bf6c49ed2448fd4a3acb621", 33714},
  				{"HELP.LZ", 0, "92f6aaf6cc1c77c2924c82cca83ee82d", 29662},
  				{"HINT.AP", 0, "f816b1fee763744dd5a29cc6e6d0fe11", 50330},
  				{"ICONS.LZ", 0, "ce2d95c1d357eab0eb29854a92ef9919", 36281},
  				{"LSGRPH.AP", 0, "e3eaeea23caa4657ea491b639e42f1e0", 79222},
  				{"PANEL.AP", 0, "b55652cf50f5f22d26840884d9e6d92a", 656347},
  				{"ROOMTEXT.AP", 0, "fed99b95a82025454fd02871f0425ad0", 127933},
  				{"TRAVEL.AP", 0, "b4adb29fef3cac55b21b5f5c42f2b7a6", 760610},
				AD_LISTEND
			},
			Common::DE_DEU,
			Common::kPlatformDOS,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGameKillingMoon,
		0
	},

	{
		// Pandora Directive Original CD EN
		// 6 x CDROM

		{
			"pandora",
			nullptr,
                        {
  				{"CHARCODE.AP", 0, "ac010da73b1aae969fa12f2d4cdae96f", 330784},
  				{"DMAP.LZ", 0, "b69b4225356bec5920257e7eeeee874c", 6773},
  				{"EXAM.LZ", 0, "1496d1d0032272f5b12c5ed0cd72c3d1", 13079},
  				{"GRAPHICS.AP", 0, "37906fc8fe3e718fe04dc6688af671dd", 1869753},
  				{"HINT.AP", 0, "19c73e9a5335f98e3814a66f247cc088", 263483},
  				{"HLPINDEX.IDX", 0, "ff6a487749a884de6c48e31bf6c0d61b", 1860},
  				{"ICONS.AP", 0, "ee72a55511a2017b1c23aacaa854b293", 107228},
  				{"RMAP.AP", 0, "9b668ce4afe3f6e05720dbb69c616f41", 17202},
  				{"ROOMCODE.AP", 0, "f064752f1c6323589e6d24f45d5e7dc9", 215272},
			},
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGamePandoraDirective,
		0
	},

	{
		// Pandora Directive CD ES Translation
		// 6 x CDROM

		{
			"pandora",
			nullptr,
                        {
  				{"CHARCODE.AP", 0, "2997304e9630a110e427b570167b6d82", 331654},
  				{"DMAP.LZ", 0, "b69b4225356bec5920257e7eeeee874c", 6773},
  				{"EXAM.LZ", 0, "bf35869d392013d3c224afc68bdcc0c0", 26433},
  				{"GRAPHICS.AP", 0, "bce89a0ea233c8b6420a6b72b8a44d26", 1867431},
  				{"HINT.AP", 0, "19c73e9a5335f98e3814a66f247cc088", 263483},
  				{"HLPINDEX.IDX", 0, "70af19b89e18d41f9ff1d931ff478c97", 1860},
  				{"ICONS.AP", 0, "ee72a55511a2017b1c23aacaa854b293", 107228},
  				{"RMAP.AP", 0, "9b668ce4afe3f6e05720dbb69c616f41", 17202},
  				{"ROOMCODE.AP", 0, "df3a7f652de001fc43aebc4a9e189601", 218513},
			},
			Common::ES_ESP,
			Common::kPlatformDOS,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGamePandoraDirective,
		0
	},

	{
		// Tex Murphy Overseee CD Original EN
		// 5 x CDROM

		{
			"overseer",
			nullptr,
                        {
  				{"CHAR.LS", 0, "be75ff657d3bfa093fcde90ab5e5797f", 144573},
  				{"DeIsL1.isu", 0, "af13e31982ff7fa00af806effb7ff1d8", 3808},
  				{"EXAM.DAT", 0, "0a0ee54c16973345a61726b6c9db8aaa", 5048},
  				{"FONTS.LS", 0, "eaaf98500c71de1c1b6f1719afbdf93e", 50287},
  				{"INV.LS", 0, "2cdcaf316a5093e760dd82fa43f60485", 528917},
  				{"JIGSAW.LS", 0, "c4db8439e4740eec13eab6cea2a019a8", 2436018},
  				{"PUZZLE.LS", 0, "2db685e0927f3709e1a7455502006e0b", 2680098},
  				{"RMAP.LS", 0, "2c555c3824d820091fb1314125ef3335", 16204},
  				{"ROOM.LS", 0, "f56c059c27b7ea07a0acf67550393825", 68696},
  				{"SPECIAL.LS", 0, "fbadb946fd54dff9176a64a06892b2e4", 2518369},
  				{"STILLS.LS", 0, "93d8ea86454475551f38a412ceedb1a7", 3226556},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGameOverseer,
		0
	},

	{
		// Tex Murphy Overseer DVD Original EN
		// 5 x CDROM

		{
			"overseer",
			nullptr,
                        {
  				{"CHAR.LS", 0, "be75ff657d3bfa093fcde90ab5e5797f", 144573},
  				{"DeIsL1.isu", 0, "f705463b70b1fa4f755db049ac277c48", 3713},
  				{"EXAM.DAT", 0, "0a0ee54c16973345a61726b6c9db8aaa", 5048},
  				{"FONTS.LS", 0, "eaaf98500c71de1c1b6f1719afbdf93e", 50287},
  				{"INV.LS", 0, "2cdcaf316a5093e760dd82fa43f60485", 528917},
  				{"JIGSAW.LS", 0, "c4db8439e4740eec13eab6cea2a019a8", 2436018},
  				{"PUZZLE.LS", 0, "2db685e0927f3709e1a7455502006e0b", 2680098},
  				{"RMAP.LS", 0, "2c555c3824d820091fb1314125ef3335", 16204},
  				{"ROOM.LS", 0, "f56c059c27b7ea07a0acf67550393825", 68696},
  				{"SPECIAL.LS", 0, "fbadb946fd54dff9176a64a06892b2e4", 2518369},
  				{"STILLS.LS", 0, "93d8ea86454475551f38a412ceedb1a7", 3226556},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_DVD,
			GUIO1(GUIO_NONE)
		},
		kGameOverseer,
		0
	},

	{
		// Tex Murphy Overseer CD ES Translation 
		// 5 x CDROM

		{
			"overseer",
			nullptr,
                        {
  				{"CHAR.LS", 0, "7a3c66c6932c4348ceff110169650d44", 144562},
  				{"DeIsL1.isu", 0, "7b0515a307cbf128637fbf4766be3ad7", 3576},
  				{"EXAM.DAT", 0, "24c65c880fe20e3209244c21cab64bd2", 5048},
  				{"FONTS.LS", 0, "3b29a10c1413f851df445e801b3af49d", 50341},
  				{"INV.LS", 0, "91a54725af43bf2c6f17966187de6077", 529201},
  				{"JIGSAW.LS", 0, "56e66b2be83562cac72c42408d01a88d", 2708750},
  				{"PUZZLE.LS", 0, "283b187d4e33fdbc5890885033659683", 3426328},
  				{"RMAP.LS", 0, "5bd3a7cd24d1f744221a5efa37847b1b", 16338},
  				{"ROOM.LS", 0, "a6e27d8811444e3ed4b61e72393ba24c", 68595},
  				{"SPECIAL.LS", 0, "be6b65afe9a771a6e6e4f8056698b7a4", 2733303},
  				{"STILLS.LS", 0, "c2347ed7b73f88ccf1f985eb5ef6604e", 324795},

			},
			Common::ES_ESP,
			Common::kPlatformWindows,
			ADGF_CD,
			GUIO1(GUIO_NONE)
		},
		kGameOverseer,
		0
	},


	{
		// Tex Murphy Overseer DVD ES Translation 
		// 5 x CDROM

		{
			"overseer",
			nullptr,
                        {
  				{"CHAR.LS", 0, "7a3c66c6932c4348ceff110169650d44", 144562},
  				{"DeIsL1.isu", 0, "7ab020c4e50d85b2ad55c13dee93deb5", 2854},
  				{"EXAM.DAT", 0, "24c65c880fe20e3209244c21cab64bd2", 5048},
  				{"FONTS.LS", 0, "3b29a10c1413f851df445e801b3af49d", 50341},
  				{"INV.LS", 0, "2cdcaf316a5093e760dd82fa43f60485", 528917},
  				{"JIGSAW.LS", 0, "56e66b2be83562cac72c42408d01a88d", 2708750},
  				{"PUZZLE.LS", 0, "283b187d4e33fdbc5890885033659683", 3426328},
  				{"RMAP.LS", 0, "5bd3a7cd24d1f744221a5efa37847b1b", 16338},
  				{"ROOM.LS", 0, "a6e27d8811444e3ed4b61e72393ba24c", 68595},
  				{"SPECIAL.LS", 0, "be6b65afe9a771a6e6e4f8056698b7a4", 2733303},
  				{"STILLS.LS", 0, "c2347ed7b73f88ccf1f985eb5ef6604e", 324795},
			},
			Common::ES_ESP,
			Common::kPlatformWindows,
			ADGF_DVD,
			GUIO1(GUIO_NONE)
		},
		kGameOverseer,
		0
	},

	{ AD_TABLE_END_MARKER, 0, 0 }

} ;

} // End of namespace Tex
