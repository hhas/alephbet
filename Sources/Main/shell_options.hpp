#ifndef SHELL_OPTIONS_H
#define SHELL_OPTIONS_H

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

#include <string>
#include <unordered_map>
#include <vector>

struct ShellOptions {
    std::unordered_map<int, bool> parse(int argc, char** argv, bool ignore_unknown_args = false);

    std::string program_name;

    bool nogl;
    bool nosound;
    bool nogamma;
    bool debug;
    bool nojoystick;
    bool insecure_lua;

    bool force_fullscreen;
    bool force_windowed;

    bool skip_intro;
    bool editor;

    std::string replay_directory;

    std::string directory;
    std::vector<std::string> files;

    std::string output;
};

extern ShellOptions shell_options;

#endif
