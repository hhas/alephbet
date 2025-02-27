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

#include "shell_options.hpp"

#include <iostream>
#include <functional>
#include <sstream>

#include "FileHandler.hpp"
#include "Logging.hpp"
#include "csstrings.hpp"

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef CreateDirectory
#endif

static void print_usage();
static void print_version();

ShellOptions shell_options;

struct ShellOptionsOption {
	bool match(const std::string& s) {
		if (s[0] == '-') {
			if (s[1] == '-') {
				return long_name == s.substr(2);
			} else if (short_name.size()) {
				return short_name == s.substr(1);
			}
		}

		return false;
	}
	
	std::string short_name;
	std::string long_name;
	std::string help;
};

static int help_tab_stop = 33;

static std::string spaces(int num_spaces)
{
	std::string s;
	for (auto i = 0; i < num_spaces; ++i) {
		s += " ";
	}
	return s;
}

static std::ostream& operator<<(std::ostream& s, const ShellOptionsOption& o) {
	if (o.help.size())
	{
		auto num_spaces = help_tab_stop - 12 - o.long_name.size();
		
		s << "\t[";
		if (o.short_name.size())
		{
			num_spaces -= 4;
			num_spaces -= o.short_name.size();
			s << "-" << o.short_name << " | ";
		}
		
		s << "--" << o.long_name << "]" << spaces(num_spaces) << o.help << "\n";
	}

	return s;
}

struct ShellOptionsCommand : public ShellOptionsOption {
	std::function<void()> command;
};

struct ShellOptionsFlag : public ShellOptionsOption {
	bool& flag;
};

struct ShellOptionsString : public ShellOptionsOption {
	std::string& string;
};

static std::string ignore;

static const std::vector<ShellOptionsCommand> shell_options_commands {
	{"h", "help", "Display this help message", print_usage},
	{"v", "version", "Display the game version", print_version}
};

static const std::vector<ShellOptionsFlag> shell_options_flags {
	{"d", "debug", "Allow saving of core files", shell_options.debug},
	{"f", "fullscreen", "Run the game fullscreen", shell_options.force_fullscreen},
	{"w", "windowed", "Run the game in a window", shell_options.force_windowed},
	{"g", "nogl", "Do not use OpenGL", shell_options.nogl},
	{"s", "nosound", "Do not access the sound card", shell_options.nosound},
	{"m", "nogamma", "Disable gamma table effects (menu fades)", shell_options.nogamma},
	{"j", "nojoystick", "Do not initialize joysticks", shell_options.nojoystick},
	{"i", "insecure_lua", "", shell_options.insecure_lua},
	{"Q", "skip-intro", "Skip intro screens", shell_options.skip_intro},
	{"e", "editor", "Use editor prefs; jump directly to map", shell_options.editor}
};

static const std::vector<ShellOptionsString> shell_options_strings {
	{"o", "output", "With -e, output to [file] and exit on quit", shell_options.output},
	{"l", "replay-directory", "Directory with replays to load", shell_options.replay_directory},
	{"NSDocumentRevisionsDebugMode", "", "", ignore} // annoying Xcode argument
};

std::unordered_map<int, bool> ShellOptions::parse(int argc, char** argv, bool ignore_unknown_args)
{
	shell_options.program_name = argv[0];
	--argc;
	++argv;

    std::vector<std::string> args;
    while (argc > 0)
    {
        if (strncmp(*argv, "-C", 2) == 0)
        {
            args.push_back(*argv + 2);
        }
        else
        {
            args.push_back(*argv);
        }

        --argc;
        ++argv;
    }

	std::unordered_map<int, bool> results;

    for (int i = 0; i < args.size(); i++)
    {
		const auto& arg = args[i];
		bool found = false;

		for (auto command : shell_options_commands)
		{
			if (command.match(arg))
			{
				command.command();
				exit(0);
			}
		}

		for (auto flag : shell_options_flags)
		{
			if (flag.match(arg))
			{
				found = true;
				flag.flag = true;
				break;
			}
		}

		for (auto option : shell_options_strings)
		{
			if (option.match(arg))
			{
				if (i < args.size() - 1 && args[i + 1][0] != '-')
				{
					found = true;
					results.insert({ i++ + 1, true });
                    option.string = args[i];
                }
                else
                {
					logFatal("%s requires an additional argument", arg.c_str());
                    printf("%s requires an additional argument\n", arg.c_str());
                    print_usage();
                    exit(1);
                }
			}
		}

		if (!found)
		{
			if (arg[0] != '-')
			{
				FileSpecifier f(arg);
				if (f.Exists())
				{
					if (f.IsDir())
					{
						shell_options.directory = arg;
					}
					else
					{
						shell_options.files.push_back(arg);
					}

					found = true;
				}
			}

			if (!found && !ignore_unknown_args)
			{
				logFatal("Unrecognized argument '%s'.", arg.c_str());
				printf("Unrecognized argument '%s'.\n", arg.c_str());
				print_usage();
				exit(1);
			}
		}

		results.insert({ i + 1, found });
	}

	return results;
}

void print_version()
{
	char app_name_version[256];
	expand_app_variables(app_name_version, "Aleph Bet $appLongVersion$");
	std::cout << app_name_version << std::endl;
}

void print_usage()
{
	std::ostringstream oss;

#ifdef __WIN32__
	oss << "Command line switches:\n\n";
#else
	oss << "\nUsage: " << shell_options.program_name << " [options] [directory] [file]\n";
#endif

	for (auto command : shell_options_commands)
	{
		oss << command;
	}

	for (auto flag : shell_options_flags)
	{
		oss << flag;
	}

	for (auto option : shell_options_strings)
	{
		oss << option;
	}

	oss << "\tdirectory" << spaces(help_tab_stop - strlen("directory") - 8)
		<< "Directory containing scenario data files\n"
		
		<< "\tfile" << spaces(help_tab_stop - strlen("file") - 8)
		<< "Saved game to load or film to play\n"
		<< "\n"
		<< "You can also use the ALEPHBET_DATA environment variable to specify\n"
		<< "the data directory.\n";

#ifdef __WIN32__
	MessageBoxW(NULL, utf8_to_wide(oss.str()).c_str(), L"Usage", MB_OK | MB_ICONINFORMATION);
#else
	std::cout << oss.str();
#endif
}
