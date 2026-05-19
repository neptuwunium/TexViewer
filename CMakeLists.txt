cmake_minimum_required(VERSION 3.18)
project(TexViewer VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2 /W3 /std:c++20 /Zc:__cplusplus /permissive-")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb")
endif()

option(TEXVIEWER_ASTC "Enable ASTC Decompression" ON)
option(TEXVIEWER_BPTC "Enable S3TC, RGTC and BPTC Decompression (BC1..7)" ON)
option(TEXVIEWER_ETC "Enable ETC Decompression" ON)
option(USE_SYSTEM_BCDEC "Use system bcdec instead of vendored" OFF)

add_subdirectory(lib)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Meta.h.in ${CMAKE_CURRENT_BINARY_DIR}/configured/Meta.h)

file(GLOB TEX_VIEWER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MainWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ImageViewer.h
)

if (NOT TEXVIEWER_BPTC)
    list(REMOVE_ITEM TEX_VIEWER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ConverterBPTC.cpp)
endif()

if (NOT TEXVIEWER_ASTC)
    list(REMOVE_ITEM TEX_VIEWER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ConverterASTC.cpp)
endif()

if (NOT TEXVIEWER_ETC)
    list(REMOVE_ITEM TEX_VIEWER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ConverterETC.cpp)
endif()

set(GLOB TEX_VIEWER_SOURCES_UI CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/MainWindow.ui
)

# Qt stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

add_executable(TexViewer ${TEX_VIEWER_SOURCES} ${TEX_VIEWER_SOURCES_UI})
target_include_directories(TexViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/configured
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Binary-Reader/include
)

if (NOT USE_SYSTEM_BCDEC)
    target_include_directories(TexViewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/bcdec)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(TexViewer Qt6::Core Qt6::Gui Qt6::Widgets)

if (TEXVIEWER_ASTC)
    target_link_libraries(TexViewer ${ASTC_TARGET})
endif()

# Application will launch CMD window if this isn't set
if (MSVC)
    target_link_options(TexViewer PUBLIC /SUBSYSTEM:windows /ENTRY:mainCRTStartup)
endif()

install(
    TARGETS TexViewer
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
