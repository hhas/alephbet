project('alephbet', ['c', 'cpp'],
  default_options: ['cpp_std=c++17'])

cpp = meson.get_compiler('cpp')

add_project_arguments('-DSDL', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_CURL', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_FFMPEG', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_MINIUPNPC', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_OPENGL', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_PNG', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_SDL_IMAGE', language: ['c', 'cpp'])
add_project_arguments('-DHAVE_ZZIP', language: ['c', 'cpp'])

if cpp.has_header('pwd.h')
    add_project_arguments('-DHAVE_PWD_H', language: ['c', 'cpp'])
endif

if cpp.has_header('unistd.h')
    add_project_arguments('-DHAVE_UNISTD_H', language: ['c', 'cpp'])
endif

if cpp.has_header_symbol('unistd.h', '_SC_NPROCESSORS_ONLN')
    # TODO: take this out when ffmpeg is removed
    add_project_arguments('-DHAVE_SYSCONF', language: ['c', 'cpp'])
endif

if cpp.has_header_symbol('stdlib.h', 'mkstemp')
    add_project_arguments('-DLUA_USE_MKSTEMP', language: ['c', 'cpp'])
endif

dsound_dep = dependency('', required: false)
wsock32_dep = dependency('', required: false)
if host_machine.system() in ['windows']
  dsound_dep = dependency('dsound')
  wsock32_dep = dependency('wsock32')
endif

thread_priority_file = 'Source_Files/Misc/thread_priority_sdl_dummy.cpp'
if host_machine.system() in ['darwin']
  thread_priority_file = 'Source_Files/Misc/thread_priority_sdl_macosx.cpp'
elif host_machine.system() in ['cygwin', 'windows']
  thread_priority_file = 'Source_Files/Misc/thread_priority_sdl_win32.cpp'
elif cpp.has_header_symbol('unistd.h', '_POSIX_PRIORITY_SCHEDULING')
  thread_priority_file = 'Source_Files/Misc/thread_priority_sdl_posix.cpp'
endif

boost_filesystem_dep = cpp.find_library('boost_filesystem')
gl_dep = dependency('gl')
glu_dep = dependency('glu')
libavcodec_dep = dependency('libavcodec')
libavutil_dep = dependency('libavutil')
libavformat_dep = dependency('libavformat')
libcurl_dep = dependency('libcurl')
libpng_dep = dependency('libpng')
libswresample_dep = dependency('libswresample')
libswscale_dep = dependency('libswscale')
sndfile_dep = dependency('sndfile')
m_dep = cpp.find_library('m', required: false)
# Always use the subproject for miniupnpc, because updates to it occasionally
# break the API.
miniupnpc_dep = subproject('miniupnpc').get_variable('miniupnpc_dep')
openal_dep = dependency('openal')
sdl2_dep = dependency('sdl2', allow_fallback: host_machine.system() in ['windows', 'darwin'])
sdl2_image_dep = dependency('sdl2_image')
sdl2_ttf_dep = dependency('sdl2_ttf')
sdl2_net_dep = dependency('sdl2_net')
zlib_dep = dependency('zlib')
zzip_dep = dependency('zziplib')

