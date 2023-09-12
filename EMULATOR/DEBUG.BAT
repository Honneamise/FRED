cl.exe /analyze /W3 /MTd /TC /I INCLUDE SRC/*.c /link opengl32.lib LIB/GLFW/glfw3dll.lib LIB/OPENAL/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:FRED_EMU.EXE 
