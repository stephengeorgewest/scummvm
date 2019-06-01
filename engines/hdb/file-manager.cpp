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
#include "hdb/file-manager.h"
#include "common/debug.h"
#include "common/file.h"
#include "common/error.h"

namespace HDB {

FileMan::FileMan() {
	_mpcFile = new Common::File;
}

bool FileMan::openMPC(const Common::String &filename) {
	uint32 offset;

	if (!_mpcFile->open(filename)) {
		error("FileMan::openMSD(): Error reading the MSD file");
		return false;
	}

	dataHeader.id = _mpcFile->readUint32BE();
	
	if (dataHeader.id == MKTAG('M', 'P', 'C', 'C')) {
		_compressed = true;
		debug("COMPRESSED FILE");
		return false;
	}
	else if (dataHeader.id == MKTAG('M', 'P', 'C', 'U')) {
		_compressed = false;
		
		offset = _mpcFile->readUint32LE();
		_mpcFile->seek((int32)offset);

		// Note: The MPC archive format assumes the offset to be uint32,
		// but Common::File::seek() takes the offset as int32. 
		
		dataHeader.dirSize = _mpcFile->readUint32LE();

		debug(8, "MPC: Read %d entries", dataHeader.dirSize);

		for (uint32 fileIndex = 0; fileIndex < dataHeader.dirSize; fileIndex++) {
			MPCEntry *dirEntry = new MPCEntry();

			for (int i = 0; i < 64; i++) {
				dirEntry->filename[i] = _mpcFile->readByte();
			}
			
			dirEntry->offset = _mpcFile->readUint32LE();
			dirEntry->length = _mpcFile->readUint32LE();
			dirEntry->ulength = _mpcFile->readUint32LE();
			dirEntry->type = (DataType)_mpcFile->readUint32LE();

			_dir.push_back(dirEntry);
		}
		
		return true;

	}
	
	error("Invalid MPC File.");
	return false;

}

void FileMan::closeMPC() {
	_dir.clear();
	_mpcFile->close();
}

void FileMan::seek(int32 offset, int flag) {
	_mpcFile->seek(offset, flag);
}

Common::SeekableReadStream *FileMan::findFirstData(const char *string, DataType type) {
	Common::String fileString;
	MPCEntry *file = NULL;

	// Find MPC Entry
	for (MPCIterator it = _dir.begin(); it != _dir.end(); it++) {
		fileString = (*it)->filename;
		if (fileString.contains(string)) {
			if ((*it)->type == type) {
				file = *it;
				break;
			}
		}
	}

	if (file == NULL) {
		return NULL;
	}

	// Load corresponding file into a buffer
	_mpcFile->seek(file->offset);
	byte *buffer = new byte[file->ulength];

	_mpcFile->read(buffer, file->ulength);

	// Return buffer wrapped in a MemoryReadStream
	return new Common::MemoryReadStream(buffer, file->ulength);
}

/*
MPCEntry **FileMan::findNextData(MPCIterator begin) {
	Common::String fileString;

	for (MPCIterator it = begin+1; it != _dir.end(); it++) {
		fileString = (*it)->filename;
		if (fileString.contains((*begin)->filename)) {
			if ((*it)->type == (*begin)->type) {
				return it;
			}
		}
	}
	return NULL;
}

int FileMan::findAmount(char *string, DataType type) {
	int count = 0;
	
	MPCIterator it = findFirstData(string, type);
	while (it && (*it)->type == type) {
		count++;
		it = findNextData(it);
	}

	return count;
}*/

} // End of Namespace HDB