sources = files(
  'Source_Files/CSeries/BStream.cpp',
  'Source_Files/CSeries/byte_swapping.cpp',
  'Source_Files/CSeries/csalerts_sdl.cpp',
  'Source_Files/CSeries/cscluts_sdl.cpp',
  'Source_Files/CSeries/csdialogs_sdl.cpp',
  'Source_Files/CSeries/csmisc_sdl.cpp',
  'Source_Files/CSeries/cspaths_sdl.cpp',
  'Source_Files/CSeries/csstrings.cpp',
  'Source_Files/CSeries/FilmProfile.cpp',
  'Source_Files/CSeries/mytm_sdl.cpp',
  'Source_Files/FFmpeg/Movie.cpp',
  'Source_Files/FFmpeg/SDL_ffmpeg.c',
  'Source_Files/Files/AStream.cpp',
  'Source_Files/Files/crc.cpp',
  'Source_Files/Files/FileHandler.cpp',
  'Source_Files/Files/find_files_sdl.cpp',
  'Source_Files/Files/game_wad.cpp',
  'Source_Files/Files/import_definitions.cpp',
  'Source_Files/Files/Packing.cpp',
  'Source_Files/Files/preprocess_map_sdl.cpp',
  'Source_Files/Files/preprocess_map_shared.cpp',
  'Source_Files/Files/resource_manager.cpp',
  'Source_Files/Files/SDL_rwops_ostream.cpp',
  'Source_Files/Files/SDL_rwops_zzip.c',
  'Source_Files/Files/wad_prefs.cpp',
  'Source_Files/Files/wad_sdl.cpp',
  'Source_Files/Files/wad.cpp',
  'Source_Files/Files/WadImageCache.cpp',
  'Source_Files/GameWorld/devices.cpp',
  'Source_Files/GameWorld/dynamic_limits.cpp',
  'Source_Files/GameWorld/effects.cpp',
  'Source_Files/GameWorld/ephemera.cpp',
  'Source_Files/GameWorld/flood_map.cpp',
  'Source_Files/GameWorld/interpolated_world.cpp',
  'Source_Files/GameWorld/items.cpp',
  'Source_Files/GameWorld/lightsource.cpp',
  'Source_Files/GameWorld/map_constructors.cpp',
  'Source_Files/GameWorld/map.cpp',
  'Source_Files/GameWorld/marathon2.cpp',
  'Source_Files/GameWorld/media.cpp',
  'Source_Files/GameWorld/monsters.cpp',
  'Source_Files/GameWorld/pathfinding.cpp',
  'Source_Files/GameWorld/physics.cpp',
  'Source_Files/GameWorld/placement.cpp',
  'Source_Files/GameWorld/platforms.cpp',
  'Source_Files/GameWorld/player.cpp',
  'Source_Files/GameWorld/projectiles.cpp',
  'Source_Files/GameWorld/scenery.cpp',
  'Source_Files/GameWorld/weapons.cpp',
  'Source_Files/GameWorld/world.cpp',
  'Source_Files/Input/joystick_sdl.cpp',
  'Source_Files/Input/mouse_sdl.cpp',
  'Source_Files/Lua/lapi.c',
  'Source_Files/Lua/lauxlib.c',
  'Source_Files/Lua/lbaselib.c',
  'Source_Files/Lua/lbitlib.c',
  'Source_Files/Lua/lcode.c',
  'Source_Files/Lua/lctype.c',
  'Source_Files/Lua/ldblib.c',
  'Source_Files/Lua/ldebug.c',
  'Source_Files/Lua/ldo.c',
  'Source_Files/Lua/ldump.c',
  'Source_Files/Lua/lfunc.c',
  'Source_Files/Lua/lgc.c',
  'Source_Files/Lua/linit.c',
  'Source_Files/Lua/liolib.c',
  'Source_Files/Lua/llex.c',
  'Source_Files/Lua/lmathlib.c',
  'Source_Files/Lua/lmem.c',
  'Source_Files/Lua/lobject.c',
  'Source_Files/Lua/lopcodes.c',
  'Source_Files/Lua/loslib.c',
  'Source_Files/Lua/lparser.c',
  'Source_Files/Lua/lstate.c',
  'Source_Files/Lua/lstring.c',
  'Source_Files/Lua/lstrlib.c',
  'Source_Files/Lua/ltable.c',
  'Source_Files/Lua/ltablib.c',
  'Source_Files/Lua/ltm.c',
  'Source_Files/Lua/lua_ephemera.cpp',
  'Source_Files/Lua/lua_hud_objects.cpp',
  'Source_Files/Lua/lua_hud_script.cpp',
  'Source_Files/Lua/lua_map.cpp',
  'Source_Files/Lua/lua_monsters.cpp',
  'Source_Files/Lua/lua_music.cpp',
  'Source_Files/Lua/lua_objects.cpp',
  'Source_Files/Lua/lua_player.cpp',
  'Source_Files/Lua/lua_projectiles.cpp',
  'Source_Files/Lua/lua_saved_objects.cpp',
  'Source_Files/Lua/lua_script.cpp',
  'Source_Files/Lua/lua_serialize.cpp',
  'Source_Files/Lua/lundump.c',
  'Source_Files/Lua/lvm.c',
  'Source_Files/Lua/lzio.c',
  'Source_Files/main.cpp',
  'Source_Files/Misc/ActionQueues.cpp',
  'Source_Files/Misc/CircularByteBuffer.cpp',
  'Source_Files/Misc/Console.cpp',
  'Source_Files/Misc/DefaultStringSets.cpp',
  'Source_Files/Misc/game_errors.cpp',
  'Source_Files/Misc/interface.cpp',
  'Source_Files/Misc/Logging.cpp',
  'Source_Files/Misc/PlayerImage_sdl.cpp',
  'Source_Files/Misc/PlayerName.cpp',
  'Source_Files/Misc/preference_dialogs.cpp',
  'Source_Files/Misc/preferences_widgets_sdl.cpp',
  'Source_Files/Misc/preferences.cpp',
  'Source_Files/Misc/Scenario.cpp',
  'Source_Files/Misc/sdl_dialogs.cpp',
  'Source_Files/Misc/sdl_widgets.cpp',
  'Source_Files/Misc/shared_widgets.cpp',
  'Source_Files/Misc/Statistics.cpp',
  'Source_Files/Misc/vbl.cpp',
  'Source_Files/ModelView/Dim3_Loader.cpp',
  'Source_Files/ModelView/Model3D.cpp',
  'Source_Files/ModelView/ModelRenderer.cpp',
  'Source_Files/ModelView/StudioLoader.cpp',
  'Source_Files/ModelView/WavefrontLoader.cpp',
  'Source_Files/Network/ConnectPool.cpp',
  'Source_Files/Network/HTTP.cpp',
  'Source_Files/Network/Metaserver/metaserver_dialogs.cpp',
  'Source_Files/Network/Metaserver/metaserver_messages.cpp',
  'Source_Files/Network/Metaserver/network_metaserver.cpp',
  'Source_Files/Network/Metaserver/SdlMetaserverClientUi.cpp',
  'Source_Files/Network/network_capabilities.cpp',
  'Source_Files/Network/network_data_formats.cpp',
  'Source_Files/Network/network_dialog_widgets_sdl.cpp',
  'Source_Files/Network/network_dialogs.cpp',
  'Source_Files/Network/network_games.cpp',
  'Source_Files/Network/network_lookup_sdl.cpp',
  'Source_Files/Network/network_messages.cpp',
  'Source_Files/Network/network_star_hub.cpp',
  'Source_Files/Network/network_star_spoke.cpp',
  'Source_Files/Network/network_udp.cpp',
  'Source_Files/Network/network.cpp',
  'Source_Files/Network/Pinger.cpp',
  'Source_Files/Network/PortForward.cpp',
  'Source_Files/Network/RingGameProtocol.cpp',
  'Source_Files/Network/SDL_netx.cpp',
  'Source_Files/Network/SSLP_limited.cpp',
  'Source_Files/Network/StarGameProtocol.cpp',
  'Source_Files/Network/Update.cpp',
  'Source_Files/RenderMain/AnimatedTextures.cpp',
  'Source_Files/RenderMain/Crosshairs_SDL.cpp',
  'Source_Files/RenderMain/ImageLoader_SDL.cpp',
  'Source_Files/RenderMain/ImageLoader_Shared.cpp',
  'Source_Files/RenderMain/OGL_Faders.cpp',
  'Source_Files/RenderMain/OGL_FBO.cpp',
  'Source_Files/RenderMain/OGL_Model_Def.cpp',
  'Source_Files/RenderMain/OGL_Render.cpp',
  'Source_Files/RenderMain/OGL_Setup.cpp',
  'Source_Files/RenderMain/OGL_Shader.cpp',
  'Source_Files/RenderMain/OGL_Subst_Texture_Def.cpp',
  'Source_Files/RenderMain/OGL_Textures.cpp',
  'Source_Files/RenderMain/Rasterizer_Shader.cpp',
  'Source_Files/RenderMain/render.cpp',
  'Source_Files/RenderMain/RenderPlaceObjs.cpp',
  'Source_Files/RenderMain/RenderRasterize_Shader.cpp',
  'Source_Files/RenderMain/RenderRasterize.cpp',
  'Source_Files/RenderMain/RenderSortPoly.cpp',
  'Source_Files/RenderMain/RenderVisTree.cpp',
  'Source_Files/RenderMain/scottish_textures.cpp',
  'Source_Files/RenderMain/shapes.cpp',
  'Source_Files/RenderMain/SW_Texture_Extras.cpp',
  'Source_Files/RenderMain/textures.cpp',
  'Source_Files/RenderOther/ChaseCam.cpp',
  'Source_Files/RenderOther/computer_interface.cpp',
  'Source_Files/RenderOther/fades.cpp',
  'Source_Files/RenderOther/FontHandler.cpp',
  'Source_Files/RenderOther/game_window.cpp',
  'Source_Files/RenderOther/HUDRenderer_Lua.cpp',
  'Source_Files/RenderOther/HUDRenderer_OGL.cpp',
  'Source_Files/RenderOther/HUDRenderer_SW.cpp',
  'Source_Files/RenderOther/HUDRenderer.cpp',
  'Source_Files/RenderOther/Image_Blitter.cpp',
  'Source_Files/RenderOther/images.cpp',
  'Source_Files/RenderOther/IMG_savepng.c',
  'Source_Files/RenderOther/motion_sensor.cpp',
  'Source_Files/RenderOther/OGL_Blitter.cpp',
  'Source_Files/RenderOther/OGL_LoadScreen.cpp',
  'Source_Files/RenderOther/overhead_map.cpp',
  'Source_Files/RenderOther/OverheadMap_OGL.cpp',
  'Source_Files/RenderOther/OverheadMap_SDL.cpp',
  'Source_Files/RenderOther/OverheadMapRenderer.cpp',
  'Source_Files/RenderOther/screen_drawing.cpp',
  'Source_Files/RenderOther/screen.cpp',
  'Source_Files/RenderOther/sdl_fonts.cpp',
  'Source_Files/RenderOther/sdl_resize.cpp',
  'Source_Files/RenderOther/Shape_Blitter.cpp',
  'Source_Files/RenderOther/TextLayoutHelper.cpp',
  'Source_Files/RenderOther/TextStrings.cpp',
  'Source_Files/RenderOther/ViewControl.cpp',
  'Source_Files/shell_misc.cpp',
  'Source_Files/shell_options.cpp',
  'Source_Files/shell.cpp',
  'Source_Files/Sound/AudioPlayer.cpp',
  'Source_Files/Sound/Decoder.cpp',
  'Source_Files/Sound/FFmpegDecoder.cpp',
  'Source_Files/Sound/Music.cpp',
  'Source_Files/Sound/MusicPlayer.cpp',
  'Source_Files/Sound/OpenALManager.cpp',
  'Source_Files/Sound/ReplacementSounds.cpp',
  'Source_Files/Sound/SndfileDecoder.cpp',
  'Source_Files/Sound/SoundFile.cpp',
  'Source_Files/Sound/SoundManager.cpp',
  'Source_Files/Sound/SoundPlayer.cpp',
  'Source_Files/Sound/StreamPlayer.cpp',
  'Source_Files/TCPMess/CommunicationsChannel.cpp',
  'Source_Files/TCPMess/Message.cpp',
  'Source_Files/TCPMess/MessageDispatcher.cpp',
  'Source_Files/TCPMess/MessageHandler.cpp',
  'Source_Files/TCPMess/MessageInflater.cpp',
  'Source_Files/XML/InfoTree.cpp',
  'Source_Files/XML/Plugins.cpp',
  'Source_Files/XML/QuickSave.cpp',
  'Source_Files/XML/XML_LevelScript.cpp',
  'Source_Files/XML/XML_MakeRoot.cpp',
  thread_priority_file,
)
  
