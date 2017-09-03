# --------------------------------------------------------------------
#  LibUjo:  An UJO binaray data object notation library.
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this program; if not, write to the Free
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#  02111-1307 USA
#  
#  You may find a copy of the license under this software is released
#  at COPYING file. This is LGPL software: you are welcome to develop
#  proprietary applications using this library without any royalty or
#  fee but returning back any change, improvement or addition in the
#  form of source code, project image, documentation patches, etc.
# --------------------------------------------------------------------
#  root CMake file for libujo-c project
#
#    From the off-tree build directory, invoke:
#      $ cmake <OPTIONS> <PATH_TO_LIBUJO_ROOT>
#
# --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)
project (ujo)

include(TestBigEndian)

#######################################################
## set build parameter
#######################################################
SET (UJO_TARGET_PLATFORM win_x86-64 CACHE STRING "set the build target")
#######################################################

set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/")
  
set (UJO_MAJOR_VERSION 0) 
set (UJO_MINOR_VERSION 9)
set (UJO_MICRO_VERSION 1)
set (UJO_PRERELEASE_VERSION )
set (UJO_API_VERSION 11)
set (UJO_VERSION 0.9.1)
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR "${UJO_MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${UJO_MINOR_VERSION}")
set (PLATFORM_FOUND false)

TEST_BIG_ENDIAN(UJO_BIG_ENDIAN)

if (UJO_BIG_ENDIAN)
   set (UJO_BYTE_ORDER 4321)
else ()
   set (UJO_BYTE_ORDER 1234)
endif ()

configure_file (
   "${SOURCE_DIR}/src/ujo_config.h.in"
   "${SOURCE_DIR}/src/ujo_config.h"
)



#######################################################
## Windows 32 bit
if (${UJO_TARGET_PLATFORM} STREQUAL win_x86-32)
  set(PROJECT win_x86-32)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows 64 bit
if (${UJO_TARGET_PLATFORM} STREQUAL win_x86-64)
  set(PROJECT win_x86-64)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL win_x86-64)

#######################################################
## LINUX 32 bit
if (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-32)
  set(PROJECT linux_x86-32)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX 64 bit
if (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-64)
  set(PROJECT linux_x86-64)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## OSX 32 bit
if (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-32)
  set(PROJECT osx_x86-32)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX 64 bit
if (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-64)
  set(PROJECT osx_x86-64)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## ANDROID ARM32
if (${UJO_TARGET_PLATFORM} STREQUAL android_arm32)
  set(PROJECT android_arm32)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## LINUX ARM32
if (${UJO_TARGET_PLATFORM} STREQUAL linux_arm32)
  set(PROJECT linux_arm32)
  add_subdirectory(./src  ${PROJECT}/libujo-c)
  add_subdirectory (./test  ${PROJECT}/test)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ${SOURCE_DIR}/COPYING)
  include (CPack)
  
  set (PLATFORM_FOUND true)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_arm32)

if (${PLATFORM_FOUND} STREQUAL false)
  message( SEND_ERROR "Unknown target platform / please check \"UJO_TARGET_PLATFORM\"" )
endif (${PLATFORM_FOUND} STREQUAL false)

