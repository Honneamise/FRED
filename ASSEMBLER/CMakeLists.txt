cmake_minimum_required(VERSION 3.2)

project(FRED_ASM)

# Flags
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /TC /W3 /MTd")

# Source code directory
file(GLOB SOURCES SRC/*.c)

# Add the executable target
add_executable(FRED_ASM ${SOURCES})

# Entry point and subsystem
set_target_properties(FRED_ASM PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64")

# Set the output file name
set_target_properties(FRED_ASM PROPERTIES OUTPUT_NAME "FRED_ASM")
