#ifndef PLUGINS_H
#define PLUGINS_H

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
 *  Plugins.h - a plugin manager
 */

#include <filesystem>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "FileHandler.hpp"

struct ScenarioInfo {
    std::string name;
    std::string scenario_id;
    std::string version;
};

struct ShapesPatch {
    bool requires_opengl;
    std::string path;
};

struct MapPatch {
    std::set<uint32_t> parent_checksums;
    using resource_key_t = std::pair<uint32_t, int>;
    std::map<resource_key_t, std::string> resource_map;
};

struct Plugin {
    DirectorySpecifier directory;
    std::string name;
    std::string description;
    std::string version;
    std::vector<std::string> mmls;
    std::string hud_lua;
    std::string solo_lua;
    std::string stats_lua;
    std::string theme;
    std::string required_version;
    std::vector<ShapesPatch> shapes_patches;
    std::vector<ScenarioInfo> required_scenarios;
    std::vector<MapPatch> map_patches;

    bool auto_enable;
    bool enabled;
    bool overridden;
    bool overridden_solo;
    bool compatible() const;
    bool allowed() const;
    bool valid() const;

    bool operator<(const Plugin& other) const { return name < other.name; }

    bool get_resource(uint32_t checksum, uint32_t type, int id, LoadedResource& rsrc) const;
};

class Plugins {
    friend class PluginLoader;

  public:

    static Plugins* instance();
    typedef std::vector<Plugin>::iterator iterator;

    enum GameMode {
        kMode_Menu,
        kMode_Solo,
        kMode_Net
    };

    void enumerate();

    void invalidate() { m_validated = false; }

    void set_mode(GameMode mode) { m_mode = mode; }

    GameMode mode() { return m_mode; }

    void load_mml(bool load_menu_mml_only);

    void load_shapes_patches(bool opengl);

    bool disable(const std::filesystem::path& path);
    bool enable(const std::filesystem::path& path);

    iterator begin() { return m_plugins.begin(); }

    iterator end() { return m_plugins.end(); }

    const Plugin* find_hud_lua();
    const Plugin* find_solo_lua();
    const Plugin* find_stats_lua();
    const Plugin* find_theme();

    bool get_resource(uint32_t type, int id, LoadedResource& rsrc);
    void set_map_checksum(uint32_t checksum);

  private:

    Plugins() {}

    void add(const Plugin& plugin) { m_plugins.push_back(plugin); }

    void validate();

    std::vector<Plugin> m_plugins;
    bool m_validated = false;
    GameMode m_mode  = kMode_Menu;

    std::stack<ScopedSearchPath, std::list<ScopedSearchPath>> m_search_paths;

    uint32_t m_map_checksum;
};


#endif
