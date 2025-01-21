set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")

cmake_minimum_required(VERSION 3.10.0)

project(controller-emu VERSION 0.0.1 LANGUAGES C CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking configuration
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(BUILD_SHARED_LIBS OFF)

# Add MSYS2 paths
set(MSYS2_MINGW64_PATH "C:/msys64/mingw64")
set(HIDAPI_INCLUDE_DIR "${MSYS2_MINGW64_PATH}/include")
set(HIDAPI_LIBRARY_DIR "${MSYS2_MINGW64_PATH}/lib")

# Add ViGEmClient as a subdirectory
add_subdirectory("src/ViGEmClient")

# Define your executable
add_executable(controller-emu src/main.cpp src/ControllerManager.cpp src/ControllerParser.cpp ${APP_ICON_RESOURCE})

# Include directories
target_include_directories(controller-emu PRIVATE
    ${HIDAPI_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(controller-emu PRIVATE
    "${HIDAPI_LIBRARY_DIR}/libhidapi.a"  # HIDAPI static library
    ViGEmClient::ViGEmClient  # Explicit target linking#
    Setupapi    # Link against SetupAPI for Windows API functions
)

target_compile_options(controller-emu PRIVATE
    -Wall
    -Wextra
    #-Werror
    -pedantic
    -g
)

