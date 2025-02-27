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

/*
 *  find_files_sdl.cpp - Routines for finding files, SDL implementation
 */

#include "FileHandler.hpp"
#include "cseries.hpp"
#include "find_files.hpp"

#include <algorithm>
#include <vector>

/*
 *  File finder base class
 */

bool FileFinder::_Find(DirectorySpecifier& dir, Typecode type, bool recursive, int depth) {
    // Get list of entries in directory
    vector<dir_entry> entries;
    if (!dir.ReadDirectory(entries))
        return false;
    sort(entries.begin(), entries.end());

    // Iterate through entries
    vector<dir_entry>::const_iterator i, end = entries.end();
    for (i = entries.begin(); i != end; i++) {

        // Construct full specifier of file/dir
        FileSpecifier file = dir + i->name;

        if (i->is_directory) {

            if (depth == 0 && i->name == "Plugins")
                continue;

            // Recurse into directory
            if (recursive)
                if (_Find(file, type, recursive, depth + 1))
                    return true;

        } else {

            // Check file type and call found() function
            if (type == WILDCARD_TYPE || type == file.GetType())
                if (found(file))
                    return true;
        }
    }
    return false;
}

/*
 *  Find all files of given type
 */

bool FindAllFiles::found(FileSpecifier& file) {
    dest_vector.push_back(file);
    return false;
}
