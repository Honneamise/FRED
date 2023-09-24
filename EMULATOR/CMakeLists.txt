cmake_minimum_required(VERSION 3.2)

project(FRED_EMU)

# Flags
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /TC /W3 /MTd")

# Source code directory
file(GLOB SOURCES SRC/*.c)

# Include directory
include_directories(EXT/INCLUDE)

# Library directory
link_directories(EXT/LIB)

# Add the executable target
add_executable(FRED_EMU ${SOURCES})

# Link libraries
target_link_libraries(FRED_EMU opengl32)
target_link_libraries(FRED_EMU /GLFW/glfw3dll.lib)
target_link_libraries(FRED_EMU /OPENAL/OpenAL32.lib)

# Entry point and subsystem
set_target_properties(FRED_EMU PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64")

# Set the output file name
set_target_properties(FRED_EMU PROPERTIES OUTPUT_NAME "FRED_EMU")
