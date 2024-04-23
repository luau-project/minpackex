cmake_minimum_required(VERSION 3.13)

# set default build type as Release
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# basic project properties
project(MINPACKEX
    VERSION "0.1.0"
    LANGUAGES C CXX
)

# find minpack header file
find_path(
    MINPACK_INC
    NAMES minpack.h
    REQUIRED
)

# depend on minpack library
find_library(MINPACK_LIB
    NAMES minpack
    REQUIRED
)

message(STATUS "Found minpack include directory: ${MINPACK_INC}")
message(STATUS "Found minpack library: ${MINPACK_LIB}")

# set the library name
set(MINPACKEX_LIBRARY_NAME "minpackex")

# set the default behavior for building
# the shared or static libraries
# based on different platforms / compilers
if (_WIN32)
    if (MINGW)
        if (NOT DEFINED BUILD_SHARED_LIBS)
            set(BUILD_SHARED_LIBS ON)
        endif()

        if (NOT DEFINED BUILD_STATIC_LIBS)
            set(BUILD_STATIC_LIBS ON)
        endif()
    endif()

    if (MSVC AND BUILD_SHARED_LIBS AND BUILD_STATIC_LIBS)
        message(FATAL_ERROR "The shared and static library cannot be built together.")
    endif()
else()
    if (NOT DEFINED BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS ON)
    endif()

    if (NOT DEFINED BUILD_STATIC_LIBS)
        set(BUILD_STATIC_LIBS ON)
    endif()
endif()

# at least one kind of library must be built
if (NOT (BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS))
    message(FATAL_ERROR "At least the shared or static library must be built (`-DBUILD_SHARED_LIBS=ON` or `-DBUILD_STATIC_LIBS=ON`).")
endif()

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

# pkgconfig input file
set(MINPACKEX_PKGCONFIG_INPUT "${MINPACKEX_LIBRARY_NAME}.pc.cmakein")
set(MINPACKEX_PKGCONFIG "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_LIBRARY_NAME}.pc")

enable_testing()

# build the shared library
if (BUILD_SHARED_LIBS)
    set(MINPACKEX_SHARED_LIB "minpackex_shared")

    add_library(${MINPACKEX_SHARED_LIB} SHARED "")
    target_sources(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})
    target_include_directories(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACK_INC} src)
    target_compile_definitions(${MINPACKEX_SHARED_LIB} PRIVATE "BUILD_SHARED" "BUILDING_MINPACKEX")
    target_link_libraries(${MINPACKEX_SHARED_LIB} PRIVATE ${MINPACK_LIB})

    set_target_properties(${MINPACKEX_SHARED_LIB}
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    install(TARGETS ${MINPACKEX_SHARED_LIB}
        RUNTIME DESTINATION "${MINPACKEX_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
    )

    if (BUILD_TESTING)
        foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
            set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}_shared")
            add_executable(
                ${MINPACKEX_LIB_TEST_NAME}
                "tests/${MINPACKEX_TEST_NAME}-driver.c"
                "tests/${MINPACKEX_TEST_NAME}-minpack.c"
                "tests/${MINPACKEX_TEST_NAME}-${MINPACKEX_LIBRARY_NAME}.c"
            )
            target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src tests ${MINPACK_INC})
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${MINPACKEX_SHARED_LIB} ${MINPACK_LIB})

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
endif()

# build the static library
if (BUILD_STATIC_LIBS)
    set(MINPACKEX_STATIC_LIB "minpackex_static")

    add_library(${MINPACKEX_STATIC_LIB} STATIC "")
    target_sources(${MINPACKEX_STATIC_LIB} PRIVATE ${MINPACKEX_SOURCE_FILES})
    target_include_directories(${MINPACKEX_STATIC_LIB} PRIVATE ${MINPACK_INC} src)
    target_compile_definitions(${MINPACKEX_STATIC_LIB} PRIVATE "BUILDING_MINPACKEX")
    target_link_libraries(${MINPACKEX_STATIC_LIB} PRIVATE ${MINPACK_LIB})

    set_target_properties(${MINPACKEX_STATIC_LIB}
        PROPERTIES
        OUTPUT_NAME ${MINPACKEX_LIBRARY_NAME}
    )

    install(TARGETS ${MINPACKEX_STATIC_LIB}
        RUNTIME DESTINATION "${MINPACKEX_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${MINPACKEX_INSTALL_LIBDIR}"
    )

    if (BUILD_TESTING)
        foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
            set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}_static")
            add_executable(
                ${MINPACKEX_LIB_TEST_NAME}
                "tests/${MINPACKEX_TEST_NAME}-driver.c"
                "tests/${MINPACKEX_TEST_NAME}-minpack.c"
                "tests/${MINPACKEX_TEST_NAME}-${MINPACKEX_LIBRARY_NAME}.c"
            )
            target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src tests ${MINPACK_INC})
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${MINPACKEX_STATIC_LIB} ${MINPACK_LIB})

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
endif()

configure_file(${MINPACKEX_PKGCONFIG_INPUT} ${MINPACKEX_PKGCONFIG} @ONLY)
install(FILES ${MINPACKEX_PKGCONFIG} DESTINATION "${MINPACKEX_INSTALL_LIBDIR}/pkgconfig")
install(FILES ${MINPACKEX_HEADER_FILES} DESTINATION "${MINPACKEX_INSTALL_INCLUDEDIR}")
install(FILES ${MINPACKEX_LICENSE_FILES} DESTINATION "${MINPACKEX_INSTALL_DOCDIR}")