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
#  Root CMake file for libujo-c regression tests
#
#    From the off-tree build directory, invoke:
#      $ cmake <OPTIONS> <PATH_TO_LIBUJO_ROOT>
#
# --------------------------------------------------------------------

MESSAGE(STATUS "UJO regression tests")


set  (TEST_UJO_SOURCES
      "testujo.c")

source_group("Sourcefiles" FILES ${TEST_UJO_SOURCES})

include_directories (.)
include_directories (../src)


#######################################################
## Windows 32 bit
if (${UJO_TARGET_PLATFORM} STREQUAL win_x86-32)
  set (TARGETNAME testujo)
endif (${UJO_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows 64 bit
if (${UJO_TARGET_PLATFORM} STREQUAL win_x86-64)
  set (TARGETNAME testujo64)
endif (${UJO_TARGET_PLATFORM} STREQUAL win_x86-64)

#######################################################
## LINUX 32 bit
if (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-32)
  ADD_DEFINITIONS("-m32")
  ADD_DEFINITIONS("-DLINUX")
  set(CMAKE_C_FLAGS "-m32")
  set (TARGETNAME testujo)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX 64 bit
if (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-64)
  ADD_DEFINITIONS("-m64")
  ADD_DEFINITIONS("-DLINUX")
  set(CMAKE_C_FLAGS "-m64")
  set (TARGETNAME testujo64)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## OSX 32 bit
#######################################################
if (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-32)
  ADD_DEFINITIONS("-arch i386")
  ADD_DEFINITIONS("-DLINUX -DOS_X")
  #ADD_DEFINITIONS("-std=c++11")
  set(CMAKE_C_FLAGS "-arch i386")
  set (TARGETNAME testujo)
endif (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX 64 bit
#######################################################
if (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-64)
  ADD_DEFINITIONS("-arch x86_64")
  ADD_DEFINITIONS("-DLINUX -DOS_X")
  set(CMAKE_C_FLAGS "-arch x86_64")
  set (TARGETNAME testujo64)
endif (${UJO_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## LINUX ARM32
if (${UJO_TARGET_PLATFORM} STREQUAL linux_arm32)
  ADD_DEFINITIONS("-DLINUX")
  set (TARGETNAME testujo)
endif (${UJO_TARGET_PLATFORM} STREQUAL linux_arm32)

add_executable(${TARGETNAME} 
	${TEST_UJO_SOURCES})
target_link_libraries(${TARGETNAME} ${UJOLIBNAME}) 
set_property(TARGET ${TARGETNAME} PROPERTY FOLDER "test")
