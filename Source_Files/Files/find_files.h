#ifndef __FIND_FILES_H
#define __FIND_FILES_H

/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

#include "FileHandler.h"

// Finds every type of file
const Typecode WILDCARD_TYPE = _typecode_unknown;

#include <vector>

// File-finder base class
class FileFinder {
public:
	FileFinder() {}
	virtual ~FileFinder() {}

	bool Find(DirectorySpecifier &dir, Typecode type, bool recursive = true) {
		return _Find(dir, type, recursive, 0);
	}

protected:
	bool _Find(DirectorySpecifier& dir, Typecode type, bool recursive, int depth);
	// Gets called for each found file, returns true if search is to be aborted
	virtual bool found(FileSpecifier &file) = 0;
};

// Find all files of given type and append them to a vector
class FindAllFiles : public FileFinder {
public:
	FindAllFiles(vector<FileSpecifier> &v) : dest_vector(v) {dest_vector.clear();}

private:
	bool found(FileSpecifier &file);
	vector<FileSpecifier> &dest_vector;
};

#endif
