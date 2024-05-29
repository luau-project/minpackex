function(
    setup_minpackex_tests_for_lib_target
    minpackex_lib_target
    minpackex_lib_name
    target_kind
    minpack_dll_dir
    minpack_inc_dirs
    minpack_libs
    minpackex_run_coverage
    report_type_for_coverage
    minpackex_lib_coverage_name
)
    # All-in-one test setup

    set(MINPACKEX_ALL_IN_ONE_TEST_NAME "tall-in-one-${target_kind}")
    set(MINPACKEX_ALL_IN_ONE_TEST_ARGS
        "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_ALL_IN_ONE_TEST_NAME}-minpack.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_ALL_IN_ONE_TEST_NAME}-${minpackex_lib_name}.txt"
    )

    add_executable(
        ${MINPACKEX_ALL_IN_ONE_TEST_NAME} "")

    target_sources(
        ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
        PRIVATE
        "tests/tall-in-one-driver.c")
    
    if (minpackex_run_coverage)
        set(__cov_target "${minpackex_lib_name}-${target_kind}-coverage")

        set(${minpackex_lib_coverage_name} ${__cov_target} PARENT_SCOPE)

        append_coverage_compiler_flags_to_target(${MINPACKEX_ALL_IN_ONE_TEST_NAME})
        
        if ("${report_type_for_coverage}" STREQUAL "LCOV")
            setup_target_for_coverage_lcov(
                NAME ${__cov_target}
                EXECUTABLE ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
                EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*"
                EXECUTABLE_ARGS ${MINPACKEX_ALL_IN_ONE_TEST_ARGS}
            )
        elseif ("${report_type_for_coverage}" STREQUAL "GCOVR_HTML")
            setup_target_for_coverage_gcovr_html(
                NAME ${__cov_target}
                EXECUTABLE ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
                EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*"
                EXECUTABLE_ARGS ${MINPACKEX_ALL_IN_ONE_TEST_ARGS}
            )
        elseif ("${report_type_for_coverage}" STREQUAL "GCOVR_XML")
            setup_target_for_coverage_gcovr_xml(
                NAME ${__cov_target}
                EXECUTABLE ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
                EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*"
                EXECUTABLE_ARGS ${MINPACKEX_ALL_IN_ONE_TEST_ARGS}
            )
        elseif ("${report_type_for_coverage}" STREQUAL "FASTCOV")
            setup_target_for_coverage_fastcov(
                NAME ${__cov_target}
                EXECUTABLE ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
                EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*"
                EXECUTABLE_ARGS ${MINPACKEX_ALL_IN_ONE_TEST_ARGS}
            )
        else()
            message(FATAL_ERROR "Unsupported coverage report type.")
        endif()
    endif()
    
    if (WIN32)
        if (EXISTS "${minpack_dll_dir}")
            if (MSVC OR "${CMAKE_C_SIMULATE_ID}" MATCHES "MSVC")
                set(__minpack_dll "${minpack_dll_dir}/minpack.dll")
            else()
                set(__minpack_dll "${minpack_dll_dir}/libminpack.dll")
            endif()

            if (EXISTS ${__minpack_dll})
                add_custom_command(
                    TARGET ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${__minpack_dll} ${CMAKE_CURRENT_BINARY_DIR}
                )
            else()
                message(WARNING "Unable to find a DLL for minpack. Tests may fail if the dll directory is not on your system environment PATH variable.")
            endif()
        endif()
    endif()

    target_include_directories(${MINPACKEX_ALL_IN_ONE_TEST_NAME} PRIVATE src ${CMAKE_CURRENT_BINARY_DIR} tests "${minpack_inc_dirs}")
    target_link_libraries(${MINPACKEX_ALL_IN_ONE_TEST_NAME} PRIVATE ${minpackex_lib_target} "${minpack_libs}")

    if (UNIX)
        target_link_libraries(${MINPACKEX_ALL_IN_ONE_TEST_NAME} PRIVATE m)
    endif()

    # Individual test names
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
        "tlmstr"
        "tlmstr1"
    )

    foreach (MINPACKEX_TEST_NAME ${MINPACKEX_TEST_NAMES})
        
        # add sources to all-in-one test
        target_sources(
            ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
            PRIVATE
            "tests/${MINPACKEX_TEST_NAME}-minpack.c"
            "tests/${MINPACKEX_TEST_NAME}-${minpackex_lib_name}.c")

        
        # setup individual test
        set(MINPACKEX_LIB_TEST_NAME "${MINPACKEX_TEST_NAME}-${target_kind}")

        set(MINPACKEX_LIB_TEST_ARGS
            "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-${target_kind}-minpack.txt"
            "${CMAKE_CURRENT_BINARY_DIR}/${MINPACKEX_TEST_NAME}-${target_kind}-${minpackex_lib_name}.txt"
        )
        
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

        target_include_directories(${MINPACKEX_LIB_TEST_NAME} PRIVATE src ${CMAKE_CURRENT_BINARY_DIR} tests "${minpack_inc_dirs}")
        target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE ${minpackex_lib_target} "${minpack_libs}")

        if (UNIX)
            target_link_libraries(${MINPACKEX_LIB_TEST_NAME} PRIVATE m)
        endif()

        add_test(
            NAME ${MINPACKEX_LIB_TEST_NAME}
            COMMAND $<TARGET_FILE:${MINPACKEX_LIB_TEST_NAME}> ${MINPACKEX_LIB_TEST_ARGS}
        )
    endforeach()

    # add all-in-one as last test
    add_test(
        NAME ${MINPACKEX_ALL_IN_ONE_TEST_NAME}
        COMMAND $<TARGET_FILE:${MINPACKEX_ALL_IN_ONE_TEST_NAME}> ${MINPACKEX_ALL_IN_ONE_TEST_ARGS}
    )

endfunction()