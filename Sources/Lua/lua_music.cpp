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

#include "lua_music.hpp"
#include "Music.hpp"

static int Lua_MusicManager_Clear(lua_State* L) {
    Music::instance()->ClearLevelMusic();
    return 0;
}

static int Lua_MusicManager_Fade(lua_State* L) {
    int duration = lua_isnumber(L, 1) ? static_cast<int>(lua_tonumber(L, 1) * 1000) : 1000;
    Music::instance()->Fade(0, duration);
    Music::instance()->ClearLevelMusic();
    return 0;
}

static int Lua_Music_Fade(lua_State* L) {
    if (!lua_isnumber(L, 2))
        return luaL_error(L, "volume: incorrect argument type");

    float limitVolume   = static_cast<float>(lua_tonumber(L, 2));
    int duration        = lua_isnumber(L, 3) ? static_cast<int>(lua_tonumber(L, 3) * 1000) : 1000;
    bool stopOnNoVolume = lua_isboolean(L, 4) ? static_cast<bool>(lua_toboolean(L, 4)) : true;

    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    Music::instance()->Fade(limitVolume, duration, stopOnNoVolume, index);
    return 0;
}

static int Lua_Music_Play(lua_State* L) {
    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    Music::instance()->Play(index);
    return 0;
}

static int Lua_Music_Stop(lua_State* L) {
    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    Music::instance()->Pause(index);
    return 0;
}

static int Lua_Music_Active_Get(lua_State* L) {
    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    lua_pushboolean(L, Music::instance()->Playing(index));
    return 1;
}

static int Lua_Music_Volume_Get(lua_State* L) {
    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    lua_pushnumber(L, static_cast<double>(Music::instance()->GetVolume(index)));
    return 1;
}

static int Lua_Music_Volume_Set(lua_State* L) {
    if (!lua_isnumber(L, 2))
        return luaL_error(L, "volume: incorrect argument type");

    int index = Lua_Music::Index(L, 1) + Music::reserved_music_slots;
    Music::instance()->SetVolume(index, static_cast<float>(lua_tonumber(L, 2)));
    return 0;
}

static bool Lua_Music_Valid(int16 index) {
    return index >= 0 && Music::instance()->IsInit(index + Music::reserved_music_slots);
}

static int Lua_MusicManager_New(lua_State* L) {
    if (!lua_isstring(L, 1))
        return luaL_error(L, "play: invalid file specifier");

    float volume = lua_isnumber(L, 2) ? static_cast<float>(lua_tonumber(L, 2)) : 1;
    bool loop    = lua_isboolean(L, 3) ? static_cast<bool>(lua_toboolean(L, 3)) : true;

    std::string search_path = L_Get_Search_Path(L);

    FileSpecifier file;
    if (search_path.size()) {
        if (!file.SetNameWithPath(lua_tostring(L, 1), search_path))
            return 0;
    } else {
        if (!file.SetNameWithPath(lua_tostring(L, 1)))
            return 0;
    }

    int id = Music::instance()->Load(file, loop, volume);
    if (id < Music::reserved_music_slots)
        return 0;

    Lua_Music::Push(L, id - Music::reserved_music_slots);
    return 1;
}

static int Lua_MusicManager_Play(lua_State* L) {
    for (int n = 1; n <= lua_gettop(L); n++) {
        if (!lua_isstring(L, n))
            return luaL_error(L, "play: invalid file specifier");

        std::string search_path = L_Get_Search_Path(L);

        FileSpecifier file;
        if (search_path.size()) {
            if (file.SetNameWithPath(lua_tostring(L, n), search_path))
                Music::instance()->PushBackLevelMusic(file);
        } else {
            if (file.SetNameWithPath(lua_tostring(L, n)))
                Music::instance()->PushBackLevelMusic(file);
        }
    }

    return 0;
}

static int Lua_MusicManager_Stop(lua_State* L) {
    Music::instance()->ClearLevelMusic();
    Music::instance()->StopLevelMusic();

    return 0;
}

static int Lua_MusicManager_Valid(lua_State* L) {
    int top = lua_gettop(L);
    for (int n = 1; n <= top; n++) {
        if (!lua_isstring(L, n))
            return luaL_error(L, "valid: invalid file specifier");
        FileSpecifier path;

        bool found;
        auto search_path = L_Get_Search_Path(L);
        if (search_path.size()) {
            found = path.SetNameWithPath(lua_tostring(L, n), search_path);
        } else {
            found = path.SetNameWithPath(lua_tostring(L, n));
        }

        lua_pushboolean(L, found && StreamDecoder::Get(path));
    }
    return top;
}

const luaL_Reg Lua_MusicManager_Methods[] = {
        {"new",   L_TableFunction<Lua_MusicManager_New>  },
        {"clear", L_TableFunction<Lua_MusicManager_Clear>},
        {"fade",  L_TableFunction<Lua_MusicManager_Fade> },
        {"play",  L_TableFunction<Lua_MusicManager_Play> },
        {"stop",  L_TableFunction<Lua_MusicManager_Stop> },
        {"valid", L_TableFunction<Lua_MusicManager_Valid>},
        {0,       0                                      }
};

const luaL_Reg Lua_Music_Get[] = {
        {"fade",   L_TableFunction<Lua_Music_Fade>},
        {"volume", Lua_Music_Volume_Get           },
        {"active", Lua_Music_Active_Get           },
        {"play",   L_TableFunction<Lua_Music_Play>},
        {"stop",   L_TableFunction<Lua_Music_Stop>},
        {0,        0                              }
};

const luaL_Reg Lua_Music_Set[] = {
        {"volume", Lua_Music_Volume_Set},
        {0,        0                   }
};

char Lua_Music_Name[]        = "music";
char Lua_MusicManager_Name[] = "Music";

int Lua_Music_register(lua_State* L) {
    Lua_Music::Register(L, Lua_Music_Get, Lua_Music_Set);
    Lua_Music::Valid = Lua_Music_Valid;

    Lua_MusicManager::Register(L, Lua_MusicManager_Methods);
    Lua_MusicManager::Length = Lua_MusicManager::ConstantLength(16); // whatever
    return 0;
}
