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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "startrek/common.h"
#include "startrek/graphics.h"

#include "common/config-manager.h"
#include "common/events.h"
#include "common/rendermode.h"
#include "graphics/cursorman.h"
#include "graphics/palette.h"
#include "graphics/surface.h"

namespace StarTrek {

Graphics::Graphics(StarTrekEngine *vm) : _vm(vm), _egaMode(false) {
	_font = nullptr;
	_egaData = nullptr;
	_lutData = nullptr;

	_screenRect = Common::Rect(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (ConfMan.hasKey("render_mode"))
		_egaMode = (Common::parseRenderMode(ConfMan.get("render_mode").c_str()) == Common::kRenderEGA) && (_vm->getGameType() != GType_STJR) && !(_vm->getFeatures() & GF_DEMO);

	if (_vm->getGameType() == GType_ST25 && _vm->getPlatform() == Common::kPlatformDOS)
		_font = new Font(_vm);

	_numSprites = 0;
	_textDisplayMode = TEXTDISPLAY_WAIT;
	_textboxVar2 = 0;
	_textboxVar6 = 0;
	_textboxHasMultipleChoices = false;

	_palData = new byte[256 * 3];
	_lutData = new byte[256 * 3];

	_paletteFadeLevel = 0;

	setMouseCursor(loadBitmap("pushbtn"));
	CursorMan.showMouse(true);
}

Graphics::~Graphics() {
	delete[] _egaData;

	delete _font;
}


void Graphics::setBackgroundImage(SharedPtr<Bitmap> bitmap) {
	_backgroundImage = bitmap;
}

/**
 * Note: this doesn't flush the palette to the screen (must call "setPaletteFadeLevel")
 */
void Graphics::loadPalette(const Common::String &paletteName) {
	// Set the palette from a PAL file
	Common::String palFile = paletteName + ".PAL";
	Common::String lutFile = paletteName + ".LUT";

	SharedPtr<Common::SeekableReadStream> palStream = _vm->loadFile(palFile.c_str());
	palStream->read(_palData, 256 * 3);

	// Load LUT file
	SharedPtr<Common::SeekableReadStream> lutStream = _vm->loadFile(lutFile.c_str());

	lutStream->read(_lutData, 256);
}

void Graphics::fadeinScreen() {
	while (_paletteFadeLevel <= 100) {
		TrekEvent event;
		do {
			_vm->popNextEvent(&event);
		}
		while (event.type != TREKEVENT_TICK);

		setPaletteFadeLevel(_palData, _paletteFadeLevel);
		_paletteFadeLevel += 10;
	}

	_paletteFadeLevel = 100;
}

void Graphics::fadeoutScreen() {
	while (_paletteFadeLevel >= 0) {
		TrekEvent event;
		do {
			_vm->popNextEvent(&event);
		}
		while (event.type != TREKEVENT_TICK);

		setPaletteFadeLevel(_palData, _paletteFadeLevel);
		_paletteFadeLevel -= 10;
	}

	_paletteFadeLevel = 0;
}

/**
 * This flushes the palette to the screen. fadeLevel ranges from 0-100.
 */
void Graphics::setPaletteFadeLevel(byte *palData, int fadeLevel) {
	byte palBuffer[256 * 3];

	int multiplier = (fadeLevel << 8) / 100;

	for (uint16 i = 0; i < 256 * 3; i++) {
		palBuffer[i] = (palData[i] * multiplier) >> 8;

		// Expand color components
		if (_vm->getPlatform() == Common::kPlatformDOS || _vm->getPlatform() == Common::kPlatformMacintosh)
			palBuffer[i] <<= 2;
	}

	_vm->_system->getPaletteManager()->setPalette(palBuffer, 0, 256);

	// FIXME: this isn't supposed to flush changes to graphics, only palettes.
	// Might not matter...
	_vm->_system->updateScreen();
}

void Graphics::incPaletteFadeLevel() {
	if (_paletteFadeLevel < 100) {
		_paletteFadeLevel += 10;
		setPaletteFadeLevel(_palData, _paletteFadeLevel);
	}
}

void Graphics::decPaletteFadeLevel() {
	if (_paletteFadeLevel > 0) {
		_paletteFadeLevel -= 10;
		setPaletteFadeLevel(_palData, _paletteFadeLevel);
	}
}


void Graphics::loadPri(const Common::String &priFile) {
	SharedPtr<Common::SeekableReadStream> priStream = _vm->loadFile(priFile);
	priStream->read(_priData, SCREEN_WIDTH * SCREEN_HEIGHT / 2);
}

void Graphics::clearPri() {
	memset(_priData, 0, sizeof(_priData));
}

byte Graphics::getPriValue(int x, int y) {
	assert(_screenRect.contains(x, y));

	int priOffset = y * SCREEN_WIDTH + x;
	byte b = _priData[priOffset / 2];
	if ((priOffset % 2) == 1)
		return b & 0xf;
	else
		return b >> 4;
}

SharedPtr<Bitmap> Graphics::loadBitmap(Common::String basename) {
	return SharedPtr<Bitmap>(new Bitmap(_vm->loadFile(basename + ".BMP")));
}

Common::Point Graphics::getMousePos() {
	return _vm->_system->getEventManager()->getMousePos();
}

void Graphics::setMouseCursor(SharedPtr<Bitmap> bitmap) {
	_mouseBitmap = bitmap;
	_vm->_system->setMouseCursor(bitmap->pixels, bitmap->width, bitmap->height, bitmap->xoffset, bitmap->yoffset, 0);
}

void Graphics::drawSprite(const Sprite &sprite) {
	int left = sprite.drawX;
	int top = sprite.drawY;
	int right = left + sprite.bitmap->width;
	int bottom = top + sprite.bitmap->height;
	drawSprite(sprite, Common::Rect(left, top, right, bottom));
}

// rect is the portion of the sprite to update. It must be entirely contained within the
// sprite's actual, full rectangle.
void Graphics::drawSprite(const Sprite &sprite, const Common::Rect &rect) {
	Common::Rect spriteRect = Common::Rect(sprite.drawX, sprite.drawY,
			sprite.drawX+sprite.bitmap->width, sprite.drawY+sprite.bitmap->height);

	assert(_screenRect.contains(rect));
	assert(spriteRect.contains(rect));

	::Graphics::Surface *surface = _vm->_system->lockScreen();

	byte *dest = (byte*)surface->getPixels() + rect.top*SCREEN_WIDTH + rect.left;

	switch(sprite.drawMode) {
	case 0: { // Normal sprite
		byte *src = sprite.bitmap->pixels + (rect.left - sprite.drawX)
			+ (rect.top - sprite.drawY) * sprite.bitmap->width;

		int priOffset = rect.top*SCREEN_WIDTH + rect.left;

		for (int y = rect.top; y < rect.bottom; y++) {
			for (int x = rect.left; x < rect.right; x++) {
				byte priByte = _priData[priOffset / 2];
				byte bgPriority;
				if ((priOffset % 2) == 1)
					bgPriority = priByte & 0xf;
				else
					bgPriority = priByte >> 4;
				priOffset++;

				byte b = *src++;
				if (b == 0 || sprite.drawPriority < bgPriority) {
					dest++;
					continue;
				}
				*dest++ = b;
			}

			src       += sprite.bitmap->width - rect.width();
			dest      += SCREEN_WIDTH - rect.width();
			priOffset += SCREEN_WIDTH - rect.width();
		}
		break;
	}

	case 1: // Invisible
		break;

	case 2: { // Normal sprite with darkened background for "transparent" pixels (and no priority)
		byte *src = sprite.bitmap->pixels + (rect.left - sprite.drawX)
			+ (rect.top - sprite.drawY) * sprite.bitmap->width;

		for (int y = rect.top; y < rect.bottom; y++) {
			for (int x = rect.left; x < rect.right; x++) {
				byte b = *src;

				if (b == 0) // Transparent (darken the pixel)
					*dest = _lutData[*dest];
				else // Solid color
					*dest = b;

				src++;
				dest++;
			}

			src += sprite.bitmap->width - rect.width();
			dest += SCREEN_WIDTH - rect.width();
		}

		break;
	}

	case 3: { // Text
		// The sprite's "bitmap" is not actually a bitmap, but instead the list of
		// characters to display.

		Common::Rect rectangle1;

		rectangle1.left   = (rect.left   - sprite.drawX) / 8;
		rectangle1.top    = (rect.top    - sprite.drawY) / 8;
		rectangle1.right  = (rect.right  - sprite.drawX) / 8;
		rectangle1.bottom = (rect.bottom - sprite.drawY) / 8;

		int drawWidth = rectangle1.width();
		int drawHeight = rectangle1.height();

		dest = (byte*)surface->getPixels() + sprite.drawY*SCREEN_WIDTH + sprite.drawX
			+ rectangle1.top*8*SCREEN_WIDTH + rectangle1.left*8;

		byte *src = sprite.bitmap->pixels + rectangle1.top * sprite.bitmap->width / 8 + rectangle1.left;

		for (int y=0; y < drawHeight; y++) {
			for (int x=0; x < drawWidth; x++) {
				byte c = *src;

				int textColor;
				if (c >= 0x10 && c <= 0x1A) // Border characters
					textColor = 0xb3;
				else
					textColor = sprite.textColor;

				byte *fontData = _font->getCharData(c);

				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						byte b = *fontData;

						if (b == 0) // Transparent: use lookup table to darken this pixel
							*dest = _lutData[*dest];
						else if (b == 0x78) // Inner part of character
							*dest = textColor;
						else // Outline of character
							*dest = b;

						fontData++;
						dest++;
					}
					dest += SCREEN_WIDTH - 8;
				}

				dest -= (SCREEN_WIDTH * 8 - 8);
				src++;
			}

			src += (sprite.bitmap->width / 8) - drawWidth;
			dest += (SCREEN_WIDTH * 8) - drawWidth * 8;

		}

		break;
	}

	default:
		error("drawSprite: draw mode %d invalid", sprite.drawMode);
		break;
	}

