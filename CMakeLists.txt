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

cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type.")
option(BUILD_SHARED_LIBS "Build the shared library." ON)
option(BUILD_STATIC_LIBS "Build the static library." OFF)

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

find_package(minpack)

# set the library name
set(MINPACKEX_LIBRARY_NAME "minpackex")

##
## Main body
##

# source files
set(MINPACKEX_SOURCE_FILES
    "src/minpackex_dpmpar.c"
    "src/minpackex_enorm.c"
    "src/minpackex_lmdif1.c"
)

# header files
set(MINPACKEX_HEADER_FILES "src/minpackex.h")

# license files
set(MINPACKEX_LICENSE_FILES "LICENSE.md")

# test names
set(MINPACKEX_TEST_NAMES "tlmdif1")

# checking compiler capabilities
include(CheckCSourceCompiles)

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
set(MINPACKEX_PKGCONFIG_INPUT "${MINPACKEX_LIBRARY_NAME}.pc.cmakein")
set(MINPACKEX_PKGCONFIG "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}.pc")

CHECK_C_SOURCE_COMPILES([[
    extern __attribute__ ((visibility ("default"))) int f(void);
    int main(void) { return f(); }
    int f(void) { return 1; }
]] HAVE_VISIBILITY)

enable_testing()

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

# build the shared library
if (BUILD_SHARED_LIBS)
    set(MINPACKEX_SHARED_LIB "minpackex_SHARED")

    add_library(${MINPACKEX_SHARED_LIB} SHARED "")
    target_sources(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})
    target_include_directories(${MINPACKEX_SHARED_LIB} PRIVATE ${minpack_INCLUDE_DIRS} src)
    target_compile_definitions(${MINPACKEX_SHARED_LIB} PRIVATE "BUILDING_SHARED=1" "BUILDING_LIBMINPACKEX=1")

    if (HAVE_VISIBILITY)
        target_compile_definitions(${MINPACKEX_SHARED_LIB} PRIVATE "HAVE_VISIBILITY=1")
    endif()

    if (MSVC)
        target_compile_definitions(${MINPACKEX_SHARED_LIB} PRIVATE "DLL_EXPORT")
    endif()

    target_link_libraries(${MINPACKEX_SHARED_LIB} PRIVATE ${minpack_LIBRARIES})

    set_target_properties(${MINPACKEX_SHARED_LIB}
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    if (DEFINED minpack_SHARED)
        get_filename_component(_minpack_lib_dir "${minpack_LIBRARY}" PATH)
        set(dependencies_lflags "\"-L${_minpack_lib_dir}\" -lminpack")
    endif()

    if (BUILD_TESTING)
        foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
            set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}_shared")
            add_executable(
                ${MINPACKEX_LIB_TEST_NAME}
                "tests/${MINPACKEX_TEST_NAME}-driver.c"
                "tests/${MINPACKEX_TEST_NAME}-minpack.c"
                "tests/${MINPACKEX_TEST_NAME}-${MINPACKEX_LIBRARY_NAME}.c"
            )

            if (WIN32)
                if (TARGET minpack_SHARED AND DEFINED minpack_RUNTIME)
                    if ("${CMAKE_C_SIMULATE_ID}" MATCHES "MSVC")
                        set(__minpack_dll "${minpack_RUNTIME}/minpack.dll")
                    else()
                        set(__minpack_dll "${minpack_RUNTIME}/libminpack.dll")
                    endif()

                    if (EXISTS "${__minpack_dll}")
                        add_custom_command(
                            TARGET ${MINPACKEX_LIB_TEST_NAME}
                            POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${MINPACKEX_LIB_TEST_NAME}> "${__minpack_dll}"
                        )
                    endif()
                endif()
            endif()

            target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src tests ${minpack_INCLUDE_DIRS})
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${MINPACKEX_SHARED_LIB} ${minpack_LIBRARIES})

            if (UNIX)
                target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE m)
            endif()

            set(CURRENT_MINPACKEX_TEST "${MINPACKEX_LIB_TEST_NAME}")
            add_test(
                NAME ${CURRENT_MINPACKEX_TEST}
                COMMAND $<TARGET_FILE:${MINPACKEX_LIB_TEST_NAME}>
                "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-shared-minpack.txt"
                "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-shared-${MINPACKEX_LIBRARY_NAME}.txt"
            )
        endforeach()
    endif()

    list(APPEND targets_to_install ${MINPACKEX_SHARED_LIB})
