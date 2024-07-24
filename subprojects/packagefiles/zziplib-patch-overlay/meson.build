project('zziplib', 'c',
  version: '0.13.77',
  default_options : ['c_std=c99'],
)

cc = meson.get_compiler('cpp')

# NOTE: no large file support outside Windows
# that would need:
#add_project_arguments('-D_ZZIP_LARGEFILE', language: 'c')
#add_project_arguments('-D_FILE_OFFSET_BITS=64', language: 'c')
# as well as something to do with ZZIP_LARGEFILE_SENSITIVE

# TODO: ZZIP_HAVE_ALIGNED_ACCESS_REQUIRED

if host_machine.system() not in ['windows']
  conf_data = configuration_data()
  conf_data.set('_DEFAULT_SOURCE', true)
  if cc.has_header('ansidecl.h')
    conf_data.set('ZZIP_HAVE_ANSIDECL_H', true)
  endif
  if cc.has_header('byteswap.h')
    conf_data.set('ZZIP_HAVE_BYTESWAP_H', true)
  endif
  if cc.has_header('direct.h')
    conf_data.set('ZZIP_HAVE_DIRECT_H', true)
  endif
  if cc.has_header_symbol('dirent.h', 'DIR')
    conf_data.set('ZZIP_HAVE_DIRENT_H', true)
  endif
  if cc.has_header_symbol('ndir.h', 'DIR')
    conf_data.set('ZZIP_HAVE_NDIR_H', true)
  endif
  if cc.has_header_symbol('sys/dir.h', 'DIR')
    conf_data.set('ZZIP_HAVE_SYS_DIR_H', true)
  endif
  if cc.has_header_symbol('sys/ndir.h', 'DIR')
    conf_data.set('ZZIP_HAVE_SYS_NDIR_H', true)
  endif
  if cc.has_header('dlfcn.h')
    conf_data.set('ZZIP_HAVE_DLFCN_H', true)
  endif
  if cc.has_header('fnmatch.h')
    conf_data.set('ZZIP_HAVE_FNMATCH_H', true)
  endif
  if cc.has_header('inttypes.h')
    conf_data.set('ZZIP_HAVE_INTTYPES_H', true)
  endif
  if cc.has_header('io.h')
    conf_data.set('ZZIP_HAVE_IO_H', true)
  endif
  if cc.has_header('memory.h')
    conf_data.set('ZZIP_HAVE_MEMORY_H', true)
  endif
  if cc.has_header('stdint.h')
    conf_data.set('ZZIP_HAVE_STDINT_H', true)
  endif
  if cc.has_header('stdlib.h')
    conf_data.set('ZZIP_HAVE_STDLIB_H', true)
  endif
  if cc.has_header('string.h')
    conf_data.set('ZZIP_HAVE_STRING_H', true)
    if cc.has_header_symbol('string.h', 'strcasecmp')
      conf_data.set('ZZIP_HAVE_STRCASECMP', true)
    endif
    if cc.has_header_symbol('string.h', 'strndup')
      conf_data.set('ZZIP_HAVE_STRNDUP', true)
    endif
  endif
  if cc.has_header('strings.h')
    conf_data.set('ZZIP_HAVE_STRING_H', true)
    if cc.has_header_symbol('strings.h', 'strcasecmp')
      conf_data.set('ZZIP_HAVE_STRCASECMP', true)
    endif
    if cc.has_header_symbol('strings.h', 'strndup')
      conf_data.set('ZZIP_HAVE_STRNDUP', true)
    endif
  endif
  if cc.has_header('sys/int_types.h')
    conf_data.set('ZZIP_HAVE_SYS_INT_TYPES_H', true)
  endif
  if cc.has_header('sys/mman.h')
    conf_data.set('ZZIP_HAVE_SYS_MMAN_H', true)
  endif
  if cc.has_header('sys/param.h')
    conf_data.set('ZZIP_HAVE_SYS_PARAM_H', true)
  endif
  if cc.has_header('sys/stat.h')
    conf_data.set('ZZIP_HAVE_SYS_STAT_H', true)
  endif
  if cc.has_header('sys/types.h')
    conf_data.set('ZZIP_HAVE_SYS_TYPES_H', true)
  endif
  if cc.has_header('sys/int_types.h')
    conf_data.set('ZZIP_HAVE_SYS_INT_TYPES_H', true)
  endif
  if cc.has_header('unistd.h')
    conf_data.set('ZZIP_HAVE_UNISTD_H', true)
  endif
  if cc.has_header('winbase.h')
    conf_data.set('ZZIP_HAVE_WINBASE_H', true)
  endif
  if cc.has_header('windows.h')
    conf_data.set('ZZIP_HAVE_WINDOWS_H', true)
  endif
  if cc.has_header('winnt.h')
    conf_data.set('ZZIP_HAVE_WINNT_H', true)
  endif
  conf_data.set('ZZIP_HAVE_ZLIB_H', true)
  conf_data.set('ZZIP_SIZEOF_INT', cc.sizeof('int'))
  conf_data.set('ZZIP_SIZEOF_LONG', cc.sizeof('long'))
  conf_data.set('ZZIP_SIZEOF_SHORT', cc.sizeof('short'))
  if host_machine.endian() == 'big'
    conf_data.set('ZZIP_WORDS_BIGENDIAN', true)
  endif
  if not cc.has_header_symbol('sys/types.h', '__int64')
    conf_data.set('__int64', 'long long')
  endif
  conf_data.set('_zzip_inline', 'inline')
  conf_data.set('_zzip_off64_t', '_zzip_off_t')
  configure_file(
    input: 'real_zzip_config.h.in',
    output: 'real_zzip_config.h',
    configuration: conf_data,
  )
endif

zlib_dep = dependency('zlib')

sources = files(
  'zzip/dir.c',
  'zzip/err.c',
  'zzip/fetch.c',
  'zzip/file.c',
  'zzip/info.c',
  'zzip/plugin.c',
  'zzip/stat.c',
  'zzip/write.c',
  'zzip/zip.c',
)

zziplib = static_library('zziplib', sources,
  dependencies: [
    zlib_dep,
  ],
)

zziplib_dep = declare_dependency(
  link_with: zziplib,
  include_directories: [
    '.',
  ],
  dependencies: [
    zlib_dep,
  ],
)
