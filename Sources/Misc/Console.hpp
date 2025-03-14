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

#ifndef CONSOLE_H
#define CONSOLE_H

#include "preferences.hpp"
#include <functional>
#include <map>
#include <string>

class CommandParser {
  public:

    CommandParser() {}

    virtual ~CommandParser() = default;
    void register_command(std::string command, std::function<void(const std::string&)> f);
    void register_command(std::string command, const CommandParser& command_parser);
    void unregister_command(std::string command);

    virtual void parse_and_execute(const std::string& command_string);

  private:

    typedef std::map<std::string, std::function<void(const std::string&)>> command_map;
    command_map m_commands;
};

class Console : public CommandParser {
  public:

    static Console* instance();

    // called by key handlers
    void enter();
    void abort(); // callback is called with empty string
    void del();
    void backspace();
    void clear();
    void forward_clear();
    void transpose();
    void delete_word();
    void textEvent(const SDL_Event& e);
    void up_arrow();
    void down_arrow();
    void left_arrow();
    void right_arrow();
    void line_home();
    void line_end();

    const std::string& displayBuffer() { return m_displayBuffer; }

    void activate_input(std::function<void(const std::string&)> callback, const std::string& prompt);
    void deactivate_input(); // like abort, but no callback

    bool input_active() { return m_active; }

    int cursor_position();

    void register_macro(std::string macro, std::string replacement);
    void unregister_macro(std::string macro);
    void clear_macros();

    // carnage reporting
    void set_carnage_message(int16 projectile_type, const std::string& on_kill, const std::string& on_suicide = "");
    void report_kill(int16 player_index, int16 aggressor_player_index, int16 projectile_index);
    void clear_carnage_messages();

    bool use_lua_console() { return m_use_lua_console || environment_preferences->use_solo_lua; };

    void use_lua_console(bool f_use) { m_use_lua_console = f_use; }

    // clear last saved level name
    void clear_saves();

  private:

    Console();

    std::function<void(std::string)> m_callback;
    std::string m_buffer;
    std::string m_displayBuffer;
    std::string m_prompt;
    bool m_active;

    std::vector<std::string> m_prev_commands;
    std::vector<std::string>::iterator m_command_iter;
    void set_command(std::string command);

    int m_cursor_position;

    std::map<std::string, std::string> m_macros;

    bool m_carnage_messages_exist;
    std::vector<std::pair<std::string, std::string>> m_carnage_messages;

    bool m_use_lua_console;

    void register_save_commands();
};

class InfoTree;
void parse_mml_console(const InfoTree& root);
void reset_mml_console();

#endif
