# -*- mode: CMAKE; -*-

project(rpn)

cmake_minimum_required(VERSION 3.16)

if((NOT CMAKE_BUILD_TYPE MATCHES Debug)
    AND (NOT CMAKE_BUILD_TYPE MATCHES Release))
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release" FORCE)
endif()

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

if(NOT DEFINED RPN_VERSION)
    if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
        execute_process(COMMAND git describe HEAD OUTPUT_VARIABLE "RPN_VERSION" OUTPUT_STRIP_TRAILING_WHITESPACE)
    else(EXISTS ${PROJECT_SOURCE_DIR}/.git)
        set(VERSION "unknown")
    endif()
    add_definitions(-DGIT_VERSION="${RPN_VERSION}")
endif()
message("RPN_VERSION is ${RPN_VERSION}")
add_definitions(-DRPN_VERSION="${RPN_VERSION}")

# INFO
set(RPN_DISPLAY_NAME "rpn")
set(RPN_URL_INFO_ABOUT "https://github.com/louisrubet/rpn")
set(RPN_CONTACT "Louis Rubet <louis@rubet.fr>")
set(RPN_FRIENDLY_STRING "Reverse Polish Notation math language")
set(RPN_LICENSE "LGPLv3")
set(RPN_LICENSE_FILE "${PROJECT_SOURCE_DIR}/LICENSE")
set(RPN_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")

# custom linenoise-ng
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/linenoise-ng/.git")
    execute_process(COMMAND git submodule init ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(COMMAND git submodule update ${PROJECT_SOURCE_DIR}/linenoise-ng)
    execute_process(COMMAND git checkout v1.1.4-rpn WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/linenoise-ng)
endif()

# custom mpreal
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/mpreal/.git")
    execute_process(COMMAND git submodule init ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(COMMAND git submodule update ${PROJECT_SOURCE_DIR}/mpreal)
    execute_process(COMMAND git checkout mpfrc++-3.6.9 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mpreal)
endif()

# includes
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/linenoise-ng/include ${PROJECT_SOURCE_DIR}/mpreal)

# src
set(RPN_SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/object.cc
    ${PROJECT_SOURCE_DIR}/src/program.cc
    ${PROJECT_SOURCE_DIR}/src/main.cc
    ${PROJECT_SOURCE_DIR}/src/object.cc
    ${PROJECT_SOURCE_DIR}/src/mpreal-out.cc
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
    ${PROJECT_SOURCE_DIR}/src/rpn-trig.cc)

set(LINENOISE_NG_SRC_FILES
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/ConvertUTF.cpp
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/linenoise.cpp
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/wcwidth.cpp)

# compiler options
set_source_files_properties(${RPN_SRC_FILES} COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-missing-field-initializers")

add_compile_options("-std=c++20"
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
    "$<$<CONFIG:RELEASE>:-O3>")

add_executable(rpn ${RPN_SRC_FILES} ${LINENOISE_NG_SRC_FILES})

target_link_libraries(rpn mpfr)
target_link_libraries(rpn gmp)

# install
install(TARGETS rpn DESTINATION bin)
