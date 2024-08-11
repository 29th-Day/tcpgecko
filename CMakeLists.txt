cmake_minimum_required(VERSION 3.18)

include(./WUPS.cmake)

set(TARGET "tcpgecko")
project(${TARGET} LANGUAGES C CXX ASM)

file(GLOB_RECURSE C_FILES src/*.c)
file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE ASM_FILES src/*.S)

add_executable(${TARGET}
    ${C_FILES}
    ${CPP_FILES}
    ${HEADER_FILES}
    ${ASM_FILES}
    src/link.ld
    # test.c
)

target_include_directories(${TARGET} PRIVATE
    ${DEVKITPRO}/libogc/include
    ${DEVKITPRO}/portlibs/ppc/include
)

target_compile_options(${TARGET} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:-mregnames>
)

target_link_options(${TARGET} PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/src/link.ld"
)

wut_create_rpx(${TARGET})

wut_create_wuhb(${TARGET})