	_vm->_system->unlockScreen();
}

void Graphics::drawAllSprites() {
	// TODO: different video modes?

	if (_numSprites == 0)
		return;

	// TODO: calculateSpriteDrawPriority()

	// Update sprite rectangles
	for (int i=0; i<_numSprites; i++) {
		Sprite *spr = _sprites[i];
		Common::Rect rect;

		rect.left   = spr->pos.x - spr->bitmap->xoffset;
		rect.top    = spr->pos.y - spr->bitmap->yoffset;
		rect.right  = rect.left + spr->bitmap->width;
		rect.bottom = rect.top + spr->bitmap->height;

		spr->drawX = rect.left;
		spr->drawY = rect.top;

		spr->drawRect = rect.findIntersectingRect(_screenRect);

		if (!spr->drawRect.isEmpty()) { // At least partly on-screen
			if (spr->lastDrawRect.left < spr->lastDrawRect.right) {
				// If the sprite's position is close to where it was last time it was
				// drawn, combine the two rectangles and redraw that whole section.
				// Otherwise, redraw the old position and current position separately.
				rect = spr->drawRect.findIntersectingRect(spr->lastDrawRect);

				if (rect.isEmpty())
					spr->rect2Valid = 0;
				else {
					spr->rectangle2 = getRectEncompassing(spr->drawRect, spr->lastDrawRect);
					spr->rect2Valid = 1;
				}
			}
			else {
				spr->rectangle2 = spr->drawRect;
				spr->rect2Valid = 1;
			}

			spr->isOnScreen = 1;
		}
		else { // Off-screen
			spr->rect2Valid = 0;
			spr->isOnScreen = 0;
		}
	}

	// Determine what portions of the screen need to be updated
	Common::Rect dirtyRects[MAX_SPRITES * 2];
	int numDirtyRects = 0;

	for (int i = 0; i < _numSprites; i++) {
		Sprite *spr = _sprites[i];

		if (spr->bitmapChanged) {
			if (spr->isOnScreen) {
				if (spr->rect2Valid) {
					dirtyRects[numDirtyRects++] = spr->rectangle2;
				}
				else {
					dirtyRects[numDirtyRects++] = spr->drawRect;
					dirtyRects[numDirtyRects++] = spr->lastDrawRect;
				}
			}
			else {
				dirtyRects[numDirtyRects++] = spr->lastDrawRect;
			}
		}
	}

	// Redraw the background on every dirty rectangle
	for (int i = 0; i < numDirtyRects; i++) {
		Common::Rect &r = dirtyRects[i];

		int offset = r.top * SCREEN_WIDTH + r.left;
		_vm->_system->copyRectToScreen(_backgroundImage->pixels+offset, SCREEN_WIDTH, r.left, r.top, r.width(), r.height());
	}

	// For each sprite, merge the rectangles that overlap with it and redraw the sprite.
	for (int i = 0; i < _numSprites; i++) {
		Sprite *spr = _sprites[i];

		if (!spr->field16 && spr->isOnScreen) {
			bool mustRedrawSprite = false;
			Common::Rect rect2;

			for (int j = 0; j < numDirtyRects; j++) {
				Common::Rect rect1 = spr->drawRect.findIntersectingRect(dirtyRects[j]);

				if (!rect1.isEmpty()) {
					if (mustRedrawSprite)
						rect2 = getRectEncompassing(rect1, rect2);
					else
						rect2 = rect1;
					mustRedrawSprite = true;
				}
			}

			if (mustRedrawSprite)
				drawSprite(*spr, rect2);
		}

		spr->field16 = false;
		spr->bitmapChanged = false;
		spr->lastDrawRect = spr->drawRect;
	}

	_vm->_system->updateScreen();
}

