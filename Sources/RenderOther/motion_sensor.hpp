#ifndef __MOTION_SENSOR_H
#define __MOTION_SENSOR_H

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

#include "shape_descriptors.hpp"

enum {
    MType_Friend, // What you, friendly players, and the Bobs are
    MType_Alien,  // What the other critters are
    MType_Enemy,  // What hostile players are
    NUMBER_OF_MDISPTYPES
};

/* ---------- prototypes/MOTION_SENSOR.C */

void initialize_motion_sensor(shape_descriptor mount, shape_descriptor virgin_mounts, shape_descriptor alien,
                              shape_descriptor _friend, shape_descriptor enemy, shape_descriptor network_compass,
                              short side_length);
void reset_motion_sensor(short monster_index);
void motion_sensor_scan(void);
bool motion_sensor_has_changed(void);
void adjust_motion_sensor_range(void);

class InfoTree;
void parse_mml_motion_sensor(const InfoTree& root);
void reset_mml_motion_sensor();

#endif
