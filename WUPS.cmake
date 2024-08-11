cmake_minimum_required(VERSION 3.18)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED DEVKITPRO OR DEFINED ENV{DEVKITPRO})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/WiiU.cmake" CACHE PATH "Path to devkitPro WiiU cmake toolchain")
    else()
        message(FATAL_ERROR "Please define DEVKITPRO inside your environment variables or pass CMAKE_TOOLCHAIN_FILE explicitly.")
    endif()
endif()

function(wups_create_plugin target)
    set(path "${CMAKE_CURRENT_BINARY_DIR}/${target}")
    set(WUPS_ROOT "${DEVKITPRO}/wups")

    target_include_directories(${target} PRIVATE
        ${WUPS_ROOT}/include
    )

    find_library(LIBWUPS NAMES wups HINTS ${WUPS_ROOT}/lib REQUIRED)
    target_link_libraries(${target} PRIVATE
        ${LIBWUPS}
    )

    target_compile_definitions(${target} PRIVATE
        __WUPS__
    )

    target_link_options(${TARGET} PRIVATE
        "-Wl,-Map,test.map"
        "-T${WUPS_ROOT}/share/wups.ld"
        "-specs=${WUPS_ROOT}/share/wups.specs"
    )

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${path}.elf ${path}.strip.elf
        COMMAND ${CMAKE_STRIP} -g ${path}.strip.elf
        COMMAND ${WUT_ELF2RPL_EXE} ${path}.strip.elf ${path}.wps
        COMMAND ${CMAKE_COMMAND} -E echo 'PL' | dd of=${path}.wps bs=1 seek=9 count=2 conv=notrunc status=none
        COMMAND ${CMAKE_COMMAND} -E rm ${path}.strip.elf
        # COMMAND ${CMAKE_COMMAND} -E echo 'build ... ${path}.wps'
        # COMMENT "Creating ${target}.wps"
    )
endfunction()
