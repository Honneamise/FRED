#include "funcs.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <GLFW/glfw3.h>

/**********/
double timer_get(void)
{
    return glfwGetTime();
}

/**********/
void timer_sleep(int64_t usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (int64_t)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);

    if(!timer){ error("[%s][INTERNAL TIMER CREATION FAILED]", __func__); return; }
	
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	
    WaitForSingleObject(timer, INFINITE);
	
    CloseHandle(timer);
}