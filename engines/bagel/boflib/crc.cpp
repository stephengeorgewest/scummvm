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

#include "bagel/boflib/crc.h"

namespace Bagel {


STATIC const USHORT crc32tabLo[256] = {
	0x0000, 0x3096, 0x612c, 0x51ba, 0xc419, 0xf48f, 0xa535, 0x95a3,
	0x8832, 0xb8a4, 0xe91e, 0xd988, 0x4c2b, 0x7cbd, 0x2d07, 0x1d91,
	0x1064, 0x20f2, 0x7148, 0x41de, 0xd47d, 0xe4eb, 0xb551, 0x85c7,
	0x9856, 0xa8c0, 0xf97a, 0xc9ec, 0x5c4f, 0x6cd9, 0x3d63, 0x0df5,
	0x20c8, 0x105e, 0x41e4, 0x7172, 0xe4d1, 0xd447, 0x85fd, 0xb56b,
	0xa8fa, 0x986c, 0xc9d6, 0xf940, 0x6ce3, 0x5c75, 0x0dcf, 0x3d59,
	0x30ac, 0x003a, 0x5180, 0x6116, 0xf4b5, 0xc423, 0x9599, 0xa50f,
	0xb89e, 0x8808, 0xd9b2, 0xe924, 0x7c87, 0x4c11, 0x1dab, 0x2d3d,
	0x4190, 0x7106, 0x20bc, 0x102a, 0x8589, 0xb51f, 0xe4a5, 0xd433,
	0xc9a2, 0xf934, 0xa88e, 0x9818, 0x0dbb, 0x3d2d, 0x6c97, 0x5c01,
	0x51f4, 0x6162, 0x30d8, 0x004e, 0x95ed, 0xa57b, 0xf4c1, 0xc457,
	0xd9c6, 0xe950, 0xb8ea, 0x887c, 0x1ddf, 0x2d49, 0x7cf3, 0x4c65,
	0x6158, 0x51ce, 0x0074, 0x30e2, 0xa541, 0x95d7, 0xc46d, 0xf4fb,
	0xe96a, 0xd9fc, 0x8846, 0xb8d0, 0x2d73, 0x1de5, 0x4c5f, 0x7cc9,
	0x713c, 0x41aa, 0x1010, 0x2086, 0xb525, 0x85b3, 0xd409, 0xe49f,
	0xf90e, 0xc998, 0x9822, 0xa8b4, 0x3d17, 0x0d81, 0x5c3b, 0x6cad,
	0x8320, 0xb3b6, 0xe20c, 0xd29a, 0x4739, 0x77af, 0x2615, 0x1683,
	0x0b12, 0x3b84, 0x6a3e, 0x5aa8, 0xcf0b, 0xff9d, 0xae27, 0x9eb1,
	0x9344, 0xa3d2, 0xf268, 0xc2fe, 0x575d, 0x67cb, 0x3671, 0x06e7,
	0x1b76, 0x2be0, 0x7a5a, 0x4acc, 0xdf6f, 0xeff9, 0xbe43, 0x8ed5,
	0xa3e8, 0x937e, 0xc2c4, 0xf252, 0x67f1, 0x5767, 0x06dd, 0x364b,
	0x2bda, 0x1b4c, 0x4af6, 0x7a60, 0xefc3, 0xdf55, 0x8eef, 0xbe79,
	0xb38c, 0x831a, 0xd2a0, 0xe236, 0x7795, 0x4703, 0x16b9, 0x262f,
	0x3bbe, 0x0b28, 0x5a92, 0x6a04, 0xffa7, 0xcf31, 0x9e8b, 0xae1d,
	0xc2b0, 0xf226, 0xa39c, 0x930a, 0x06a9, 0x363f, 0x6785, 0x5713,
	0x4a82, 0x7a14, 0x2bae, 0x1b38, 0x8e9b, 0xbe0d, 0xefb7, 0xdf21,
	0xd2d4, 0xe242, 0xb3f8, 0x836e, 0x16cd, 0x265b, 0x77e1, 0x4777,
	0x5ae6, 0x6a70, 0x3bca, 0x0b5c, 0x9eff, 0xae69, 0xffd3, 0xcf45,
	0xe278, 0xd2ee, 0x8354, 0xb3c2, 0x2661, 0x16f7, 0x474d, 0x77db,
	0x6a4a, 0x5adc, 0x0b66, 0x3bf0, 0xae53, 0x9ec5, 0xcf7f, 0xffe9,
	0xf21c, 0xc28a, 0x9330, 0xa3a6, 0x3605, 0x0693, 0x5729, 0x67bf,
	0x7a2e, 0x4ab8, 0x1b02, 0x2b94, 0xbe37, 0x8ea1, 0xdf1b, 0xef8d};

STATIC const USHORT crc32tabHi[256] = {
	0x0000, 0x7707, 0xee0e, 0x9909, 0x076d, 0x706a, 0xe963, 0x9e64,
	0x0edb, 0x79dc, 0xe0d5, 0x97d2, 0x09b6, 0x7eb1, 0xe7b8, 0x90bf,
	0x1db7, 0x6ab0, 0xf3b9, 0x84be, 0x1ada, 0x6ddd, 0xf4d4, 0x83d3,
	0x136c, 0x646b, 0xfd62, 0x8a65, 0x1401, 0x6306, 0xfa0f, 0x8d08,
	0x3b6e, 0x4c69, 0xd560, 0xa267, 0x3c03, 0x4b04, 0xd20d, 0xa50a,
	0x35b5, 0x42b2, 0xdbbb, 0xacbc, 0x32d8, 0x45df, 0xdcd6, 0xabd1,
	0x26d9, 0x51de, 0xc8d7, 0xbfd0, 0x21b4, 0x56b3, 0xcfba, 0xb8bd,
	0x2802, 0x5f05, 0xc60c, 0xb10b, 0x2f6f, 0x5868, 0xc161, 0xb666,
	0x76dc, 0x01db, 0x98d2, 0xefd5, 0x71b1, 0x06b6, 0x9fbf, 0xe8b8,
	0x7807, 0x0f00, 0x9609, 0xe10e, 0x7f6a, 0x086d, 0x9164, 0xe663,
	0x6b6b, 0x1c6c, 0x8565, 0xf262, 0x6c06, 0x1b01, 0x8208, 0xf50f,
	0x65b0, 0x12b7, 0x8bbe, 0xfcb9, 0x62dd, 0x15da, 0x8cd3, 0xfbd4,
	0x4db2, 0x3ab5, 0xa3bc, 0xd4bb, 0x4adf, 0x3dd8, 0xa4d1, 0xd3d6,
	0x4369, 0x346e, 0xad67, 0xda60, 0x4404, 0x3303, 0xaa0a, 0xdd0d,
	0x5005, 0x2702, 0xbe0b, 0xc90c, 0x5768, 0x206f, 0xb966, 0xce61,
	0x5ede, 0x29d9, 0xb0d0, 0xc7d7, 0x59b3, 0x2eb4, 0xb7bd, 0xc0ba,
	0xedb8, 0x9abf, 0x03b6, 0x74b1, 0xead5, 0x9dd2, 0x04db, 0x73dc,
	0xe363, 0x9464, 0x0d6d, 0x7a6a, 0xe40e, 0x9309, 0x0a00, 0x7d07,
	0xf00f, 0x8708, 0x1e01, 0x6906, 0xf762, 0x8065, 0x196c, 0x6e6b,
	0xfed4, 0x89d3, 0x10da, 0x67dd, 0xf9b9, 0x8ebe, 0x17b7, 0x60b0,
	0xd6d6, 0xa1d1, 0x38d8, 0x4fdf, 0xd1bb, 0xa6bc, 0x3fb5, 0x48b2,
	0xd80d, 0xaf0a, 0x3603, 0x4104, 0xdf60, 0xa867, 0x316e, 0x4669,
	0xcb61, 0xbc66, 0x256f, 0x5268, 0xcc0c, 0xbb0b, 0x2202, 0x5505,
	0xc5ba, 0xb2bd, 0x2bb4, 0x5cb3, 0xc2d7, 0xb5d0, 0x2cd9, 0x5bde,
	0x9b64, 0xec63, 0x756a, 0x026d, 0x9c09, 0xeb0e, 0x7207, 0x0500,
	0x95bf, 0xe2b8, 0x7bb1, 0x0cb6, 0x92d2, 0xe5d5, 0x7cdc, 0x0bdb,
	0x86d3, 0xf1d4, 0x68dd, 0x1fda, 0x81be, 0xf6b9, 0x6fb0, 0x18b7,
	0x8808, 0xff0f, 0x6606, 0x1101, 0x8f65, 0xf862, 0x616b, 0x166c,
	0xa00a, 0xd70d, 0x4e04, 0x3903, 0xa767, 0xd060, 0x4969, 0x3e6e,
	0xaed1, 0xd9d6, 0x40df, 0x37d8, 0xa9bc, 0xdebb, 0x47b2, 0x30b5,
	0xbdbd, 0xcaba, 0x53b3, 0x24b4, 0xbad0, 0xcdd7, 0x54de, 0x23d9,
	0xb366, 0xc461, 0x5d68, 0x2a6f, 0xb40b, 0xc30c, 0x5a05, 0x2d02};

ULONG CalcCrcQuick(UBYTE chr, ULONG crcValue) {
	chr ^= (UBYTE)crcValue;
	return (crcValue >> 8) ^ crc32tabLo[chr] ^ ((ULONG)crc32tabHi[chr] << 16);
}

ULONG CalculateCRC(const VOID HUGE *pBuffer, LONG lBufLen, ULONG lCrcValue) {
	LONG i;
	UBYTE HUGE *p, c;

	Assert(pBuffer != NULL);
	Assert(lBufLen > 0);

	p = (UBYTE HUGE *)pBuffer;
	i = -1;
	while (++i < lBufLen) {

		c = (UBYTE)(*p ^ (UBYTE)lCrcValue);
		lCrcValue = (lCrcValue >> 8) ^ crc32tabLo[c] ^ ((ULONG)crc32tabHi[c] << 16);

		p++;
	}

	return lCrcValue;
}

} // namespace Bagel
