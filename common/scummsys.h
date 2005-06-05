/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2005 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */
#ifndef SCUMMSYS_H
#define SCUMMSYS_H

#if !defined(_STDAFX_H) && !defined(__PLAYSTATION2__)
#error Included scummsys.h without including stdafx.h first!
#endif

#include <stdlib.h>
#include <stdio.h>

// Use config.h, generated by configure
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

#if defined(_MSC_VER)

	#define scumm_stricmp stricmp
	#define scumm_strnicmp _strnicmp
	#define snprintf _snprintf

	#if defined(CHECK_HEAP)
	#undef CHECK_HEAP
	#define CHECK_HEAP checkHeap();
	#endif

	#define SCUMM_LITTLE_ENDIAN

	#define FORCEINLINE __forceinline
	#define NORETURN _declspec(noreturn)
	#define PLUGIN_EXPORT __declspec(dllexport)

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned long uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed long int32;
	
	#define START_PACK_STRUCTS pack(push, 1)
	#define END_PACK_STRUCTS	 pack(pop)

#if defined(_WIN32_WCE) && _WIN32_WCE < 300

	#define CDECL __cdecl

#endif

#elif defined(__MINGW32__)

	#define scumm_stricmp stricmp
	#define scumm_strnicmp strnicmp
	#define SCUMM_LITTLE_ENDIAN

	#define _HEAPOK 0

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;

	#define START_PACK_STRUCTS pack (push, 1)
	#define END_PACK_STRUCTS	 pack(pop)

	#define PLUGIN_EXPORT __declspec(dllexport)

#elif defined(UNIX)

	#define scumm_stricmp strcasecmp
	#define scumm_strnicmp strncasecmp

	#ifndef CONFIG_H
		#ifdef X11_BACKEND
	
		// You need to set this manually if necessary
	//	#define SCUMM_LITTLE_ENDIAN
		
		#else
		/* need this for the SDL_BYTEORDER define */
		#include <SDL_byteorder.h>
	
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		#define SCUMM_LITTLE_ENDIAN
		#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
		#define SCUMM_BIG_ENDIAN
		#else
		#error Neither SDL_BIG_ENDIAN nor SDL_LIL_ENDIAN is set.
		#endif
		#endif
	#endif

	// You need to set this manually if necessary
//	#define SCUMM_NEED_ALIGNMENT

	#ifndef HAVE_CONFIG_H
	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint;
	typedef unsigned int uint32;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	#endif

	#if defined(__DECCXX) // Assume alpha architecture
	#define INVERSE_MKID
	#define SCUMM_NEED_ALIGNMENT
	#endif

	#if defined(__GNUC__)
	#else
	#define START_PACK_STRUCTS pack (1)
	#define END_PACK_STRUCTS	 pack ()
	#endif

#elif defined(__PALMOS_TRAPS__)	// PALMOS
	#include <PalmOS.h>
	#include "globals.h"
	#include "extend.h"
	
	#define STRINGBUFLEN 256

	#define scumm_stricmp stricmp
	#define scumm_strnicmp strnicmp
	
	#define SCUMM_BIG_ENDIAN
	#define SCUMM_NEED_ALIGNMENT

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned long uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed long int32;
	
	#define START_PACK_STRUCTS pack (1)
	#define END_PACK_STRUCTS   pack ()

	#define NEWGUI_256

#elif defined(__MORPHOS__)
	#define scumm_stricmp stricmp
	#define scumm_strnicmp strnicmp

	#define SCUMM_BIG_ENDIAN
	#define SCUMM_NEED_ALIGNMENT

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned long uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed long int32;

	#if defined(__GNUC__)
	#else
		#define START_PACK_STRUCTS pack (1)
		#define END_PACK_STRUCTS	 pack ()
	#endif
	#define main morphos_main

#elif defined(__DC__)

	#define scumm_stricmp strcasecmp
	#define scumm_strnicmp strncasecmp
	#define SCUMM_LITTLE_ENDIAN
	#define SCUMM_NEED_ALIGNMENT

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned long uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed long int32;

	#define START_PACK_STRUCTS pack (push, 1)
	#define END_PACK_STRUCTS	 pack(pop)

#elif defined __GP32__ //ph0x
	#define SCUMM_NEED_ALIGNMENT
	#define SCUMM_LITTLE_ENDIAN 

	#define scumm_stricmp stricmp
	#define scumm_strnicmp strnicmp

	#define _HEAPOK 0

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned long uint32;
	typedef unsigned int uint;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed long int32;

	#define START_PACK_STRUCTS pack (push, 1)
	#define END_PACK_STRUCTS	 pack(pop)
