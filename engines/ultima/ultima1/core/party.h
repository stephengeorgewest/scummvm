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

#ifndef ULTIMA_ULTIMA1_CORE_PARTY_H
#define ULTIMA_ULTIMA1_CORE_PARTY_H

#include "ultima/shared/core/party.h"

namespace Ultima {
namespace Ultima1 {

class Ultima1Game;

/**
 * Implements the data for a playable character within the game
 */
class Character : public Shared::Character {
private:
	Ultima1Game *_game;
public:
	/**
	 * Constructor
	 */
	Character(Ultima1Game *game) : Shared::Character(), _game(game) {}

	/**
	 * Gets the damage a given weapon does
	 */
	uint getWeaponDamage() const;
};

/**
 * Implements the party
 */
class Party : public Shared::Party {
public:
	/**
	 * Constructor
	 */
	Party(Ultima1Game *game);
};

} // End of namespace Ultima1
} // End of namespace Ultima

#endif
