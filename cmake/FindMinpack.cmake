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
    
    get_filename_component(__minpack_prefix "${MINPACK_INCLUDE_DIRS}" PATH)

    find_path(minpack_RUNTIME_DIR
        NAMES minpack.dll libminpack.dll
        HINTS "${__minpack_prefix}/bin"
        NO_DEFAULT_PATH
    )

    set(minpack_FOUND TRUE)

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

    get_filename_component(__minpack_prefix "${MINPACK_INCLUDE_DIRS}" PATH)

    find_path(minpack_RUNTIME_DIR
        NAMES minpack.dll libminpack.dll
        HINTS "${__minpack_prefix}/bin"
        NO_DEFAULT_PATH
    )

    set(minpack_FOUND TRUE)

else()
    find_package(minpack)

    if (NOT minpack_FOUND)
        find_path(minpack_INCLUDE_DIRS
            NAMES minpack.h
            REQUIRED
        )

        find_library(minpack_LIBRARIES
            NAMES minpack
            REQUIRED
        )

        get_filename_component(__minpack_prefix "${MINPACK_INCLUDE_DIRS}" PATH)

        find_path(minpack_RUNTIME_DIR
            NAMES minpack.dll libminpack.dll
            HINTS "${__minpack_prefix}/bin"
            NO_DEFAULT_PATH
        )

        set(minpack_FOUND TRUE)
    endif()
endif()