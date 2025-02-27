#ifndef STUDIO_LOADER
#define STUDIO_LOADER

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

#include <stdio.h>
#include "Model3D.hpp"
#include "FileHandler.hpp"

// Load a 3D Studio MAX model, without converting its coordinate system.
bool LoadModel_Studio(FileSpecifier& Spec, Model3D& Model);

// Load a 3D Studio MAX model, and convert its vertex and texture coordinates
// from its right-handed coordinate system to Aleph Bet's left-handed system.
bool LoadModel_Studio_RightHand(FileSpecifier& Spec, Model3D& Model);

#endif
