# -*- mode: CMAKE; -*-

cmake_minimum_required(VERSION 2.6)

project(rpn)

if((NOT CMAKE_BUILD_TYPE MATCHES Debug)
    AND (NOT CMAKE_BUILD_TYPE MATCHES Release))
    set(CMAKE_BUILD_TYPE Release CACHE string "Choose the type of build, options are: None Debug Release" FORCE)
endif()

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

# INFO
set(RPN_VERSION "2.3.2")
set(RPN_DISPLAY_NAME "rpn")
set(RPN_URL_INFO_ABOUT "https://github.com/louisrubet/rpn")
set(RPN_CONTACT "Louis Rubet <louis@rubet.fr>")
set(RPN_FRIENDLY_STRING "rpn - Reverse Polish Notation math language")
set(RPN_LICENSE "LGPLv3")
set(RPN_LICENSE_FILE "${PROJECT_SOURCE_DIR}/LICENSE")
set(RPN_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")

# compiler options
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Compiler type GNU: ${CMAKE_CXX_COMPILER}")
    # TODO still up to date?
    set(BASE_COMPILER_OPTIONS "-std=c++14 -Wl,--no-as-needed")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_COMPILER_OPTIONS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BASE_COMPILER_OPTIONS} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BASE_COMPILER_OPTIONS} -O3 -fomit-frame-pointer -s")
endif()

# custom linenoise-ng
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/linenoise-ng/.git")
    execute_process(COMMAND git submodule init ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(COMMAND git submodule update ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(COMMAND git checkout v1.1.1-rpn WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/linenoise-ng)
endif()

# custom mpreal
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/mpreal/.git")
    execute_process(COMMAND git submodule init ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(COMMAND git submodule update ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(COMMAND git checkout mpfrc++-3.6.9 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mpreal)
endif()

# includes
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/linenoise-ng/include ${PROJECT_SOURCE_DIR}/mpreal)

# build
add_executable(
  rpn
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/object.cpp
  ${PROJECT_SOURCE_DIR}/src/mpreal-out.cpp
  ${PROJECT_SOURCE_DIR}/src/program.cpp
  ${PROJECT_SOURCE_DIR}/src/parse.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-branch.cpp
  #${PROJECT_SOURCE_DIR}/src/rpn-complex.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-general.cpp
  #${PROJECT_SOURCE_DIR}/src/rpn-logs.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-program.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-real.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-stack.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-store.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-string.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-test.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-test-framework.cpp
  #${PROJECT_SOURCE_DIR}/src/rpn-time.cpp
  ${PROJECT_SOURCE_DIR}/src/rpn-trig.cpp
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/ConvertUTF.cpp
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/linenoise.cpp
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/wcwidth.cpp
)
target_link_libraries(rpn mpfr)

# man
add_custom_target(man ALL)
add_custom_command(
    TARGET man
    COMMAND gzip -f -k ${CMAKE_CURRENT_SOURCE_DIR}/doc/rpn.1
    OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/rpn.1.gz
)

# install
install(TARGETS rpn DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/doc/rpn.1.gz DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)

# packages (common variables for all packages)
set(CPACK_PACKAGE_NAME ${RPN_DISPLAY_NAME})
set(CPACK_PACKAGE_VENDOR ${RPN_CONTACT})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${RPN_FRIENDLY_STRING})
set(CPACK_PACKAGE_DESCRIPTION ${RPN_FRIENDLY_STRING})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${RPN_DESCRIPTION_FILE})
set(CPACK_RESOURCE_FILE_LICENSE ${RPN_LICENSE_FILE})
set(CPACK_RESOURCE_FILE_README ${RPN_DESCRIPTION_FILE})

set(CPACK_PACKAGE_FILE_NAME "${RPN_DISPLAY_NAME}-${RPN_VERSION}-amd64")
set(CPACK_PACKAGE_VERSION ${RPN_VERSION})
#set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_CONTACT ${RPN_CONTACT})
set(CPACK_PACKAGE_LICENSE ${RPN_LICENSE})
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

# DEB package
set(CPACK_GENERATOR "DEB;RPM;TGZ")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${RPN_CONTACT})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_SECTION "math")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

include(CPack)

# RPM package
set(CPACK_RPM_PACKAGE_REQUIRES "mpfr")
