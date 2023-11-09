project(
  'libultrahdr-wasm',
  'cpp',
  version: '1.0.0',
  meson_version: '>=0.63.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++20'
    ],
)

cargs = []

add_global_arguments('-DUSE_BIG_ENDIAN_IN_ICC=false', language: 'cpp')
add_global_arguments('-DUSE_BIG_ENDIAN_IN_MPF=false', language: 'cpp')

deps = [
  dependency('libturbojpeg', disabler: true, required: true, static: true)
]

subdir('third_party/image_io')

sources = files(
  'lib/gainmapmath.cpp',
  'lib/icc.cpp',
  'lib/jpegr.cpp',
  'lib/jpegrutils.cpp',
  'lib/jpegencoderhelper.cpp',
  'lib/jpegdecoderhelper.cpp',
  'lib/multipictureformat.cpp'
)

sources += image_io_lib

lib_ultra_hdr_inc = include_directories(
  'lib'
)

libultrahdr_lib = static_library(
  'libultrahdr',
  sources,
  include_directories: [image_io_inc, lib_ultra_hdr_inc],
  dependencies: deps,
  c_args: cargs,
  install: false
)

libultrahdr_dep = declare_dependency(
  link_with: libultrahdr_lib,
)

link_args = [
  '-sALLOW_MEMORY_GROWTH',
  '--bind',
  '--embind-emit-tsd=libultrahdr.d.ts'
]

# release

executable(
  'libultrahdr',
  'libultrahdr-wasm.cpp',
  dependencies: [deps, libultrahdr_dep],
  include_directories: lib_ultra_hdr_inc,
  link_args: link_args
)

# release esm

executable(
  'libultrahdr-esm',
  'libultrahdr-wasm.cpp',
  dependencies: [deps, libultrahdr_dep],
  include_directories: lib_ultra_hdr_inc,
  link_args: [
    link_args,
    '-sMODULARIZE',
    '-sUSE_ES6_IMPORT_META',
    '-sEXPORT_ES6'
  ]
)

# debug

executable(
  'libultrahdr-debug',
  'libultrahdr-wasm.cpp',
  dependencies: [deps, libultrahdr_dep],
  include_directories: lib_ultra_hdr_inc,
  link_args: [
    link_args,
    '-gsource-map',
    '-sLOAD_SOURCE_MAP',
    '-sNO_DISABLE_EXCEPTION_CATCHING',
    '-sMODULARIZE',
    '-sUSE_ES6_IMPORT_META',
    '-sEXPORT_ES6'
  ]
)

# npm pack doesn't work otherwise
fs = import('fs')
fs.copyfile('.npmignore', '.npmignore')
