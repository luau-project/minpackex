#                                                                                 #
#                                                                                 #
###################################################################################
# License                                                                         #
###################################################################################
#                                                                                 #
# Copyright (c) 2024 luau-project                                                 #
#                    https://github.com/luau-project/minpackex                    #
#                                                                                 #
# Redistribution and use in source and binary forms, with or without              #
# modification, are permitted provided that the following conditions are met:     #
#                                                                                 #
# 1. Redistributions of source code must retain the above copyright notice,       #
# this list of conditions and the following disclaimer.                           #
#                                                                                 #
# 2. Redistributions in binary form must reproduce the above copyright            #
# notice, this list of conditions and the following disclaimer in the             #
# documentation and/or other materials provided with the distribution.            #
#                                                                                 #
# 3. Neither the name of the copyright holder nor the names of its                #
# contributors may be used to endorse or promote products derived from this       #
# software without specific prior written permission.                             #
#                                                                                 #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”     #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,           #
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR          #
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR               #
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,           #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,             #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;     #
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,        #
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR         #
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF          #
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                      #
#                                                                                 #
###################################################################################
###################################################################################
##                                 Body                                          ##
###################################################################################
###################################################################################
#                                                                                 #
#                                                                                 #

cmake_minimum_required(VERSION 3.17)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type.")
option(BUILD_SHARED_LIBS "Build the shared library." ON)
option(BUILD_STATIC_LIBS "Build the static library." OFF)
option(USE_COVERAGE "Use code coverage on tests." OFF)
set(COVERAGE_REPORT_TYPE "LCOV" CACHE STRING "Report type for code coverage. Possible values are LCOV (default), GCOVR_XML, GCOVR_HTML and FASTCOV.")

# at least one kind of library must be built
if (NOT (BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS))
    message(FATAL_ERROR "You must build either the shared or static library by settting `-DBUILD_SHARED_LIBS=ON` or `-DBUILD_STATIC_LIBS=ON`")
endif()

set(MINPACKEX_VERSION_MAJOR 0)
set(MINPACKEX_VERSION_MINOR 1)
set(MINPACKEX_VERSION_PATCH 0)

# basic project properties
project(minpackex
    VERSION "${MINPACKEX_VERSION_MAJOR}.${MINPACKEX_VERSION_MINOR}.${MINPACKEX_VERSION_PATCH}"
    LANGUAGES C
)

# set the library name
set(MINPACKEX_LIBRARY_NAME "minpackex")

# source files
set(MINPACKEX_SOURCE_FILES
    "src/${MINPACKEX_LIBRARY_NAME}_chkder.c"
    "src/${MINPACKEX_LIBRARY_NAME}_dpmpar.c"
    "src/${MINPACKEX_LIBRARY_NAME}_enorm.c"
    "src/${MINPACKEX_LIBRARY_NAME}_hybrd.c"
    "src/${MINPACKEX_LIBRARY_NAME}_hybrd1.c"
    "src/${MINPACKEX_LIBRARY_NAME}_hybrj.c"
    "src/${MINPACKEX_LIBRARY_NAME}_hybrj1.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmdif.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmdif1.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmder.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmder1.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmstr.c"
    "src/${MINPACKEX_LIBRARY_NAME}_lmstr1.c"
)

# main header input file
set(MINPACKEX_MAIN_HEADER_INPUT "cmake/${MINPACKEX_LIBRARY_NAME}.h.in")

# main header transformed file
set(MINPACKEX_MAIN_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}.h")

# export header generated by GenerateExportHeader CMake module
set(MINPACKEX_EXPORT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}_export.h")

# installable header files
set(MINPACKEX_HEADER_FILES ${MINPACKEX_MAIN_HEADER} ${MINPACKEX_EXPORT_HEADER})

# license files
set(MINPACKEX_LICENSE_FILES "LICENSE.md")

if (WIN32)
    # def file required to generate import file (.lib) to link shared libraries
    # with Intel C compiler
    if (MSVC OR "${CMAKE_C_SIMULATE_ID}" MATCHES "MSVC")
        set(MINPACKEX_DEF_FILE "src/${MINPACKEX_LIBRARY_NAME}.def")
    else()
        set(MINPACKEX_DEF_FILE "src/lib${MINPACKEX_LIBRARY_NAME}.def")
    endif()
endif()

# use GNU directory conventions
include(GNUInstallDirs)

