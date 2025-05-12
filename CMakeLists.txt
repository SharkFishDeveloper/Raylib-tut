# Set compiler for C and C++
set(CMAKE_C_COMPILER "D:/mingw-64x/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/mingw-64x/mingw64/bin/g++.exe")

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(main VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Collect all .c files in the functions directory and its subdirectories
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/index.c
)

# Collect all .h files in the functions directory and its subdirectories
file(GLOB_RECURSE HEADERS
)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    D:/random-packg/raylib-5.5_win64_mingw-w64/raylib-5.5_win64_mingw-w64/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    D:/random-packg/raylib-5.5_win64_mingw-w64/raylib-5.5_win64_mingw-w64/lib/libraylib.a # Full path
    opengl32
    gdi32
    winmm
)

# Optionally add compiler flags for warnings and debugging
target_compile_options(${PROJECT_NAME} PRIVATE )