void Graphics::addSprite(Sprite *sprite) {
	if (_numSprites >= MAX_SPRITES)
		error("addSprite: too many sprites");

	// Initialize some fields
	sprite->drawMode = 0;
	sprite->field8 = 0;
	sprite->field16 = false;
	sprite->bitmapChanged = true; // FIXME (delete this later?)

	sprite->lastDrawRect.top = -1;
	sprite->lastDrawRect.left = -1;
	sprite->lastDrawRect.bottom = -2;
	sprite->lastDrawRect.right = -2;

	_sprites[_numSprites++] = sprite;
}

void Graphics::delSprite(Sprite *sprite) {
	for (int i=0; i<_numSprites; i++) {
		if (sprite != _sprites[i])
			continue;

		_numSprites--;
		_sprites[i] = _sprites[_numSprites];
		return;
	}

	error("delSprite: sprite not in list");
}


void Graphics::copyBackgroundScreen() {
	drawDirectToScreen(_backgroundImage);
}

void Graphics::drawDirectToScreen(SharedPtr<Bitmap> bitmap) {
	int xoffset = bitmap->xoffset;
	int yoffset = bitmap->yoffset;

	_vm->_system->copyRectToScreen(bitmap->pixels, bitmap->width, xoffset, yoffset, bitmap->width, bitmap->height);
}


void Graphics::loadEGAData(const char *filename) {
	// Load EGA palette data
	if (!_egaMode)
		return;

	if (!_egaData)
		_egaData = new byte[256];

	SharedPtr<Common::SeekableReadStream> egaStream = _vm->loadFile(filename);
	egaStream->read(_egaData, 256);
}

void Graphics::drawBackgroundImage(const char *filename) {
	// Draw an stjr BGD image (palette built-in)

	SharedPtr<Common::SeekableReadStream> imageStream = _vm->loadFile(filename);
	byte *palette = new byte[256 * 3];
	imageStream->read(palette, 256 * 3);

	// Expand color components
	for (uint16 i = 0; i < 256 * 3; i++)
		palette[i] <<= 2;

	uint16 xoffset = imageStream->readUint16LE();
	uint16 yoffset = imageStream->readUint16LE();
	uint16 width = imageStream->readUint16LE();
	uint16 height = imageStream->readUint16LE();

	byte *pixels = new byte[width * height];
	imageStream->read(pixels, width * height);

	_vm->_system->getPaletteManager()->setPalette(palette, 0, 256);
	_vm->_system->copyRectToScreen(pixels, width, xoffset, yoffset, width, height);
	_vm->_system->updateScreen();

	delete[] palette;
}

}
