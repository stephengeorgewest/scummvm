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

#ifndef SUPERNOVA_H
#define SUPERNOVA_H

#include "audio/audiostream.h"
#include "audio/mixer.h"
#include "audio/decoders/raw.h"
#include "common/array.h"
#include "common/random.h"
#include "common/scummsys.h"
#include "engines/engine.h"

#include "supernova/console.h"
#include "supernova/graphics.h"
#include "supernova/msn_def.h"

namespace Supernova {

struct ScreenBuffer {
	ScreenBuffer()
	    : _x(0)
	    , _y(0)
	    , _width(0)
	    , _height(0)
	    , _pitch(0)
	    , _pixels(0)
	{}

	byte *_pixels;
	int _x;
	int _y;
	int _width;
	int _height;
	int _pitch;
};
class ScreenBufferStack {
public:
	ScreenBufferStack();

	void push(int x, int y, int width, int height, int pitch = 320);
	void restore();

private:
	ScreenBuffer _buffer[8];
	ScreenBuffer *_last;
};

class SupernovaEngine : public Engine {
public:
	SupernovaEngine(OSystem *syst);
	~SupernovaEngine();

	virtual Common::Error run();

private:
	Common::RandomSource *_rnd;
	Console *_console;
	Audio::SoundHandle _soundHandle;
	bool _gameRunning;
	MSNImageDecoder _image;
	byte _imageIndex;
	byte _sectionIndex;
	byte _menuBrightness;
	byte _brightness;
	uint _delay;
	bool _messageDisplayed;

	uint getDOSTicks();
	void initData();
	void initPalette();
	void paletteFadeIn();
	void paletteFadeOut();
	void paletteBrightness();
	void updateEvents();
	void playSound(int filenumber, int offset = 0);
	void playSoundMod(int filenumber);
	void stopSound();
	void renderImage(int filenumber, int section, bool fullscreen = false);
	void renderMessage(const char *text, MessagePosition position = kMessageNormal);
	void removeMessage();
	void renderText(const char *text, int x, int y, byte color);
	void renderBox(int x, int y, int width, int height, byte color);
};


class Inventory {
public:
	Inventory();

	void add(Object &obj);
	void remove(Object &obj);
	Object *get(size_t index) const;
	Object *get(ObjectID id) const;

private:
	Object *_inventory[kMaxCarry];
	size_t _numObjects;
};

}

#endif
