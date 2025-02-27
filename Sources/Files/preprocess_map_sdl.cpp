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
 *  preprocess_map_sdl.cpp - Save game routines, SDL implementation
 */

#include "FileHandler.hpp"
#include "cseries.hpp"

#include <vector>

#include "QuickSave.hpp"
#include "game_errors.hpp"
#include "game_wad.hpp"
#include "interface.hpp"
#include "map.hpp"
#include "shell.hpp"
#include "world.hpp"

// From shell_sdl.cpp
extern vector<DirectorySpecifier> data_search_path;

/*
 *  Get FileSpecifiers for default data files
 */

static bool get_default_spec(FileSpecifier& file, const string& name) {
    vector<DirectorySpecifier>::const_iterator i = data_search_path.begin(), end = data_search_path.end();
    while (i != end) {
        file = *i + name;
        if (file.Exists())
            return true;
        i++;
    }
    return false;
}

static bool get_default_spec(FileSpecifier& file, int type) {
    char name[256];
    getcstr(name, strFILENAMES, type);
    return get_default_spec(file, name);
}

bool have_default_files(void) {
    FileSpecifier file;
    return (get_default_spec(file, filenameDEFAULT_MAP)
            && (get_default_spec(file, filenameIMAGES) || get_default_spec(file, filenameEXTERNAL_RESOURCES))
            && get_default_spec(file, filenameSHAPES8));
}

void get_default_external_resources_spec(FileSpecifier& file) { get_default_spec(file, filenameEXTERNAL_RESOURCES); }

void get_default_map_spec(FileSpecifier& file) {
    if (!get_default_spec(file, filenameDEFAULT_MAP))
        alert_bad_extra_file();
}

void get_default_physics_spec(FileSpecifier& file) {
    get_default_spec(file, filenamePHYSICS_MODEL);
    // don't care if it does not exist
}

void get_default_shapes_spec(FileSpecifier& file) {
    if (!get_default_spec(file, filenameSHAPES8))
        alert_bad_extra_file();
}

void get_default_sounds_spec(FileSpecifier& file) {
    get_default_spec(file, filenameSOUNDS8);
    // don't care if it does not exist
}

bool get_default_music_spec(FileSpecifier& file) { return get_default_spec(file, filenameMUSIC); }

bool get_default_theme_spec(FileSpecifier& file) {
    FileSpecifier theme  = "Themes";
    theme               += getcstr(temporary, strFILENAMES, filenameDEFAULT_THEME);
    return get_default_spec(file, theme.GetPath());
}

/*
 *  Choose saved game for loading
 */

bool choose_saved_game_to_load(FileSpecifier& saved_game) { return load_quick_save_dialog(saved_game); }

/*
 *  Save game
 */

bool save_game(void) {
    bool success = create_quick_save();
    if (success)
        screen_printf("Game saved");
    else
        screen_printf("Save failed");

    return success;
}

/*
 *  Store additional data in saved game file
 */

void add_finishing_touches_to_save_file(FileSpecifier& file) {
    // The MacOS version stores an overhead thumbnail and the level name
    // in resources. Do we want to imitate this?
}
