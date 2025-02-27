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

#include "Logging.hpp"
#include "alephversion.hpp"
#include "csstrings.hpp"
#include "shell.hpp"
#include "shell_options.hpp"
#include <SDL_main.h>

int main(int argc, char** argv) {
    // Print banner (don't bother if this doesn't appear when started from a GUI)
    char app_name_version[256];
    expand_app_variables(app_name_version, "Aleph Bet $appLongVersion$");
    printf("%s\n%s\n\n"
           "Original code by Bungie Software <http://www.bungie.com/>\n"
           "Additional work by Loren Petrich, Chris Pruett, Rhys Hill et al.\n"
           "TCP/IP networking by Woody Zenfell\n"
           "SDL port by Christian Bauer <Christian.Bauer@uni-mainz.de>\n"
#if defined(__MACH__) && defined(__APPLE__)
           "Mac OS X/SDL version by Chris Lovell, Alexander Strange, and Woody Zenfell\n"
#endif
           "\nThis is free software with ABSOLUTELY NO WARRANTY.\n"
           "You are welcome to redistribute it under certain conditions.\n"
           "For details, see the file COPYING.\n"
#if defined(__WIN32__)
           // Windows is statically linked against SDL, so we have to include this:
           "\nSimple DirectMedia Layer (SDL) Library included under the terms of the\n"
           "GNU Library General Public License.\n"
           "For details, see the file COPYING.SDL.\n"
#endif
#if !defined(DISABLE_NETWORKING)
           "\nBuilt with network play enabled.\n"
#endif
           ,
           app_name_version, AB_HOMEPAGE_URL);

    shell_options.parse(argc, argv);

    auto code = 0;

    try {

        // Initialize everything
        initialize_application();

        for (std::vector<std::string>::iterator it = shell_options.files.begin(); it != shell_options.files.end();
             ++it) {
            if (handle_open_document(*it)) {
                break;
            }
        }

        // Run the main loop
        main_event_loop();

    } catch (std::exception& e) {
        try {
            logFatal("Unhandled exception: %s", e.what());
        } catch (...) {}
        code = 1;
    } catch (...) {
        try {
            logFatal("Unknown exception");
        } catch (...) {}
        code = 1;
    }

    try {
        shutdown_application();
    } catch (...) {}

    return code;
}