incdir = include_directories(
  'Source_Files',
  'Source_Files/CSeries',
  'Source_Files/FFmpeg',
  'Source_Files/Files',
  'Source_Files/GameWorld',
  'Source_Files/Input',
  'Source_Files/Lua',
  'Source_Files/Misc',
  'Source_Files/ModelView',
  'Source_Files/Network',
  'Source_Files/Network/Metaserver',
  'Source_Files/RenderMain',
  'Source_Files/RenderMain/Shaders',
  'Source_Files/RenderOther',
  'Source_Files/Sound',
  'Source_Files/TCPMess',
  'Source_Files/XML',
)

executable('alephbet',
  sources,
  win_subsystem: 'windows',
  include_directories: incdir,
  dependencies: [
    boost_filesystem_dep,
    dsound_dep,
    wsock32_dep,
    gl_dep,
    glu_dep,
    libavcodec_dep,
    libavutil_dep,
    libavformat_dep,
    libpng_dep,
    libswresample_dep,
    libswscale_dep,
    libcurl_dep,
    m_dep,
    miniupnpc_dep,
    openal_dep,
    sdl2_dep,
    sdl2_image_dep,
    sdl2_ttf_dep,
    sdl2_net_dep,
    sndfile_dep,
    zlib_dep,
    zzip_dep,
  ])
