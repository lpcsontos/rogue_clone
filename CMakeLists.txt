cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)   # <-- rename "MyGame" to whatever you like

# ---- Options ----
# Steam multiplayer is off by default so the project builds without the SDK.
# Turn it on later with:  cmake -B build -DUSE_STEAM=ON
option(USE_STEAM "Link the Steamworks SDK for Steam multiplayer / joining friends" OFF)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put the built executable in build/bin so it's easy to find
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---- raylib ----
# Downloaded and built for you on first configure. Works the same on
# Linux (GCC/Clang) and Windows (MSVC or MinGW).
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0            # latest stable release
    GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # we only want the library
FetchContent_MakeAvailable(raylib)

# raygui: immediate-mode UI (buttons, sliders, checkboxes...). Header-only,
# so we just download it and add its src/ folder to the include path below.
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 5.0            # pairs with raylib 6.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raygui)

# ---- Our sources ----
# Every .cpp under impl/ gets compiled. CONFIGURE_DEPENDS makes CMake
# re-scan for new files on the next build, so you don't edit this file
# every time you add one.
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp"
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${GAME_SOURCES}
)

# .hpp files live in include/  ->  lets you write #include "foo.hpp"
# raygui_SOURCE_DIR/src holds raygui.h  ->  lets you #include "raygui.h"
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${raygui_SOURCE_DIR}/src"
)

# raylib's headers become available to EVERY file in this target
# (main.cpp, impl/*.cpp, and any .hpp they include), so you can
# #include "raylib.h" anywhere in the project.
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# raylib needs a few system frameworks on macOS. Harmless safety net;
# raylib's own CMake usually handles this. (Linux/Windows deps are
# propagated automatically by the raylib target.)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()

# =========================================================================
#  Steamworks (optional)  --  for Steam multiplayer / inviting friends
# =========================================================================
# The Steamworks SDK is proprietary and can't be auto-downloaded:
#   1. Download it:  https://partner.steamgames.com/downloads/list
#   2. Unzip so you have:  <project>/third_party/steamworks_sdk/sdk/
#   3. Configure with:     cmake -B build -DUSE_STEAM=ON
#
# In your code, guard Steam calls with  #ifdef USE_STEAM  so the game
# still compiles when the flag is off.
if(USE_STEAM)
    set(STEAMWORKS_SDK_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/steamworks_sdk/sdk"
        CACHE PATH "Path to the unzipped Steamworks SDK 'sdk' folder")

    if(NOT EXISTS "${STEAMWORKS_SDK_DIR}/public/steam/steam_api.h")
        message(FATAL_ERROR
            "USE_STEAM is ON but the Steamworks SDK was not found at:\n"
            "  ${STEAMWORKS_SDK_DIR}\n"
            "Download it, or point at it with -DSTEAMWORKS_SDK_DIR=/path/to/sdk")
    endif()

    if(WIN32)
        set(STEAM_LINK_LIB "${STEAMWORKS_SDK_DIR}/redistributable_bin/win64/steam_api64.lib")
        set(STEAM_RUNTIME  "${STEAMWORKS_SDK_DIR}/redistributable_bin/win64/steam_api64.dll")
    else() # Linux (64-bit)
        set(STEAM_LINK_LIB "${STEAMWORKS_SDK_DIR}/redistributable_bin/linux64/libsteam_api.so")
        set(STEAM_RUNTIME  "${STEAM_LINK_LIB}")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE "${STEAMWORKS_SDK_DIR}/public")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${STEAM_LINK_LIB}")
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_STEAM)

    # Steam's shared library must sit next to the executable at runtime.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${STEAM_RUNTIME}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying Steam runtime library next to the executable")
endif()

# =========================================================================
#  Packaging / distribution  (CPack)
# =========================================================================
# Make the Windows .exe fully standalone by statically linking the MSVC
# runtime, so friends don't need the "VC++ Redistributable" installed.
# ---- Standalone runtime linking (so friends need nothing pre-installed) ----
if(MSVC)
    # Static MSVC runtime -> no "VC++ Redistributable" needed.
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # MinGW, including cross-compiling from Linux -> fold libgcc / libstdc++ /
    # winpthread into the .exe so it runs on a bare Windows machine.
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libgcc -static-libstdc++ -static)
endif()

# =========================================================================
#  Windows packaging (CPack)  --  works natively OR cross-built from Linux
# =========================================================================
# On Linux the deliverable is just the plain executable in build/bin/ and you
# don't run cpack at all. The block below only activates when building Windows
# binaries -- WIN32 is true both for a native Windows build and for a MinGW
# cross-build started with the toolchain file (see mingw-w64-toolchain.cmake).
if(WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

    # Ship game assets alongside the exe (uncomment when you have an assets/ dir):
    # install(DIRECTORY assets/ DESTINATION assets)

    # Steam's shared library must ship next to the exe.
    if(USE_STEAM)
        install(FILES "${STEAM_RUNTIME}" DESTINATION .)
    endif()

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "0.1.0")
    set(CPACK_PACKAGE_VENDOR "YourName")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A game made with raylib")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
    # Drives Start Menu shortcut + installer "run now" option: <exe-name> <label>
    set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

    # ZIP  -> portable folder you zip and send as-is
    # NSIS -> a setup.exe (installs to Program Files + desktop/Start Menu icons)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${PROJECT_NAME}.exe")
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$DESKTOP\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\${PROJECT_NAME}.exe'")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$DESKTOP\\\\${PROJECT_NAME}.lnk'")

    include(CPack)
endif()
