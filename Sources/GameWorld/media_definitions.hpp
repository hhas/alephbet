#ifndef __MEDIA_DEFINITIONS_H
#define __MEDIA_DEFINITIONS_H

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

#include "SoundManagerEnums.hpp"
#include "effects.hpp"
#include "fades.hpp"
#include "media.hpp"

/* ---------- structures */

struct media_definition {
    int16 collection, shape, shape_count, shape_frequency;
    int16 transfer_mode;

    int16 damage_frequency; // mask&ticks
    struct damage_definition damage;

    int16 detonation_effects[NUMBER_OF_MEDIA_DETONATION_TYPES];
    int16 sounds[NUMBER_OF_MEDIA_SOUNDS];

    int16 submerged_fade_effect;
};

/* ---------- globals */

// LP addition: added JjaroGoo support (copy of sewage)
static struct media_definition media_definitions[NUMBER_OF_MEDIA_TYPES] = {
        /* _media_water */
        {
         _collection_walls1,                                    19,
         1,                                                                           0, /* collection, shape, shape_count, frequency */
                _xfer_normal, /* transfer mode */

                0, {NONE, 0, 0, 0, FIXED_ONE}, /* damage frequency and definition */

                {_effect_small_water_splash, _effect_medium_water_splash, _effect_large_water_splash,
                 _effect_large_water_emergence}, /* small, medium, large detonation effects */
                {NONE, NONE, _snd_enter_water, _snd_exit_water, _snd_walking_in_water, _ambient_snd_water,
                 _ambient_snd_under_media, _snd_enter_water, _snd_exit_water},

         _effect_under_water,                                                                                                                                                                                                                                                                                                           /* submerged fade effect */
        },

        /* _media_lava */
        {
         _collection_walls2,                                                      12,
         1, 0,                                                                              /* collection, shape, shape_count, frequency */
                _xfer_normal,                                                                                                        /* transfer mode */

                0xf,                                                                                             {_damage_lava, _alien_damage, 16, 0, FIXED_ONE},                                                                                                     /* damage frequency and definition */

                {_effect_small_lava_splash, _effect_medium_lava_splash, _effect_large_lava_splash,
                 _effect_large_lava_emergence},        /* small, medium, large detonation effects */
                {NONE, NONE, _snd_enter_lava, _snd_exit_lava, _snd_walking_in_lava, _ambient_snd_lava,
                 _ambient_snd_under_media, _snd_enter_lava, _snd_exit_lava},

         _effect_under_lava,                                                   /* submerged fade effect */
        },

        /* _media_goo */
        {
         _collection_walls5,                          5,
         1,                                                          0,                                 /* collection, shape, shape_count, frequency */
                _xfer_normal,                                                                                                           /* transfer mode */

                0x7,                                                                 {_damage_goo, _alien_damage, 8, 0, FIXED_ONE},                                                                /* damage frequency and definition */

                {_effect_small_goo_splash, _effect_medium_goo_splash, _effect_large_goo_splash,
                 _effect_large_goo_emergence},                                                                                            /* small, medium, large detonation effects */
                {NONE, NONE, _snd_enter_lava, _snd_exit_lava, _snd_walking_in_lava, _ambient_snd_goo,
                 _ambient_snd_under_media, _snd_enter_lava, _snd_exit_lava},

         _effect_under_goo,                                                                                                                                                                                                                                                 /* submerged fade effect */
        },

        /* _media_sewage */
        {
         _collection_walls3,                                     13,
         1,                                                                            0,                                                             /* collection, shape, shape_count, frequency */
                _xfer_normal,                                                  /* transfer mode */

                0,                                                                                                                             {NONE, 0, 0, 0, FIXED_ONE},                                                                           /* damage frequency and definition */

                {_effect_small_sewage_splash, _effect_medium_sewage_splash, _effect_large_sewage_splash,
                 _effect_large_sewage_emergence},                                                                                              /* small, medium, large detonation effects */
                {NONE, NONE, _snd_enter_sewage, _snd_exit_sewage, NONE, _ambient_snd_sewage, _ambient_snd_under_media,
                 _snd_enter_sewage, _snd_exit_sewage},

         _effect_under_sewage, /* submerged fade effect */
        },

        /* _media_jjaro */
        {
         _collection_walls4, 13,
         1,                           0,                                                                               /* collection, shape, shape_count, frequency */
                _xfer_normal,                                                                                          /* transfer mode */

                0,                                                                                                        {NONE, 0, 0, 0, FIXED_ONE},                                                                                                                                                         /* damage frequency and definition */

                {_effect_small_jjaro_splash, _effect_medium_jjaro_splash, _effect_large_jjaro_splash,
                 _effect_large_jjaro_emergence},                                                                                                                                                                            /* small, medium, large detonation effects */
                {NONE, NONE, _snd_enter_sewage, _snd_exit_sewage, NONE, _ambient_snd_sewage, _ambient_snd_under_media,
                 _snd_enter_sewage, _snd_exit_sewage},

         _effect_under_jjaro, /* submerged fade effect */
        },
};

#endif