#elif defined __PLAYSTATION2__
	#define SCUMM_NEED_ALIGNMENT
	#define SCUMM_LITTLE_ENDIAN 

	#define scumm_stricmp strcasecmp
	#define scumm_strnicmp strncasecmp

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned int uint;
	typedef unsigned long uint64;
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef signed long int64;

	#define START_PACK_STRUCTS pack (push, 1)
	#define END_PACK_STRUCTS	 pack(pop)

	#include "backends/ps2/fileio.h"

	#define fopen(a, b)			ps2_fopen(a, b)
	#define fclose(a)			ps2_fclose(a)
	#define fflush(a)			ps2_fflush(a)
	#define fseek(a, b, c)		ps2_fseek(a, b, c)
	#define ftell(a)			ps2_ftell(a)
	#define feof(a)				ps2_feof(a)
	#define fread(a, b, c, d)	ps2_fread(a, b, c, d)
	#define fwrite(a, b, c, d)	ps2_fwrite(a, b, c, d)
	#define fgetc(a)			ps2_fgetc(a)
	#define fgets(a, b, c)		ps2_fgets(a, b, c)
	#define fputc(a, b)			ps2_fputc(a, b)
	#define fputs(a, b)			ps2_fputs(a, b)
	#define fprintf				ps2_fprintf
	#define fsize(a)			ps2_fsize(a)

	extern void ps2_disableHandleCaching(void);

#elif defined (__amigaos4__)
	#include <exec/types.h>

	#define	scumm_stricmp strcasecmp
	#define	scumm_strnicmp strncasecmp

	#define	SCUMM_BIG_ENDIAN

	// You need to set this manually if necessary
	#define	SCUMM_NEED_ALIGNMENT

	#ifndef	HAVE_CONFIG_H
	typedef	unsigned char byte;
	typedef	unsigned int uint;
	#endif
#else
	#error No system type defined
#endif


//
// GCC specific stuff
//
#if defined(__GNUC__)
	#define GCC_PACK __attribute__((packed))
	#define NORETURN __attribute__((__noreturn__)) 
#else
	#define GCC_PACK
#endif


//
// Fallbacks / default values for various special macros
//
#ifndef START_PACK_STRUCTS
#define	START_PACK_STRUCTS
#define	END_PACK_STRUCTS
#endif

#ifndef FORCEINLINE
#define FORCEINLINE inline
#endif

#ifndef CHECK_HEAP
#define CHECK_HEAP
#endif

#ifndef CDECL
#define	CDECL
#endif

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT
#endif

#ifndef NORETURN
#define	NORETURN
#endif

#ifndef STRINGBUFLEN
#define STRINGBUFLEN 1024
#endif


//
// Overlay color type (FIXME: shouldn't be declared here)
//
#if defined(NEWGUI_256)
	// 256 color only on PalmOS
	typedef byte OverlayColor;
#else
	// 15/16 bit color mode everywhere else...
	typedef int16 OverlayColor;
#endif


//
// Endian conversion functions, macros etc., follow from here!
//

FORCEINLINE uint32 SWAP_BYTES_32(uint32 a) {
	return ((a >> 24) & 0x000000FF) |
		   ((a >>  8) & 0x0000FF00) |
		   ((a <<  8) & 0x00FF0000) |
		   ((a << 24) & 0xFF000000);
}

FORCEINLINE uint16 SWAP_BYTES_16(uint16 a) {
	return ((a >> 8) & 0x00FF) + ((a << 8) & 0xFF00);
}


#if defined(SCUMM_LITTLE_ENDIAN)

	#define PROTO_MKID(a) ((uint32) \
			(((a) >> 24) & 0x000000FF) | \
			(((a) >>  8) & 0x0000FF00) | \
			(((a) <<  8) & 0x00FF0000) | \
			(((a) << 24) & 0xFF000000))
	#define PROTO_MKID_BE(a) ((uint32)(a))

	#if defined(INVERSE_MKID)
	#  define MKID(a) PROTO_MKID_BE(a)
	#  define MKID_BE(a) PROTO_MKID(a)
	#else
	#  define MKID(a) PROTO_MKID(a)
	#  define MKID_BE(a) PROTO_MKID_BE(a)
	#endif

	#define READ_UINT32(a) READ_LE_UINT32(a)

	#define FROM_LE_32(a) ((uint32)(a))
	#define FROM_LE_16(a) ((uint16)(a))

	#define TO_LE_32(a) ((uint32)(a))
	#define TO_LE_16(a) ((uint16)(a))

	#define TO_BE_32(a) SWAP_BYTES_32(a)
	#define TO_BE_16(a) SWAP_BYTES_16(a)

