if (DEFINED minpack_INCLUDE_DIR AND DEFINED minpack_LIBRARY_DIR)
    find_path(minpack_INCLUDE_DIRS
        NAMES minpack.h
        HINTS "${minpack_INCLUDE_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )
    
    find_library(minpack_LIBRARIES
        NAMES minpack
        HINTS "${minpack_LIBRARY_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )
    
    get_filename_component(__minpack_prefix "${minpack_INCLUDE_DIRS}" PATH)

    if (WIN32)
        find_path(minpack_RUNTIME_DIR
            NAMES minpack.dll libminpack.dll
            HINTS "${__minpack_prefix}/bin"
            NO_DEFAULT_PATH
        )
    endif()

    set(minpack_FOUND TRUE)
    set(minpack_LIBRARY "${minpack_LIBRARIES}")

elseif(DEFINED minpack_DIR)

    set(minpack_INCLUDE_DIR "${minpack_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
    set(MINPACK_LIBRARY_DIR "${minpack_DIR}/${CMAKE_INSTALL_LIBDIR}")

    find_path(minpack_INCLUDE_DIRS
        NAMES minpack.h
        HINTS "${minpack_INCLUDE_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )

    find_library(minpack_LIBRARIES
        NAMES minpack
        HINTS "${minpack_LIBRARY_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )

    get_filename_component(__minpack_prefix "${minpack_INCLUDE_DIRS}" PATH)

    if (WIN32)
        find_path(minpack_RUNTIME_DIR
            NAMES minpack.dll libminpack.dll
            HINTS "${__minpack_prefix}/bin"
            NO_DEFAULT_PATH
        )
    endif()

    set(minpack_FOUND TRUE)
    set(minpack_LIBRARY "${minpack_LIBRARIES}")

else()
    find_package(minpack QUIET)

    if (NOT minpack_FOUND)
        find_path(minpack_INCLUDE_DIRS
            NAMES minpack.h
            REQUIRED
        )

        find_library(minpack_LIBRARIES
            NAMES minpack
            REQUIRED
        )

        get_filename_component(__minpack_prefix "${minpack_INCLUDE_DIRS}" PATH)

        if (WIN32)
            find_path(minpack_RUNTIME_DIR
                NAMES minpack.dll libminpack.dll
                HINTS "${__minpack_prefix}/bin"
                NO_DEFAULT_PATH
            )
        endif()

        set(minpack_FOUND TRUE)
        set(minpack_LIBRARY "${minpack_LIBRARIES}")
        set(minpack_INCLUDE_DIR "${minpack_INCLUDE_DIRS}")
    endif()
endif()