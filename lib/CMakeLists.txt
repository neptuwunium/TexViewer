if (TEXVIEWER_ASTC)
    if (NOT (DEFINED ASTC_ARCH))
        set(ASTC_ARCH NATIVE)
    endif()

    string(TOUPPER ${ASTC_ARCH} ASTC_ARCH)
    string(TOLOWER ${ASTC_ARCH} ASTC_ARCH_LOWER)
    if(${ASTC_ARCH_LOWER} STREQUAL "sse41")
        set(ASTC_ARCH_LOWER "sse4.1")
    endif()

    set(ASTCENC_ISA_${ASTC_ARCH} ON)
    set(ASTCENC_CLI OFF)
    set(ASTCENC_UNITTEST OFF)
    set(ASTCENC_DECOMPRESSOR ON)
    set(ASTCENC_UNIVERSAL_BUILD OFF)
    set(ASTC_RAW_TARGET astcdec-${ASTC_ARCH_LOWER}-static)
    set(ASTC_TARGET ${ASTC_RAW_TARGET} PARENT_SCOPE)

    add_subdirectory(astc)

    if (NOT TARGET ${ASTC_RAW_TARGET})
        message(FATAL_ERROR "Couldn't find ASTC library")
    endif()

    target_include_directories(${ASTC_RAW_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/astc/Source)

    if (MSVC)
        target_compile_definitions(${ASTC_RAW_TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(gtest PUBLIC -Wno-deprecated-declarations)
        endif ()
    endif ()
endif()
