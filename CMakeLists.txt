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

add_subdirectory(libbfdec-cpp)

# includes
include_directories(${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/linenoise-ng/include
    ${PROJECT_SOURCE_DIR}/libbfdec-cpp
    ${PROJECT_SOURCE_DIR}/libbfdec-cpp/libbf
    ${PROJECT_SOURCE_DIR}/libbfdec-cpp/quickjs-fake)

# src
set(RPN_SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/object.cc
    ${PROJECT_SOURCE_DIR}/src/program.cc
    ${PROJECT_SOURCE_DIR}/src/main.cc
    ${PROJECT_SOURCE_DIR}/src/object.cc
    # ${PROJECT_SOURCE_DIR}/src/mpreal-out.cc
    ${PROJECT_SOURCE_DIR}/src/lexer.cc
    ${PROJECT_SOURCE_DIR}/src/input.cc
    # ${PROJECT_SOURCE_DIR}/src/rpn-branch.cc
    # ${PROJECT_SOURCE_DIR}/src/rpn-complex.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-general.cc
    # ${PROJECT_SOURCE_DIR}/src/rpn-logs.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-program.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-real.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-stack.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-store.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-string.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-test.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-test-framework.cc
    ${PROJECT_SOURCE_DIR}/src/rpn-time.cc
    # ${PROJECT_SOURCE_DIR}/src/rpn-trig.cc
    # ${PROJECT_SOURCE_DIR}/libbfdec-cpp/libbf/libbf.c
    # ${PROJECT_SOURCE_DIR}/libbfdec-cpp/libbf/cutils.c
    # ${PROJECT_SOURCE_DIR}/libbfdec-cpp/libbfdec++.cpp
    )

set(LINENOISE_NG_SRC_FILES
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/ConvertUTF.cpp
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/linenoise.cpp
    ${PROJECT_SOURCE_DIR}/linenoise-ng/src/wcwidth.cpp)

# compiler options
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set_source_files_properties(${RPN_SRC_FILES} COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-missing-field-initializers")
add_compile_options("$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>" "$<$<CONFIG:RELEASE>:-O3>")

# executable
add_executable(rpn ${RPN_SRC_FILES} ${LINENOISE_NG_SRC_FILES} ${LIBBF_SRC_FILES})
target_link_libraries(rpn libbf)

# install
install(TARGETS rpn DESTINATION bin)
