# -*- mode: CMAKE; -*-

cmake_minimum_required(VERSION 2.6)

project(rpn)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE string "Choose the type of build, options are: None Debug Release" FORCE)
endif()

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

# INFO
set(RPN_VERSION "2.1")
set(RPN_DISPLAY_NAME "rpn")
set(RPN_URL_INFO_ABOUT "https://github.com/louisrubet/rpn")
set(RPN_CONTACT "Louis Rubet <louis@rubet.fr>")
set(RPN_FRIENDLY_STRING "rpn - Reverse Polish Notation language")

# compiler options
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "Compiler type GNU: ${CMAKE_CXX_COMPILER}")
  set(BASE_COMPILER_OPTIONS "-std=c++0x -Wl,--no-as-needed -lmpfr")
  set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} ${BASE_COMPILER_OPTIONS}")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} ${BASE_COMPILER_OPTIONS} -O0 -g")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} ${BASE_COMPILER_OPTIONS} -O3 -fomit-frame-pointer")

else()
  # unknown compiler
  message(STATUS "Compiler type UNKNOWN: ${CMAKE_CXX_COMPILER}")
  set(BASE_COMPILER_OPTIONS "-std=c++11 -Wall -Wextra")
  set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} ${BASE_COMPILER_OPTIONS}")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} ${BASE_COMPILER_OPTIONS} -O0 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} ${BASE_COMPILER_OPTIONS} -Os")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} ${BASE_COMPILER_OPTIONS} -O3 -fomit-frame-pointer")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${BASE_COMPILER_OPTIONS} -O3 -g")

endif()

# includes
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/linenoise-ng_v1.0.1/include)

# build
add_executable(
  rpn
  src/main.cpp
  src/object.cpp
  src/program.cpp
  linenoise-ng_v1.0.1/src/ConvertUTF.cpp
  linenoise-ng_v1.0.1/src/linenoise.cpp
  linenoise-ng_v1.0.1/src/wcwidth.cpp
)

# install
install(TARGETS rpn DESTINATION bin)

# packages
SET(CPACK_PACKAGE_FILE_NAME "${RPN_DISPLAY_NAME}-${RPN_VERSION}-amd64")
SET(CPACK_PACKAGE_NAME ${RPN_DISPLAY_NAME})
SET(CPACK_PACKAGE_VERSION ${RPN_VERSION})
SET(CPACK_PACKAGE_CONTACT ${RPN_CONTACT})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY, ${RPN_FRIENDLY_STRING})

# DEB package
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME ${RPN_DISPLAY_NAME})
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION, ${RPN_FRIENDLY_STRING})
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${RPN_CONTACT})
SET(CPACK_DEBIAN_PACKAGE_VERSION ${RPN_VERSION})
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${SYSTEM_PROCESSOR})
SET(CPACK_DEBIAN_PACKAGE_SECTION "math")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
#SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")
#SET(DEBIAN_PACKAGE_BUILDS_DEPENDS "debhelper (>> 5.0.0), libncurses5-dev, tcl8.4")
include(CPack)
