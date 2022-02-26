# -*- mode: CMAKE; -*-

project(rpn)

cmake_minimum_required(VERSION 3.22)

if((NOT CMAKE_BUILD_TYPE MATCHES Debug)
    AND (NOT CMAKE_BUILD_TYPE MATCHES Release))
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release" FORCE)
endif()

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

if(EXISTS ${PROJECT_SOURCE_DIR}/.git)
  EXECUTE_PROCESS(
    COMMAND git rev-parse --abbrev-ref --short=10 HEAD
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE "BRANCH")
    MESSAGE( STATUS "Source Git Branch: ${GIT_BRANCH}" )
    EXECUTE_PROCESS(
      COMMAND git describe --dirty --tags --long --always
      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
      OUTPUT_VARIABLE "TAG"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
      MESSAGE( STATUS "Source Git Tag: ${GIT_TAG}" )
      EXECUTE_PROCESS(
        COMMAND echo ${TAG}-${BRANCH}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_VARIABLE "GIT_VERSION"
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
        MESSAGE( STATUS "Source Git version: ${GIT_VERSION}" )
        add_definitions(-DGIT_VERSION="${GIT_VERSION}")
  else(EXISTS ${PROJECT_SOURCE_DIR}/.git)
    add_definitions(-DGIT_VERSION="unknown")
    message("Warning, built outside of a git repo, version set to ${GIT_VERSION}")
endif(EXISTS ${PROJECT_SOURCE_DIR}/.git)

# INFO
set(RPN_DISPLAY_NAME "rpn")
set(RPN_URL_INFO_ABOUT "https://github.com/louisrubet/rpn")
set(RPN_CONTACT "Louis Rubet <louis@rubet.fr>")
set(RPN_FRIENDLY_STRING "Reverse Polish Notation math language")
set(RPN_LICENSE "LGPLv3")
set(RPN_LICENSE_FILE "${PROJECT_SOURCE_DIR}/LICENSE")
set(RPN_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")

# compiler options
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Compiler type GNU: ${CMAKE_CXX_COMPILER}")
    # TODO still up to date?
    set(BASE_COMPILER_OPTIONS "-std=c++17 -Wl,--no-as-needed")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_COMPILER_OPTIONS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BASE_COMPILER_OPTIONS} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BASE_COMPILER_OPTIONS} -O3 -s")
endif()

# custom linenoise-ng
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/linenoise-ng/.git")
    execute_process(command git submodule init ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(command git submodule update ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(command git checkout v1.1.1-rpn WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/linenoise-ng)
endif()

# custom mpreal
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/mpreal/.git")
    execute_process(command git submodule init ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(command git submodule update ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(command git checkout mpfrc++-3.6.9 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mpreal)
endif()

# includes
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/linenoise-ng/include ${PROJECT_SOURCE_DIR}/mpreal)

# build
add_executable(
  rpn
  ${PROJECT_SOURCE_DIR}/src/main.cc
  ${PROJECT_SOURCE_DIR}/src/object.cc
  ${PROJECT_SOURCE_DIR}/src/mpreal-out.cc
  ${PROJECT_SOURCE_DIR}/src/program.cc
  ${PROJECT_SOURCE_DIR}/src/lexer.cc
  ${PROJECT_SOURCE_DIR}/src/input.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-branch.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-complex.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-general.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-logs.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-program.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-real.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-stack.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-store.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-string.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-test.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-test-framework.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-time.cc
  ${PROJECT_SOURCE_DIR}/src/rpn-trig.cc
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/ConvertUTF.cpp
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/linenoise.cpp
  ${PROJECT_SOURCE_DIR}/linenoise-ng/src/wcwidth.cpp
)

target_link_libraries(rpn mpfr)
target_link_libraries(rpn gmp)

# man
add_custom_target(man ALL)
add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/doc/rpn.1.gz
    COMMAND gzip -f -k ${PROJECT_SOURCE_DIR}/doc/rpn.1
)

# install
install(TARGETS rpn DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/doc/rpn.1.gz DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)
