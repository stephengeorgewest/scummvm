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

#include "m4/burger/rooms/section3/mine.h"
#include "m4/burger/vars.h"

namespace M4 {
namespace Burger {
namespace Rooms {

#define MINE_END 39

const char *Mine::SAID[][4] = {
	{ "TUNNEL",  "311w007", "311w007z", nullptr    },
	{ "DEBRIS",  nullptr,   "311w010",  "311w011"  },
	{ "GROUND",  "311w012", "311w007z", "311w007z" },
	{ "WALL",    "311w012", "311w007z", "311w007z" },
	{ "CEILING", "311w012", "311w007z", "311w007z" },
	{ nullptr, nullptr, nullptr, nullptr }
};

const int16 Mine::MINE_SCENE_NUMBERS[] = {
	305, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319
};

const MineRoom Mine::MINE_INFO[] = {
  //                            Links                      Doors
  //                    ---------------------   --------------------------   Correct
  // Room#  Scene ID    Back Front Left Right   Back   Front  Left   Right    Link    Check  
  // -----  ----------  ---- ----- ---- -----   -----  -----  -----  -----   -------  -----
       0,   SCENE_305,   6,   -1,   -1,  -1,    FRONT, NONE,  NONE,  NONE,   BACK,      0,  // mine entrance
       1,   SCENE_313,  -1,   -1,    2,   6,    NONE,  NONE,  RIGHT, LEFT,   RIGHT,     0,       
       2,   SCENE_316,  -1,   11,   13,   1,    NONE,  BACK,  RIGHT, LEFT,   LEFT,      0,       
       3,   SCENE_318,   9,   14,    5,   4,    FRONT, BACK,  RIGHT, LEFT,   FRONT,     0,    
       4,   SCENE_315,   5,   -1,    3,  10,    FRONT, NONE,  RIGHT, LEFT,   LEFT,      0,       
       5,   SCENE_317,  18,    4,    9,   3,    FRONT, BACK,  RIGHT, LEFT,   RIGHT,     0,       
       6,   SCENE_318,  11,    0,    1,   7,    FRONT, BACK,  RIGHT, LEFT,   RIGHT,     0,       
       7,   SCENE_317,  13,   14,    6,  18,    FRONT, RIGHT, RIGHT, LEFT,   FRONT,     0,       
       8,   SCENE_313,  -1,   -1,   10,   9,    NONE,  NONE,  RIGHT, LEFT,   RIGHT,     0,       
       9,   SCENE_318,  10,    3,    8,   5,    FRONT, BACK,  RIGHT, LEFT,   FRONT,     0,
  // -----  ----------  ---- ----- ---- -----   -----  -----  -----  -----   ----     -----
      10,   SCENE_316,  -1,    9,    4,   8,    NONE,  BACK,  RIGHT, LEFT,   LEFT,      0,       
      11,   SCENE_317,   2,    6,   11,  11,    FRONT, BACK,  RIGHT, LEFT,   FRONT,     0,       
      12,   SCENE_319,  -1,   -1,   -1,  13,    NONE,  NONE,  NONE,  LEFT,   RIGHT,     0,       
      13,   SCENE_316,  -1,    7,   12,   2,    NONE,  BACK,  RIGHT, LEFT,   FRONT,     0,       
      14,   SCENE_315,   3,   -1,   26,   7,    FRONT, NONE,  BACK,  FRONT,  LEFT,      0,       
      15,   SCENE_317,  17,   16,   17,  20,    FRONT, BACK,  RIGHT, LEFT,   RIGHT,     0,       
      16,   SCENE_311,  15,   -1,   -1,  -1,    FRONT, NONE,  NONE,  NONE,   BACK,      0,       
      17,   SCENE_318,  22,   15,   20,  15,    FRONT, BACK,  RIGHT, LEFT,   LEFT,      0,       
      18,   SCENE_316,  -1,    5,    7,  19,    NONE,  BACK,  RIGHT, LEFT,   LEFT,      0,       
      19,   SCENE_312,  -1,   -1,   18,  -1,    NONE,  NONE,  RIGHT, NONE,   LEFT,      0,       
  // -----  ----------  ---- ----- ---- -----   -----  -----  ------ -----   ----     -----
      20,   SCENE_315,  26,   -1,   15,  17,    FRONT, NONE,  RIGHT, LEFT,   BACK,      0,       
      21,   SCENE_319,  -1,   -1,   -1,  22,    NONE,  NONE,  NONE,  LEFT,   RIGHT,     0,       
      22,   SCENE_316,  -1,   17,   21,  23,    NONE,  BACK,  RIGHT, LEFT,   FRONT,     0,       
      23,   SCENE_312,  -1,   -1,   22,  -1,    NONE,  NONE,  RIGHT, NONE,   LEFT,      0,       
      24,   SCENE_318,  35,   29,   33,  25,    FRONT, BACK,  RIGHT, LEFT,   RIGHT,     0,       
      25,   SCENE_314,  -1,   -1,   24,  26,    NONE,  NONE,  RIGHT, LEFT,   RIGHT,     0,       
      26,   SCENE_317,  14,   20,   25,  38,    LEFT,  BACK,  RIGHT, LEFT,   RIGHT,     0,       
      27,   SCENE_318,  38,   38,   37,  28,    FRONT, BACK,  RIGHT, LEFT,   BACK,      0,       
      28,   SCENE_312,  -1,   -1,   27,  -1,    NONE,  NONE,  RIGHT, NONE,   LEFT,      0,       
      29,   SCENE_317,  24,   30,   35,  31,    FRONT, BACK,  RIGHT, LEFT,   BACK,      0,       
  // -----  ----------  ---- ----- ---- -----   -----  -----  ------ -----   ----     -----
      30,   SCENE_311,  29,   -1,   -1,  -1,    FRONT, NONE,  NONE,  NONE,   BACK,      0,       
      31,   SCENE_312,  -1,   -1,   29,  -1,    NONE,  NONE,  RIGHT, NONE,   LEFT,      0,       
      32,   SCENE_319,  -1,   -1,   -1,  33,    NONE,  NONE,  NONE,  LEFT,   RIGHT,     0,       
      33,   SCENE_314,  -1,   -1,   32,  24,    NONE,  NONE,  RIGHT, LEFT,   RIGHT,     0,       
      34,   SCENE_319,  -1,   -1,   -1,  35,    NONE,  NONE,  NONE,  LEFT,   RIGHT,     0,       
      35,   SCENE_316,  -1,   24,   34,  29,    NONE,  BACK,  RIGHT, LEFT,   FRONT,     0,       
      36,   SCENE_319,  -1,   -1,   -1,  37,    NONE,  NONE,  NONE,  LEFT,   RIGHT,     0,       
      37,   SCENE_314,  -1,   -1,   36,  27,    NONE,  NONE,  RIGHT, LEFT,   RIGHT,     0,       
      38,   SCENE_318,  27,   27,   26,  39,    FRONT, BACK,  RIGHT, LEFT,   RIGHT,     0,       
      39,   SCENE_310,  -1,   -1,   38,  -1,    NONE,  NONE,  RIGHT, NONE,   NONE,      0, // treasure      
  // -----  ----------  ---- ----- ---- -----   -----  -----  -----  -----   -------  -----
      40,   NO_SCENE,   -1,   -1,   -1,  -1,    NONE,  NONE,  NONE,  NONE,   NONE,	    0  
};																					 

/**
 * Coords for starting fade down for exits
 */
Rectangle Mine::FADE_DOWN_INFO[MAX_SCENE_TYPES][4] = {
	//
	// ( x1, y1, x2, y2) for four directions
	//
	//  	   Back		  		 	 Front	     		 	Left 		   	       Right 
	//-------------------    -------------------    -------------------    -------------------
	  255,   0, 315, 250,      0,   0,   0,   0,      0,   0,   0,   0,      0,   0,   0,   0,  // 305
		0,   0,   0,   0,      0,   0,   0,   0,      0,   0, 170, 270,      0,   0,   0,   0,  // 310
	  270,   0, 365, 255,      0,   0,   0,   0,      0,   0,   0,   0,      0,   0,   0,   0,  // 311
		0,   0,   0,   0,      0,   0,   0,   0,      0,   0, 175, 268,      0,   0,   0,   0,  // 312
		0,   0,   0,   0,      0,   0,   0,   0,      0,   0, 172, 256,    470,   0, 639, 265,  // 313
		0,   0,   0,   0,      0,   0,   0,   0,      0,   0, 172, 265,    470,   0, 639, 280,  // 314
	  280,   0, 365, 245,      0,   0,   0,   0,      0,   0, 165, 257,    470,   0, 639, 271,  // 315
		0,   0,   0,   0,    325, 350, 500, 374,      0,   0, 178, 265,    470,   0, 639, 265,  // 316
	  275,   0, 365, 255,    136, 344, 360, 374,      0,   0, 165, 268,    470,   0, 639, 270,  // 317
	  275,   0, 365, 252,    185, 344, 500, 374,      0,   0, 165, 265,    470,   0, 639, 270,  // 318
		0,   0,   0,   0,      0,   0,   0,   0,      0,   0,   0,   0,    470,   0, 639, 270   // 319
};


void Mine::preload() {
	Section3Room::preload();
	_mineCtr = 0;
}

void Mine::daemon() {
	switch (_G(kernel).trigger) {
	case 304:
		digi_play("300_007", 2);
		break;

	// TODO: Further cases
	default:
		_G(kernel).continue_handling_trigger = true;
		break;
	}
}

void Mine::pre_parser() {
	_G(kernel).trigger_mode = KT_DAEMON;

	if (player_said("tunnel") && player_said_any("walk through", "GEAR")) {
		if (_G(click_y) > 300)
			set_fade_down_rect(FRONT);
		else if (_G(click_x) < 200)
			set_fade_down_rect(LEFT);
		else if (_G(click_x) < 400)
			set_fade_down_rect(BACK);
		else
			set_fade_down_rect(RIGHT);

		// Turn on the fade down area
		_fade_down_rect_active = true;
	}
}

void Mine::parser() {
	_G(kernel).trigger_mode = KT_DAEMON;

	if (player_said("LOOK AT") && player_said_any("WALL", "CEILING", "GROUND")) {
		term_message("Room #: %d", _G(flags)[V149]);
		term_message("Distance from pig: %d", getTreasureDistance());

		_mineCtr = (_mineCtr + 1) % 5;

		if (_mineCtr == 0) {
			wilbur_speech("311w012");
			_G(player).command_ready = false;
			return;
		}
	}

	if (_G(walker).wilbur_said(SAID)) {
		// Already handled
	} else if (player_said("tunnel") && player_said_any("walk through", "GEAR")) {
		pal_fade_set_start(0);

		if (_G(click_y) > 300)
			mine_travel_link(FRONT);
		else if (_G(click_x) < 200)
			mine_travel_link(LEFT);
		else if (_G(click_x) > 400)
			mine_travel_link(RIGHT);
		else
			mine_travel_link(BACK);
	} else if (player_said("LOOK AT", "DEBRIS") && _G(game).room_id != 305) {
		_G(wilbur_should) = 407;
		kernel_trigger_dispatch_now(gCHANGE_WILBUR_ANIMATION);
	} else {
		return;
	}

	_G(player).command_ready = false;
}

int Mine::getTreasureDistance() const {
	int distance = 0;

	for (int index = _G(flags)[V149]; index != MINE_END; ++distance) {
		const MineRoom &me = MINE_INFO[index];
		index = me.link[me.correctLink];
	}

	return distance;
}

void Mine::mine_travel_link(int16 takeLink) {
	_mineRoomInfo = MINE_INFO[_mineRoomIndex];		// Get this mine room info
	_entranceDoor = _mineRoomInfo.door[takeLink];	// Get which door to enter from in new room
	_mineRoomIndex = _mineRoomInfo.link[takeLink];	// Get which link to take

	_mineRoomInfo = MINE_INFO[_mineRoomIndex];		// Get new mine room info from new index
	_presentSceneID = _mineRoomInfo.scene_id;		// Set the scene ID
	_G(game).new_room = MINE_SCENE_NUMBERS[_presentSceneID]; // Go to the corresponding scene number
	_G(kernel).force_restart = true; 				// Makes the scene start over even if new_room = present room
}

void Mine::set_fade_down_rect(MineDoors exit_door) {
	_fade_down_rect.x1 = FADE_DOWN_INFO[_presentSceneID][exit_door].x1;
	_fade_down_rect.y1 = FADE_DOWN_INFO[_presentSceneID][exit_door].y1;
	_fade_down_rect.x2 = FADE_DOWN_INFO[_presentSceneID][exit_door].x2;
	_fade_down_rect.y2 = FADE_DOWN_INFO[_presentSceneID][exit_door].y2;
}


} // namespace Rooms
} // namespace Burger
} // namespace M4
