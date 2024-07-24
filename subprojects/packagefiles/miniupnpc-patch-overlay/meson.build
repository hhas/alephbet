project('miniupnpc', 'c',
  version: '2.3.7',
)

cc = meson.get_compiler('cpp')

add_project_arguments('-DMINIUPNPC_SET_SOCKET_TIMEOUT', language: 'c')
add_project_arguments('-DMINIUPNPC_GET_SRC_ADDR', language: 'c')
add_project_arguments('-D_BSD_SOURCE', language: 'c')
add_project_arguments('-D_DEFAULT_SOURCE', language: 'c')
if host_machine.system() in ['windows']
  add_project_arguments('-DNDEBUG', language: 'c')
  add_project_arguments('-D_WIN32_WINNT=0x501', language: 'c')
endif
if host_machine.system() in ['netbsd']
  add_project_arguments('-D_NETBSD_SOURCE', language: 'c')
endif
if host_machine.system() in ['freebsd', 'darwin']
  add_project_arguments('-D_XOPEN_SOURCE=600', language: 'c')
endif

resolv_dep = dependency('', required: false)
nsl_dep = dependency('', required: false)
socket_dep = dependency('', required: false)
if host_machine.system() in ['sunos']
  resolv_dep = cc.find_library('resolv')
  nsl_dep = cc.find_library('nsl')
  socket_dep = cc.find_library('socket')
endif

ws2_32_dep = dependency('', required: false)
iphlpapi_dep = dependency('', required: false)
if host_machine.system() in ['windows']
  ws2_32_dep = cc.find_library('ws2_32')
  iphlpapi_dep = cc.find_library('iphlpapi')
endif

if cc.has_argument('-fno-common')
  add_project_arguments('-fno-common', language: 'c')
endif

sources = files(
  'src/addr_is_reserved.c',
  'src/connecthostport.c',
  'src/igd_desc_parse.c',
  'src/minisoap.c',
  'src/minissdpc.c',
  'src/miniupnpc.c',
  'src/miniwget.c',
  'src/minixml.c',
  'src/portlistingparse.c',
  'src/receivedata.c',
  'src/upnpcommands.c',
  'src/upnpdev.c',
  'src/upnperrors.c',
  'src/upnpreplyparse.c',
)

miniupnpc = static_library('miniupnpc', sources,
  include_directories: [
    'include',
  ],
  dependencies: [
    iphlpapi_dep,
    nsl_dep,
    resolv_dep,
    socket_dep,
    ws2_32_dep,
  ],
)

miniupnpc_dep = declare_dependency(
  link_with: miniupnpc,
  include_directories: [
    'include',
  ],
  dependencies: [
    iphlpapi_dep,
    nsl_dep,
    resolv_dep,
    socket_dep,
    ws2_32_dep,
  ],
)
