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

#ifndef VCRUISE_AD2044_ITEMS_H
#define VCRUISE_AD2044_ITEMS_H

#include "common/scummsys.h"

namespace VCruise {

struct AD2044ItemInfo {
	uint32 enNameCRC;
	uint32 plNameCRC;
	uint16 inspectionScreenID;
	uint16 scriptItemID;
};

static const uint kNumAD2044Items = 74;

extern const AD2044ItemInfo g_ad2044ItemInfos[kNumAD2044Items];

} // End of namespace VCruise


#endif
