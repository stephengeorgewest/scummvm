/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "xeen/items.h"
#include "xeen/resources.h"

namespace Xeen {

XeenItem::XeenItem() {
	_material = _id = _bonusFlags = 0;
	_equipped = 0;
}

void XeenItem::synchronize(Common::Serializer &s) {
	s.syncAsByte(_material);
	s.syncAsByte(_id);
	s.syncAsByte(_bonusFlags);
	s.syncAsByte(_equipped);
}

int XeenItem::getElementalCategory() const {
	int idx;
	for (idx = 0; ELEMENTAL_CATEGORIES[idx] < _material; ++idx)
		;

	return idx;
}

int XeenItem::getAttributeCategory() const {
	int m = _material - 59;
	int idx;
	for (idx = 0; ATTRIBUTE_CATEGORIES[idx] < m; ++idx)
		;

	return idx;
}

Treasure::Treasure() {
	_hasItems = false;
	_v1 = false;
}

} // End of namespace Xeen
