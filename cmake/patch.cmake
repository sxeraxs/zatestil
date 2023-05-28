function(apply_patch PATCH_FILE WORKING_DIR)
    message(STATUS "applying patch ${PATCH_FILE} in ${WORKING_DIR}")
    execute_process(COMMAND git apply --check ${PATCH_FILE}
            WORKING_DIRECTORY ${WORKING_DIR}
            RESULT_VARIABLE SUCCESS
            ERROR_QUIET)

    if (${SUCCESS} EQUAL 0)
        execute_process(COMMAND git apply ${PATCH_FILE}
                WORKING_DIRECTORY ${WORKING_DIR}
                RESULT_VARIABLE SUCCESS)

        if (${SUCCESS} EQUAL 1)
            # We don't stop here because it can happen in case of parallel builds
            message(WARNING "\nError: failed to apply the patch patch: ${PATCH_FILE}\n")
        endif ()
    endif ()
endfunction()
