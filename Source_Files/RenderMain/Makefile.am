## Process this file with automake to produce Makefile.in 

# Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers, and
# the Aleph Bet developers.
#
# Aleph Bet is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# Aleph Bet is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <https://www.gnu.org/licenses/>.
#
# This license notice applies only to the Aleph Bet engine itself, and does
# not apply to Marathon, Marathon 2, or Marathon Infinity scenarios and
# assets, nor to elements of any third-party scenarios.

noinst_LIBRARIES = librendermain.a

if MAKE_OPENGL
OPENGL_SOURCES = Rasterizer_Shader.cpp RenderRasterize_Shader.cpp
else
OPENGL_SOURCES = 
endif

librendermain_a_SOURCES = AnimatedTextures.h collection_definition.h		   \
  Crosshairs.h DDS.h ImageLoader.h low_level_textures.h OGL_Faders.h		   \
  OGL_Headers.h OGL_Model_Def.h OGL_Render.h OGL_Setup.h OGL_FBO.h			   \
  OGL_Subst_Texture_Def.h OGL_Texture_Def.h OGL_Textures.h Rasterizer.h		   \
  Rasterizer_OGL.h Rasterizer_Shader.h Rasterizer_SW.h render.h				   \
  RenderPlaceObjs.h RenderRasterize.h RenderRasterize_Shader.h				   \
  RenderSortPoly.h RenderVisTree.h scottish_textures.h shape_definitions.h	   \
  shape_descriptors.h SW_Texture_Extras.h textures.h OGL_Shader.h vec3.h	   \
  Shaders/bump_bloom.frag Shaders/bump.frag Shaders/invincible_bloom.frag	   \
  Shaders/invincible.frag Shaders/invisible_bloom.frag Shaders/invisible.frag  \
  Shaders/landscape_bloom.frag Shaders/landscape.frag						   \
  Shaders/landscape_infravision.frag Shaders/landscape.vert					   \
  Shaders/sprite_bloom.frag Shaders/sprite.frag								   \
  Shaders/sprite_infravision.frag Shaders/sprite.vert Shaders/wall_bloom.frag  \
  Shaders/wall.frag Shaders/wall_infravision.frag Shaders/wall.vert			   \
  AnimatedTextures.cpp Crosshairs_SDL.cpp ImageLoader_Shared.cpp			   \
  ImageLoader_SDL.cpp OGL_Faders.cpp OGL_Model_Def.cpp OGL_Render.cpp		   \
  OGL_Setup.cpp OGL_Subst_Texture_Def.cpp OGL_Textures.cpp render.cpp		   \
  RenderPlaceObjs.cpp $(OPENGL_SOURCES) RenderRasterize.cpp RenderSortPoly.cpp \
  RenderVisTree.cpp scottish_textures.cpp shapes.cpp SW_Texture_Extras.cpp	   \
  textures.cpp OGL_Shader.cpp OGL_FBO.cpp

EXTRA_librendermain_a_SOURCES = Rasterizer_Shader.cpp	\
RenderRasterize_Shader.cpp

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/Files \
  -I$(top_srcdir)/Source_Files/GameWorld -I$(top_srcdir)/Source_Files/Input \
  -I$(top_srcdir)/Source_Files/Misc -I$(top_srcdir)/Source_Files/ModelView \
  -I$(top_srcdir)/Source_Files/Network \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/XML -I$(top_srcdir)/Source_Files
