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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ASYLUM_STATICRES_H
#define ASYLUM_STATICRES_H

#include "common/rect.h"

namespace Asylum {

/** This fixes the menu icons text x position on screen */
const Common::Rect menuRects[12] = {Common::Rect(  40,  19,   93,  68),
                                    Common::Rect( 140,  19,  193,  68),
                                    Common::Rect( 239,  19,  292,  68),
                                    Common::Rect( 340,  19,  393,  68),
                                    Common::Rect( 439,  19,  492,  68),
                                    Common::Rect( 538,  19,  591,  68),
                                    Common::Rect(  40, 400,   93, 449),
                                    Common::Rect( 140, 400,  193, 449),
                                    Common::Rect( 239, 400,  292, 449),
                                    Common::Rect( 340, 400,  393, 449),
                                    Common::Rect( 538, 400,  591, 449), // The last two icons are swapped
                                    Common::Rect( 439, 400,  492, 449)};

const uint32 eyeFrameIndex[8] = {3, 5, 1, 7, 4, 8, 2, 6};

const int moviesCd[49] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 2, 2, 2, 2, 2, 2, 2, 3, 3,
                          3, 3, 3, 3, 3, 3, 3, 3, 2};

const int chapterIndexes[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 4, 8, 9, 0, 0};

/** Default Actor Indices by Scene */
const int actorType[16] = {0, 0, 0, 0, 1, 0, 2, 0, 3, 0, 1, 1, 3, 0, 0, 0};

/** Speech indexes */
const uint speechIndex[20]       = {17, 22, 27, 37, 45, 12, 16, 19, 25, 29, 14, 18, 23, 29, 35,  6,  9, 13, 19, 27};
const uint speechIndexRandom[20] = { 5,  5, 10,  8,  6,  4,  3,  6,  4,  4,  4,  5,  6,  6,  6,  3,  4,  6,  8,  4};

/** Encounter indexes */
const int encounterPortrait1Index[16] = {-1, 36, 20, 31, 1, 37, 7, 0, 0, 0, 1, 2, 3, 0, 0, 0};
const int encounterPortrait2Index[80] = {32, 33, 34, 21, 22, 23, 24, 25, 26, 27,
                                         28, 29, 30, 31, 32,  1,  2,  3,  1,  2,
                                          3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
                                         13, 14, 19, 20, 21, 22, 23, 24, 25, 26,
                                         27, 28, 29, 30,  2,  0,  3,  4,  5,  6,
                                          7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
                                         17, 18, 19, 20, 21, 22,  0,  1,  2,  3,
                                          4,  5,  6, 35, 33, 34, 35, 36, 37, 36};

// Delta array for points
static Common::Point deltaPointsArray[8] = {
	Common::Point( 0, -1),
	Common::Point(-1, -1),
	Common::Point(-1,  0),
	Common::Point(-1,  1),
	Common::Point( 0,  1),
	Common::Point( 1,  1),
	Common::Point( 1,  0),
	Common::Point( 1, -1)
};

// We hardcode all the text resources here. It makes the resulting code easier,
// otherwise we'll have to read the text resources in the same obscure way
// they're stored in VIDS.CAP.
// A value of -1 means that the video has no subtitles, -2 that it doesn't exist.
// The negative values aren't used in the code, they just make the table
// easier to understand.
const int32 video_subtitle_resourceIds[49] = {
	-1,   1088, 1279, 1122, 1286, 1132, 1133, 1134, 1135, 1136, // 0 - 9
	-1,   -2,   1140, 1141, -2,   -1,   1142, -1,   -2,   1155, // 10 - 19
	1157, 1159, 1162, 1164, -2,   1171, 1177, 1184, 1190, 1201, // 20 - 29
	-2,   -2,   -2,   1207, 1213, 1217, 1223, 1227, -2,   1228, // 30 - 39
	-2,   1244, 1247, 1250, 1256, 1120, 1127, -1,   -1 };       // 40 - 48

// Angle Tables used by getAngle()
const uint8 angleTable01[256] = {
	0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07,
	0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
	0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E,
	0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x12, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x14, 0x14,
	0x15, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17, 0x17,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
	0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D,
	0x1D, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
	0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x25, 0x25,
	0x25, 0x25, 0x25, 0x25, 0x25, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x27, 0x27, 0x27, 0x27,
	0x27, 0x27, 0x27, 0x27, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x29, 0x29,
	0x29, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B,
	0x2B, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2D
};

const uint8 angleTable02[256] = {
	0x00, 0x04, 0x07, 0x0B, 0x0E, 0x11, 0x15, 0x18, 0x1B, 0x1D, 0x20, 0x23, 0x25, 0x27, 0x29, 0x2B,
	0x2D, 0x2F, 0x30, 0x32, 0x33, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x3F, 0x40, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x44, 0x45, 0x45, 0x46, 0x46, 0x46, 0x47, 0x47,
	0x48, 0x48, 0x48, 0x49, 0x49, 0x49, 0x49, 0x4A, 0x4A, 0x4A, 0x4B, 0x4B, 0x4B, 0x4B, 0x4C, 0x4C,
	0x4C, 0x4C, 0x4C, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4F,
	0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x52, 0x52, 0x52, 0x52,
	0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
	0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x54, 0x54, 0x54,
	0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
	0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x56, 0x56, 0x56, 0x56,
	0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56,
	0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56,
	0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56
};

const uint8 angleTable03[256] = {
	0x00, 0x45, 0x63, 0x72, 0x76, 0x79, 0x81, 0x82, 0x83, 0x84, 0x84, 0x85, 0x85, 0x86, 0x86, 0x86,
	0x86, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89,
	0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89,
	0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89,
	0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89,
	0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89,
	0x89, 0x89, 0x89, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

} // end of namespace Asylum

#endif // ASYLUM_STATICRES_H