# if MINPACKEX_INSTALL_BINDIR was not provided by the user
if (NOT DEFINED MINPACKEX_INSTALL_BINDIR)
    # use the one provided by GNUInstallDirs
    set(MINPACKEX_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
endif()

# if MINPACKEX_INSTALL_LIBDIR was not provided by the user
if (NOT DEFINED MINPACKEX_INSTALL_LIBDIR)
    # use the one provided by GNUInstallDirs
    set(MINPACKEX_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
endif()

# if MINPACKEX_INSTALL_INCLUDEDIR was not provided by the user
if (NOT DEFINED MINPACKEX_INSTALL_INCLUDEDIR)
    # use the one provided by GNUInstallDirs
    set(MINPACKEX_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# if MINPACKEX_INSTALL_DOCDIR was not provided by the user
if (NOT DEFINED MINPACKEX_INSTALL_DOCDIR)
    # use the one provided by GNUInstallDirs
    set(MINPACKEX_INSTALL_DOCDIR ${CMAKE_INSTALL_DOCDIR})
endif()

if (NOT DEFINED MINPACKEX_INSTALL_PKGCONFIGDIR)
    # Installation directory for pkg-config file.
    set(MINPACKEX_INSTALL_PKGCONFIGDIR "${MINPACKEX_INSTALL_LIBDIR}/pkgconfig")
endif()

if (IS_ABSOLUTE "${MINPACKEX_INSTALL_BINDIR}")
    message(FATAL_ERROR "MINPACKEX_INSTALL_BINDIR must be a relative path.")
endif()

if (IS_ABSOLUTE "${MINPACKEX_INSTALL_LIBDIR}")
    message(FATAL_ERROR "MINPACKEX_INSTALL_LIBDIR must be a relative path.")
endif()

if (IS_ABSOLUTE "${MINPACKEX_INSTALL_INCLUDEDIR}")
    message(FATAL_ERROR "MINPACKEX_INSTALL_INCLUDEDIR must be a relative path.")
endif()

if (IS_ABSOLUTE "${MINPACKEX_INSTALL_DOCDIR}")
    message(FATAL_ERROR "MINPACKEX_INSTALL_DOCDIR must be a relative path.")
endif()

if (IS_ABSOLUTE "${MINPACKEX_INSTALL_PKGCONFIGDIR}")
    message(FATAL_ERROR "MINPACKEX_INSTALL_PKGCONFIGDIR must be a relative path.")
endif()

# pkgconfig input file
set(MINPACKEX_PKGCONFIG_INPUT "cmake/${MINPACKEX_LIBRARY_NAME}.pc.in")
set(MINPACKEX_PKGCONFIG "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}.pc")

# minpackex config cmake file to be transformed
set(minpackex_config_cmake_in "cmake/${MINPACKEX_LIBRARY_NAME}-config.cmake.in")

# minpackex config cmake destination file
set(minpackex_config_cmake "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}-config.cmake")

# minpackex config version cmake file to be transformed
set(minpackex_config_version_cmake_in "cmake/${MINPACKEX_LIBRARY_NAME}-config-version.cmake.in")

# minpackex config version cmake destination file
set(minpackex_config_version_cmake "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}-config-version.cmake")

# minpackex targets destination file
set(minpackex_targets_export "${MINPACKEX_LIBRARY_NAME}-targets")

# hold targets to install
set(targets_to_install "")

include(cmake/FindMinpack.cmake)

enable_testing()

if (BUILD_TESTING)
    include(cmake/SetupMinpackexTests.cmake)
endif()

# Whether it is possible to run
# code coverage or not.
# For now, code coverage is only supported for GCC on Unix
set(MINPACKEX_CONFIG_IS_ELLEGIBLE_FOR_COVERAGE
    UNIX AND (NOT (CYGWIN OR MINGW)) AND BUILD_TESTING AND
    CMAKE_C_COMPILER_ID STREQUAL "GNU" AND
    "${CMAKE_BUILD_TYPE}" MATCHES "Debug")

# Whether to perform code coverage or not.
set(MINPACKEX_PERFORM_COVERAGE MINPACKEX_CONFIG_IS_ELLEGIBLE_FOR_COVERAGE AND USE_COVERAGE)

if (MINPACKEX_PERFORM_COVERAGE)
    include(cmake/GccCodeCoverage.cmake)
endif()

set(minpackex_coverage_target_list "")

# generate minpackex_export.h
include(GenerateExportHeader)

# build the shared library
if (BUILD_SHARED_LIBS)
    set(MINPACKEX_SHARED_LIB "minpackex_SHARED")

    add_library(${MINPACKEX_SHARED_LIB} SHARED "")
    target_sources(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})

    generate_export_header(${MINPACKEX_SHARED_LIB} BASE_NAME ${MINPACKEX_LIBRARY_NAME})

    if (WIN32)
        target_sources(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACKEX_DEF_FILE})
    endif()

    target_include_directories(${MINPACKEX_SHARED_LIB} PRIVATE ${minpack_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} src)
    target_link_libraries(${MINPACKEX_SHARED_LIB} PRIVATE ${minpack_LIBRARIES})

    set_target_properties(${MINPACKEX_SHARED_LIB}
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    if ((WIN32 AND DEFINED minpack_LIBRARY_DIR AND DEFINED minpack_RUNTIME_DIR) OR (UNIX AND "${minpack_LIBRARY}" MATCHES "libminpack\\.(so|dylib)$"))
        set(dependencies_lflags "\"-L${minpack_LIBRARY_DIR}\" -lminpack")
    endif()

    if (BUILD_TESTING)
        if (MINPACKEX_PERFORM_COVERAGE)
            append_coverage_compiler_flags_to_target(${MINPACKEX_SHARED_LIB})
        endif()

        setup_minpackex_tests_for_lib_target(
            ${MINPACKEX_SHARED_LIB}
            ${MINPACKEX_LIBRARY_NAME}
            "shared"
            "${minpack_RUNTIME_DIR}"
            "${minpack_INCLUDE_DIRS}"
            "${minpack_LIBRARIES}"
            "${MINPACKEX_PERFORM_COVERAGE}"
            "${COVERAGE_REPORT_TYPE}"
            minpackex_shared_lib_coverage_name
        )

        if (MINPACKEX_PERFORM_COVERAGE)
            list(APPEND minpackex_coverage_target_list "${minpackex_shared_lib_coverage_name}")
        endif()
    endif()

    list(APPEND targets_to_install ${MINPACKEX_SHARED_LIB})
endif()

# build the static library
if (BUILD_STATIC_LIBS)
    set(MINPACKEX_STATIC_LIB "minpackex_STATIC")

    add_library(${MINPACKEX_STATIC_LIB} STATIC "")
    target_sources(${MINPACKEX_STATIC_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})

    if (BUILD_SHARED_LIBS)
        # needed by minpackex_export.h
        target_compile_definitions(${MINPACKEX_STATIC_LIB} PRIVATE "MINPACKEX_STATIC_DEFINE")
    else()
        generate_export_header(${MINPACKEX_STATIC_LIB} BASE_NAME ${MINPACKEX_LIBRARY_NAME})
    endif()

    target_include_directories(${MINPACKEX_STATIC_LIB} PRIVATE ${minpack_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} src)
    target_link_libraries(${MINPACKEX_STATIC_LIB} PRIVATE ${minpack_LIBRARIES})

    set_target_properties(${MINPACKEX_STATIC_LIB}
        PROPERTIES
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    if ((NOT DEFINED dependencies_lflags) AND ((WIN32 AND DEFINED minpack_LIBRARY_DIR AND DEFINED minpack_RUNTIME_DIR) OR (UNIX AND "${minpack_LIBRARY}" MATCHES "libminpack\\.(so|dylib)$")))
        set(dependencies_lflags "\"-L${minpack_LIBRARY_DIR}\" -lminpack")
    endif()

    if (BUILD_TESTING)
        if (MINPACKEX_PERFORM_COVERAGE)
            append_coverage_compiler_flags_to_target(${MINPACKEX_STATIC_LIB})
        endif()
        
        setup_minpackex_tests_for_lib_target(
            ${MINPACKEX_STATIC_LIB}
            ${MINPACKEX_LIBRARY_NAME}
            "static"
            "${minpack_RUNTIME_DIR}"
            "${minpack_INCLUDE_DIRS}"
            "${minpack_LIBRARIES}"
            "${MINPACKEX_PERFORM_COVERAGE}"
            "${COVERAGE_REPORT_TYPE}"
            minpackex_static_lib_coverage_name
        )

        if (MINPACKEX_PERFORM_COVERAGE)
            list(APPEND minpackex_coverage_target_list "${minpackex_static_lib_coverage_name}")
        endif()
    endif()

    list(APPEND targets_to_install ${MINPACKEX_STATIC_LIB})
endif()

if (MINPACKEX_PERFORM_COVERAGE)
    add_custom_target(coverage DEPENDS "${minpackex_coverage_target_list}")
endif()

set(_dependencies_cflags_list "")

foreach (_minpack_inc_dir ${minpack_INCLUDE_DIRS})
    list(APPEND _dependencies_cflags_list "\"-I${_minpack_inc_dir}\"")
endforeach()

list(JOIN _dependencies_cflags_list " " dependencies_cflags)

configure_file(
    ${minpackex_config_cmake_in}
    ${minpackex_config_cmake} @ONLY
)
configure_file(
    ${minpackex_config_version_cmake_in}
    ${minpackex_config_version_cmake} @ONLY
)

configure_file(
    ${MINPACKEX_MAIN_HEADER_INPUT}
    ${MINPACKEX_MAIN_HEADER} @ONLY
)

# At the moment of writing, on Windows,
# the generated minpackex_export.h header file
# uses __declspec(dllexport) to export functions on shared libraries,
# and __declspec(dllimport) to use such exported functions.
#
# Below, especially for GCC and Clang from MSYS2,
# we replace __declspec(dllimport) with the empty string.
# We employ this "hack" here to have both shared and static libraries built,
# not posing any compiler definition (-D<>) for the library user.
# Doing so also makes tests execution easier on C.I. for such compilers.

if (WIN32 AND (CYGWIN OR MINGW))
    file(READ "${MINPACKEX_EXPORT_HEADER}" __MINPACKEX_EXPORT_HEADER_CONTENT)
    string(REPLACE "__declspec(dllimport)" "" __DLL_IMPORT_REPLACED "${__MINPACKEX_EXPORT_HEADER_CONTENT}")
    file(WRITE "${MINPACKEX_EXPORT_HEADER}" "${__DLL_IMPORT_REPLACED}")
endif()

configure_file(${MINPACKEX_PKGCONFIG_INPUT} ${MINPACKEX_PKGCONFIG} @ONLY)
install(FILES ${MINPACKEX_PKGCONFIG} DESTINATION "${MINPACKEX_INSTALL_PKGCONFIGDIR}")
install(FILES ${MINPACKEX_HEADER_FILES} DESTINATION "${MINPACKEX_INSTALL_INCLUDEDIR}")
install(FILES ${MINPACKEX_LICENSE_FILES} DESTINATION "${MINPACKEX_INSTALL_DOCDIR}")

install(TARGETS ${targets_to_install} EXPORT ${minpackex_targets_export}
    RUNTIME DESTINATION "${MINPACKEX_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
)

install(
    FILES ${minpackex_config_cmake} ${minpackex_config_version_cmake}
    DESTINATION "${MINPACKEX_INSTALL_LIBDIR}/${MINPACKEX_LIBRARY_NAME}-${PROJECT_VERSION}"
)

install(
    EXPORT ${minpackex_targets_export}
    DESTINATION "${MINPACKEX_INSTALL_LIBDIR}/${MINPACKEX_LIBRARY_NAME}-${PROJECT_VERSION}"
)

if (BUILD_TESTING AND USE_COVERAGE AND (NOT MINPACKEX_CONFIG_IS_ELLEGIBLE_FOR_COVERAGE))
    message(WARNING "Unfortunately, your configuration is not supported to run code coverage.")
endif()

message(STATUS "")
message(STATUS "")
message(STATUS " ${MINPACKEX_LIBRARY_NAME} ${PROJECT_VERSION} configuration summary:")
message(STATUS "")
message(STATUS "   Dependencies:")
message(STATUS "")
message(STATUS "     minpack:")
message(STATUS "")
if (minpack_FOUND)
message(STATUS "       Found ......................... : YES")
else()
message(STATUS "       Found ......................... : NO")
endif()
message(STATUS "       Library ....................... : ${minpack_LIBRARY}")
message(STATUS "       Include dir ................... : ${minpack_INCLUDE_DIR}")
message(STATUS "")
message(STATUS "   Build options:")
message(STATUS "")
message(STATUS "     Build type ...................... : ${CMAKE_BUILD_TYPE}")
message(STATUS "     Build shared libs ............... : ${BUILD_SHARED_LIBS}")
message(STATUS "     Build static libs ............... : ${BUILD_STATIC_LIBS}")
if (BUILD_TESTING)
message(STATUS "     Build tests ..................... : ON")
else()
message(STATUS "     Build tests ..................... : OFF")
endif()
if (MINPACKEX_CONFIG_IS_ELLEGIBLE_FOR_COVERAGE)
message(STATUS "")
message(STATUS "   Code coverage options:")
message(STATUS "")
if (USE_COVERAGE)
message(STATUS "     Code coverage on tests .......... : ON")
else()
message(STATUS "     Code coverage on tests .......... : OFF")
endif()
message(STATUS "     Coverage report type ............ : ${COVERAGE_REPORT_TYPE}")
endif()
message(STATUS " ")
message(STATUS "   Install directories:")
message(STATUS " ")
message(STATUS "     Install prefix .................. : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "     Binary dir ...................... : ${MINPACKEX_INSTALL_BINDIR}")
message(STATUS "     Lib dir ......................... : ${MINPACKEX_INSTALL_LIBDIR}")
message(STATUS "     Include dir ..................... : ${MINPACKEX_INSTALL_INCLUDEDIR}")
message(STATUS "     Doc dir ......................... : ${MINPACKEX_INSTALL_DOCDIR}")
message(STATUS "     pkg-config dir .................. : ${MINPACKEX_INSTALL_PKGCONFIGDIR}")
message(STATUS "")
message(STATUS "")