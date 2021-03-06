project(PhononVLC)

cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Phonon
find_package(Phonon REQUIRED)
if(PHONON_PULSESUPPORT)
  add_definitions(-DPHONON_PULSESUPPORT)
endif(PHONON_PULSESUPPORT)
if(PHONON_FOUND_EXPERIMENTAL)
  add_definitions(-DPHONON_EXPERIMENTAL)
else(PHONON_FOUND_EXPERIMENTAL)
  add_definitions(-DPHONON_VLC_NO_EXPERIMENTAL)
endif(PHONON_FOUND_EXPERIMENTAL)

# libVLC
set(LIBVLC_MIN_VERSION "1.1.1")
find_package(LIBVLC REQUIRED)

add_definitions(${QT_DEFINITIONS})
remove_definitions(-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

include_directories(${PHONON_INCLUDES} ${QT_INCLUDES} ${LIBVLC_INCLUDE_DIR})

set(PHONON_VLC_MAJOR_VERSION "0")
set(PHONON_VLC_MINOR_VERSION "4")
set(PHONON_VLC_PATCH_VERSION "55")
set(PHONON_VLC_VERSION "${PHONON_VLC_MAJOR_VERSION}.${PHONON_VLC_MINOR_VERSION}.${PHONON_VLC_PATCH_VERSION}")
add_definitions(-DPHONON_VLC_VERSION="${PHONON_VLC_VERSION}")

add_subdirectory(src)