endif()

# build the static library
if (BUILD_STATIC_LIBS)
    set(MINPACKEX_STATIC_LIB "minpackex_STATIC")

    add_library(${MINPACKEX_STATIC_LIB} STATIC "")
    target_sources(${MINPACKEX_STATIC_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})
    target_include_directories(${MINPACKEX_STATIC_LIB} PRIVATE ${minpack_INCLUDE_DIRS} src)
    target_compile_definitions(${MINPACKEX_STATIC_LIB} PRIVATE "BUILDING_LIBMINPACKEX=1")
    target_link_libraries(${MINPACKEX_STATIC_LIB} PRIVATE ${minpack_LIBRARIES})

    set_target_properties(${MINPACKEX_STATIC_LIB}
        PROPERTIES
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    if (NOT DEFINED dependencies_lflags AND DEFINED minpack_SHARED)
        get_filename_component(_minpack_lib_dir "${minpack_LIBRARY}" PATH)
        set(dependencies_lflags "\"-L${_minpack_lib_dir}\" -lminpack")
    endif()

    if (BUILD_TESTING)
        foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
            set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}_static")
            add_executable(
                ${MINPACKEX_LIB_TEST_NAME}
                "tests/${MINPACKEX_TEST_NAME}-driver.c"
                "tests/${MINPACKEX_TEST_NAME}-minpack.c"
                "tests/${MINPACKEX_TEST_NAME}-${MINPACKEX_LIBRARY_NAME}.c"
            )

            if (WIN32)
                if (TARGET minpack_SHARED AND DEFINED minpack_RUNTIME)
                    if ("${CMAKE_C_SIMULATE_ID}" MATCHES "MSVC")
                        set(__minpack_dll "${minpack_RUNTIME}/minpack.dll")
                    else()
                        set(__minpack_dll "${minpack_RUNTIME}/libminpack.dll")
                    endif()

                    if (EXISTS "${__minpack_dll}")
                        add_custom_command(
                            TARGET ${MINPACKEX_LIB_TEST_NAME}
                            POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${MINPACKEX_LIB_TEST_NAME}> "${__minpack_dll}"
                        )
                    endif()
                endif()
            endif()

            target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src tests ${minpack_INCLUDE_DIRS})
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${MINPACKEX_STATIC_LIB} ${minpack_LIBRARIES})

            if (UNIX)
                target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE m)
            endif()

            set(CURRENT_MINPACKEX_TEST "${MINPACKEX_LIB_TEST_NAME}")
            add_test(
                NAME ${CURRENT_MINPACKEX_TEST}
                COMMAND $<TARGET_FILE:${MINPACKEX_LIB_TEST_NAME}>
                "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-static-minpack.txt"
                "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-static-${MINPACKEX_LIBRARY_NAME}.txt"
            )
        endforeach()
    endif()

    list(APPEND targets_to_install ${MINPACKEX_STATIC_LIB})
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

message(STATUS "")
message(STATUS "")
message(STATUS " ${MINPACKEX_LIBRARY_NAME} ${PROJECT_VERSION} configuration summary:")
message(STATUS "")
message(STATUS "   Dependencies:")
message(STATUS "")
message(STATUS "     minpack:")
message(STATUS "")
message(STATUS "       Found ......................... : ${minpack_FOUND}")
message(STATUS "       Library ....................... : ${minpack_LIBRARY}")
message(STATUS "       Include dir ................... : ${minpack_INCLUDE_DIR}")
message(STATUS "")
message(STATUS "   Build options:")
message(STATUS "")
message(STATUS "     Build type ...................... : ${CMAKE_BUILD_TYPE}")
message(STATUS "     Build shared libs ............... : ${BUILD_SHARED_LIBS}")
message(STATUS "     Build static libs ............... : ${BUILD_STATIC_LIBS}")
if (BUILD_TESTING)
message(STATUS "     Build tests ..................... : ${BUILD_TESTING}")
else()
message(STATUS "     Build tests ..................... : 0")
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