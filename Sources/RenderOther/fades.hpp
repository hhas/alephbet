#ifndef __FADES_H
#define __FADES_H

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

/* ---------- constants */

enum {
    NUMBER_OF_GAMMA_LEVELS = 8,
    DEFAULT_GAMMA_LEVEL    = 2
};

enum /* fade types */
{
    _start_cinematic_fade_in, /* force all colors to black immediately */
    _cinematic_fade_in,       /* fade in from black */
    _long_cinematic_fade_in,
    _cinematic_fade_out,     /* fade out from black */
    _end_cinematic_fade_out, /* force all colors from black immediately */

    _fade_red,              /* bullets and fist */
    _fade_big_red,          /* bigger bullets and fists */
    _fade_bonus,            /* picking up items */
    _fade_bright,           /* teleporting */
    _fade_long_bright,      /* nuclear monster detonations */
    _fade_yellow,           /* explosions */
    _fade_big_yellow,       /* big explosions */
    _fade_purple,           /* ? */
    _fade_cyan,             /* fighter staves and projectiles */
    _fade_white,            /* absorbed */
    _fade_big_white,        /* rocket (probably) absorbed */
    _fade_orange,           /* flamethrower */
    _fade_long_orange,      /* marathon lava */
    _fade_green,            /* hunter projectile */
    _fade_long_green,       /* alien green goo */
    _fade_static,           /* compiler projectile */
    _fade_negative,         /* minor fusion projectile */
    _fade_big_negative,     /* major fusion projectile */
    _fade_flicker_negative, /* hummer projectile */
    _fade_dodge_purple,     /* alien weapon */
    _fade_burn_cyan,        /* armageddon beast electricity */
    _fade_dodge_yellow,     /* armageddon beast projectile */
    _fade_burn_green,       /* hunter projectile */

    _fade_tint_green,                      /* under goo */
    _fade_tint_blue,                       /* under water */
    _fade_tint_orange,                     /* under lava */
    _fade_tint_gross,                      /* under sewage */
    _fade_tint_jjaro, /* under JjaroGoo */ // LP addition

    NUMBER_OF_FADE_TYPES
};

// LP change: rearranged to get order: water, lava, sewage, jjaro, pfhor
enum /* effect types */
{
    _effect_under_water,
    _effect_under_lava,
    _effect_under_sewage,
    _effect_under_jjaro,
    _effect_under_goo,
    NUMBER_OF_FADE_EFFECT_TYPES
};

// LP addition, since XML does not support direct specification of callbacks
// very well.
// Moved out here for the convenience of OpenGL fader implementations.
enum {
    _tint_fader_type,
    _randomize_fader_type,
    _negate_fader_type,
    _dodge_fader_type,
    _burn_fader_type,
    _soft_tint_fader_type,
    NUMBER_OF_FADER_FUNCTIONS
};

/* ---------- prototypes/FADES.C */

void initialize_fades(void);
bool update_fades(bool game_in_progress = false);

void start_fade(short type);
void stop_fade(void);
bool fade_finished(void);

void set_fade_effect(short type);

void gamma_correct_color_table(struct color_table* uncorrected_color_table, struct color_table* corrected_color_table,
                               short gamma_level);
float get_actual_gamma_adjust(short gamma_level);

void explicit_start_fade(short type, struct color_table* original_color_table, struct color_table* animated_color_table,
                         bool game_in_progress = false);
void full_fade(short type, struct color_table* original_color_table);

// see if the screen was set to black by the last fade
bool fade_blacked_screen(void);

// LP: sets the number of calls of set_fade_effect() that get ignored;
// this is a workaround for a MacOS-version bug where something gets painted on the screen
// after certain dialog boxes are cleared, thus canceling out the fader effect.
void SetFadeEffectDelay(int _FadeEffectDelay);

class InfoTree;
void parse_mml_faders(const InfoTree& root);
void reset_mml_faders();

#endif