#elif defined(SCUMM_BIG_ENDIAN)

	#define MKID(a) ((uint32)(a))
	#define MKID_BE(a) ((uint32)(a))
	//#define MKID_BE(a) SWAP_BYTES_32(a)

	#define READ_UINT32(a) READ_BE_UINT32(a)

	#define FROM_LE_32(a) SWAP_BYTES_32(a)
	#define FROM_LE_16(a) SWAP_BYTES_16(a)

	#define TO_LE_32(a) SWAP_BYTES_32(a)
	#define TO_LE_16(a) SWAP_BYTES_16(a)

	#define TO_BE_32(a) ((uint32)(a))
	#define TO_BE_16(a) ((uint16)(a))

#else

	#error No endianness defined

#endif


#if defined(SCUMM_NEED_ALIGNMENT) || defined(SCUMM_BIG_ENDIAN)
	FORCEINLINE uint16 READ_LE_UINT16(const void *ptr) {
		const byte *b = (const byte *)ptr;
		return (b[1] << 8) + b[0];
	}
	FORCEINLINE uint32 READ_LE_UINT32(const void *ptr) {
		const byte *b = (const byte *)ptr;
		return (b[3] << 24) + (b[2] << 16) + (b[1] << 8) + (b[0]);
	}
	FORCEINLINE void WRITE_LE_UINT16(void *ptr, uint16 value) {
		byte *b = (byte *)ptr;
		b[0] = (byte)(value >> 0);
		b[1] = (byte)(value >> 8);
	}
	FORCEINLINE void WRITE_LE_UINT32(void *ptr, uint32 value) {
		byte *b = (byte *)ptr;
		b[0] = (byte)(value >>  0);
		b[1] = (byte)(value >>  8);
		b[2] = (byte)(value >> 16);
		b[3] = (byte)(value >> 24);
	}
#else
	FORCEINLINE uint16 READ_LE_UINT16(const void *ptr) {
		return *(const uint16 *)(ptr);
	}
	FORCEINLINE uint32 READ_LE_UINT32(const void *ptr) {
		return *(const uint32 *)(ptr);
	}
	FORCEINLINE void WRITE_LE_UINT16(void *ptr, uint16 value) {
		*(uint16 *)(ptr) = value;
	}
	FORCEINLINE void WRITE_LE_UINT32(void *ptr, uint32 value) {
		*(uint32 *)(ptr) = value;
	}
#endif


#if defined(SCUMM_NEED_ALIGNMENT) || defined(SCUMM_LITTLE_ENDIAN)
	FORCEINLINE uint16 READ_BE_UINT16(const void *ptr) {
		const byte *b = (const byte *)ptr;
		return (b[0] << 8) + b[1];
	}
	FORCEINLINE uint32 READ_BE_UINT32(const void *ptr) {
		const byte *b = (const byte*)ptr;
		return (b[0] << 24) + (b[1] << 16) + (b[2] << 8) + (b[3]);
	}
	FORCEINLINE void WRITE_BE_UINT16(void *ptr, uint16 value) {
		byte *b = (byte *)ptr;
		b[0] = (byte)(value >> 8);
		b[1] = (byte)(value >> 0);
	}
	FORCEINLINE void WRITE_BE_UINT32(void *ptr, uint32 value) {
		byte *b = (byte *)ptr;
		b[0] = (byte)(value >> 24);
		b[1] = (byte)(value >> 16);
		b[2] = (byte)(value >>  8);
		b[3] = (byte)(value >>  0);
	}
#else
	FORCEINLINE uint16 READ_BE_UINT16(const void *ptr) {
		return *(const uint16 *)(ptr);
	}
	FORCEINLINE uint32 READ_BE_UINT32(const void *ptr) {
		return *(const uint32 *)(ptr);
	}
	FORCEINLINE void WRITE_BE_UINT16(void *ptr, uint16 value) {
		*(uint16 *)(ptr) = value;
	}
	FORCEINLINE void WRITE_BE_UINT32(void *ptr, uint32 value) {
		*(uint32 *)(ptr) = value;
	}
#endif

FORCEINLINE uint32 READ_LE_UINT24(const void *ptr) {
	const byte *b = (const byte *)ptr;
	return (b[2] << 16) + (b[1] << 8) + (b[0]);
}

FORCEINLINE uint32 READ_BE_UINT24(const void *ptr) {
	const byte *b = (const byte*)ptr;
	return (b[0] << 16) + (b[1] << 8) + (b[2]);
}
	
#endif
