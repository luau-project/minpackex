function(
    setup_minpackex_tests_for_lib_target
    minpackex_lib_target
    minpackex_lib_name
    target_kind
    minpack_dll_dir
    minpack_inc_dirs
    minpack_libs
    )

    # test names
    set(MINPACKEX_TEST_NAMES
        "tchkder"
        "tdpmpar"
        "tenorm"
        "thybrd"
        "thybrd1"
        "thybrj"
        "thybrj1"
        "tlmder"
        "tlmder1"
        "tlmdif"
        "tlmdif1"
        "tlmstr1"
    )
    
    foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
        set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}-${target_kind}")
        add_executable(
            ${MINPACKEX_LIB_TEST_NAME}
            "tests/${MINPACKEX_TEST_NAME}-driver.c"
            "tests/${MINPACKEX_TEST_NAME}-minpack.c"
            "tests/${MINPACKEX_TEST_NAME}-${minpackex_lib_name}.c"
        )

        if (WIN32)
            if (EXISTS "${minpack_dll_dir}")
                if (MSVC OR "${CMAKE_C_SIMULATE_ID}" MATCHES "MSVC")
                    set(__minpack_dll "${minpack_dll_dir}/minpack.dll")
                else()
                    set(__minpack_dll "${minpack_dll_dir}/libminpack.dll")
                endif()

                if (EXISTS ${__minpack_dll})
                    add_custom_command(
                        TARGET ${MINPACKEX_LIB_TEST_NAME}
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${__minpack_dll} ${CMAKE_CURRENT_BINARY_DIR}
                    )
                else()
                    message(WARNING "Unable to find a DLL for minpack. Tests may fail if the dll directory is not on your system environment PATH variable.")
                endif()
            endif()
        endif()

        target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src tests "${minpack_inc_dirs}")
        target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${minpackex_lib_target} "${minpack_libs}")

        if (UNIX)
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE m)
        endif()

        set(CURRENT_MINPACKEX_TEST "${MINPACKEX_LIB_TEST_NAME}")
        add_test(
            NAME ${CURRENT_MINPACKEX_TEST}
            COMMAND $<TARGET_FILE:${MINPACKEX_LIB_TEST_NAME}>
            "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-${target_kind}-minpack.txt"
            "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-${target_kind}-${minpackex_lib_name}.txt"
        )
    endforeach()
endfunction()

