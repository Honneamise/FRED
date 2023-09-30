cmake_minimum_required(VERSION 3.2)

project(FRED_EMU C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TC /W3 /MTd")

file(GLOB SOURCES SRC/*.c)

include_directories(EXT/INCLUDE)

link_directories(EXT/LIB)

add_executable(FRED_EMU ${SOURCES})

target_link_libraries(FRED_EMU opengl32)
target_link_libraries(FRED_EMU /GLFW/glfw3dll.lib)
target_link_libraries(FRED_EMU /OPENAL/OpenAL32.lib)

set_target_properties(FRED_EMU PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64")

set_target_properties(FRED_EMU PROPERTIES OUTPUT_NAME "FRED_EMU")
