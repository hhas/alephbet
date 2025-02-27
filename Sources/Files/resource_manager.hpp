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
 *  resource_manager.h - MacOS resource handling for non-Mac platforms
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "cstypes.hpp"
#include <SDL.h>
#include <stdio.h>
#include <vector>

#ifndef NO_STD_NAMESPACE
using std::vector;
#endif

class FileSpecifier;
class LoadedResource;

extern void initialize_resources(void);

extern SDL_RWops* open_res_file(FileSpecifier& file);
extern void close_res_file(SDL_RWops* file);
extern SDL_RWops* open_res_file_from_rwops(SDL_RWops* file);
extern SDL_RWops* cur_res_file(void);
extern void use_res_file(SDL_RWops* file);

extern size_t count_1_resources(uint32 type);
extern size_t count_resources(uint32 type);

extern void get_1_resource_id_list(uint32 type, vector<int>& ids);
extern void get_resource_id_list(uint32 type, vector<int>& ids);

extern bool get_1_resource(uint32 type, int id, LoadedResource& rsrc);
extern bool get_resource(uint32 type, int id, LoadedResource& rsrc);

extern bool get_1_ind_resource(uint32 type, int index, LoadedResource& rsrc);
extern bool get_ind_resource(uint32 type, int index, LoadedResource& rsrc);

extern bool has_1_resource(uint32 type, int id);
extern bool has_resource(uint32 type, int id);

extern void set_external_resources_file(FileSpecifier&);
extern void close_external_resources();

#endif
