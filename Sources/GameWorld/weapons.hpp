#ifndef __WEAPONS_H
#define __WEAPONS_H

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

#include "cstypes.hpp"

/* enums for player.c */
enum { /* Weapons */
    _weapon_fist,
    _weapon_pistol,
    _weapon_plasma_pistol,
    _weapon_assault_rifle,
    _weapon_missile_launcher,
    _weapon_flamethrower,
    _weapon_alien_shotgun,
    _weapon_shotgun,
    _weapon_ball, // or something
    // LP addition:
    _weapon_smg,
    MAXIMUM_NUMBER_OF_WEAPONS,

    _weapon_doublefisted_pistols = MAXIMUM_NUMBER_OF_WEAPONS, /* This is a pseudo-weapon */
    _weapon_doublefisted_shotguns,
    PLAYER_TORSO_SHAPE_COUNT
};

enum {
    _shape_weapon_idle,
    _shape_weapon_charging,
    _shape_weapon_firing,
    PLAYER_TORSO_WEAPON_ACTION_COUNT // ZZZ: added this one
};

enum {
    _primary_weapon,
    _secondary_weapon,
    NUMBER_OF_TRIGGERS
};

enum /* weapon display positioning modes */
{
    _position_low,    /* position==0 is invisible, position==FIXED_ONE is sticking out from left/bottom */
    _position_center, /* position==0 is off left/bottom, position==FIXED_ONE is off top/right */
    _position_high    /* position==0 is invisible, position==FIXED_ONE is sticking out from right/top
           (mirrored, whether you like it or not) */
};

/* ----------------- structures */

struct weapon_display_information {
    // Has sequence info for 3D-model weapon display
    short collection, shape_index, low_level_shape_index;

    _fixed vertical_position, horizontal_position;
    short vertical_positioning_mode, horizontal_positioning_mode;
    short transfer_mode;
    _fixed transfer_phase;

    bool flip_horizontal, flip_vertical;

    // Needed for animated models: which frame in an individual sequence (0, 1, 2, ...)
    short Frame, NextFrame;

    // Needed for animated models: which tick in a frame, and total ticks per frame
    short Phase, Ticks;

    // type in bits 0-1 and shell casing id in bits 4-7
    short interpolation_data;
};

// SB: This needs to be accessed in lua_script.cpp

enum {
    MAXIMUM_SHELL_CASINGS = 4
};

struct trigger_data {
    short state, phase;
    short rounds_loaded;
    short shots_fired, shots_hit;
    short ticks_since_last_shot; /* used to play shell casing sound, and to calculate arc for shell casing drawing... */
    short ticks_firing;          /* How long have we been firing? (only valid for automatics) */
    uint16 sequence;             /* what step of the animation are we in? (NOT guaranteed to be in sync!) */
};

struct weapon_data {
    short weapon_type; /* stored here to make life easier.. */
    uint16 flags;
    uint16 unused; /* non zero-> weapon is powered up */
    struct trigger_data triggers[NUMBER_OF_TRIGGERS];
};

struct shell_casing_data {
    short type;
    short frame;

    uint16 flags;

    _fixed x, y;
    _fixed vx, vy;
};

struct player_weapon_data {
    short current_weapon;
    short desired_weapon;
    struct weapon_data weapons[MAXIMUM_NUMBER_OF_WEAPONS];
    struct shell_casing_data shell_casings[MAXIMUM_SHELL_CASINGS];
};

// For external access:
const int SIZEOF_weapon_definition = 134;

const int SIZEOF_player_weapon_data = 472;

/* ----------------- prototypes */
/* called once at startup */
void initialize_weapon_manager(void);

/* Initialize the weapons for a completely new game. */
void initialize_player_weapons_for_new_game(short player_index);

/* initialize the given players weapons-> called after creating a player */
void initialize_player_weapons(short player_index);

// Old external-access stuff: superseded by the packing and unpacking routines below
void* get_weapon_array(void);
int32 calculate_weapon_array_length(void);

/* while this returns true, keep calling.. */
bool get_weapon_display_information(short* count, struct weapon_display_information* data);

/* When the player runs over an item, check for reloads, etc. */
void process_new_item_for_reloading(short player_index, short item_type);

/* Update the given player's weapons */
void update_player_weapons(short player_index, uint32 action_flags);

/* Mark the weapon collections for loading or unloading.. */
void mark_weapon_collections(bool loading);

/* Called when a player dies to discharge the weapons that they have charged up. */
void discharge_charged_weapons(short player_index);

/* Called on entry to a level, and will change weapons if this one doesn't work */
/*  in the given environment. */
void check_player_weapons_for_environment_change(void);

/* Tell me when one of my projectiles hits, and return the weapon_identifier I passed */
/*  to new_projectile... */
void player_hit_target(short player_index, short weapon_identifier);

/* for drawing the player */
void get_player_weapon_mode_and_type(short player_index, short* shape_weapon_type, short* shape_mode);

/* For the game window to update properly */
short get_player_desired_weapon(short player_index);

/* This is pinned to the maximum I think I can hold.. */
short get_player_weapon_ammo_count(short player_index, short which_weapon, short which_trigger);

short get_player_weapon_ammo_maximum(short player_index, short which_weapon, short which_trigger);
int16 get_player_weapon_ammo_type(short player_index, short which_weapon, short which_trigger);
bool get_player_weapon_drawn(short player_index, short which_weapon, short which_trigger);

#ifdef DEBUG
void debug_print_weapon_status(void);
#endif

// LP: to pack and unpack this data;
// these do not make the definitions visible to the outside world

uint8* unpack_player_weapon_data(uint8* Stream, size_t Count);
uint8* pack_player_weapon_data(uint8* Stream, size_t Count);
uint8* unpack_weapon_definition(uint8* Stream, size_t Count);
uint8* pack_weapon_definition(uint8* Stream, size_t Count);
uint8* unpack_m1_weapon_definition(uint8* Stream, size_t Count);
void init_weapon_definitions();

// LP additions: get number of weapon types;
size_t get_number_of_weapon_types();

class InfoTree;
void parse_mml_weapons(const InfoTree& root);
void reset_mml_weapons();

#endif
