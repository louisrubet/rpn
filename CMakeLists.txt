# -*- mode: CMAKE; -*-

project(rpn)

cmake_minimum_required(VERSION 3.22)

if((NOT CMAKE_BUILD_TYPE MATCHES Debug)
    AND (NOT CMAKE_BUILD_TYPE MATCHES Release))
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release" FORCE)
endif()

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND git describe --long HEAD OUTPUT_VARIABLE "GIT_VERSION" OUTPUT_STRIP_TRAILING_WHITESPACE)
    add_definitions(-DGIT_VERSION="${GIT_VERSION}")
else(EXISTS ${PROJECT_SOURCE_DIR}/.git)
    set(GIT_VERSION "unknown")
endif()
add_definitions(-DGIT_VERSION="${GIT_VERSION}")
message("GIT_VERSION is ${GIT_VERSION}")
message("GIT_VERSION is ${GIT_VERSION}")

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

# src
set(RPN_SRC_FILES
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
    ${PROJECT_SOURCE_DIR}/src/rpn-trig.cc)

set(LINENOISE_NG_SRC_FILES
${PROJECT_SOURCE_DIR}/linenoise-ng/src/ConvertUTF.cpp
${PROJECT_SOURCE_DIR}/linenoise-ng/src/linenoise.cpp
${PROJECT_SOURCE_DIR}/linenoise-ng/src/wcwidth.cpp)

# compiler options
set_source_files_properties(${RPN_SRC_FILES} COMPILE_FLAGS # some harder warnings
    "-Wall -Wextra -pedantic -Wno-missing-field-initializers")

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Compiler type GNU: ${CMAKE_CXX_COMPILER}")
    # TODO still up to date?
    set(BASE_COMPILER_OPTIONS "-std=c++17 -Wl,--no-as-needed")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_COMPILER_OPTIONS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BASE_COMPILER_OPTIONS} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BASE_COMPILER_OPTIONS} -O3 -s")
endif()

add_executable(rpn ${RPN_SRC_FILES} ${LINENOISE_NG_SRC_FILES})

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
