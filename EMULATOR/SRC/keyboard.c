#include "funcs.h"
#include "io.h"
#include "bus.h"
#include "video.h"
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <GLFW/glfw3.h>

/*
NOTE FOR CIRCULAR BUFFERS:
if FRONT == BACK --> the buffer is empty
if (FRONT + 1) == BACK --> the buffer is full
*/

#define KEYBOARD_BUFFER_SIZE 64

typedef struct KEYBOARD
{
    uint8_t front;
    uint8_t back;
    uint8_t buffer[KEYBOARD_BUFFER_SIZE];

} KEYBOARD;

static KEYBOARD *keyboard = NULL;

/**********/
static void keyboard_push(uint8_t val)
{
    io_set(EF1, 1);

    uint8_t next = 0;

    next = keyboard->front + 1;
    
    if (next >= KEYBOARD_BUFFER_SIZE) { next = 0; }

    //buffer is full, advance back discarding old values
    if (next == keyboard->back)
    {
        keyboard->back++;
        if(keyboard->back >= KEYBOARD_BUFFER_SIZE) { keyboard->back = 0; }
    }

    keyboard->buffer[keyboard->front] = val;

    keyboard->front = next;

}

/**********/
static uint8_t keyboard_pop()
{
    uint8_t next = 0;
    uint8_t data = 0;

    //TODO : keyboard empty, throw an error ?
    if (keyboard->front == keyboard->back) { return data; }

    next = keyboard->back + 1;

    if(next >= KEYBOARD_BUFFER_SIZE){ next = 0; }

    data = keyboard->buffer[keyboard->back];

    keyboard->back = next;

    //if empty set EF1 low
    if (keyboard->front == keyboard->back)
    { 
        io_set(EF1, 0); 
    }

    return data;
}

/**********/
static void keyboard_glfw_char_callback(GLFWwindow *window, unsigned int codepoint)
{
    if(codepoint < 128)
    {
        keyboard_push((uint8_t)codepoint);
    }
}

/**********/
static void keyboard_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //TODO : FIND A SOLUTION FOR DEAD KEYS
}

/**********/
void keyboard_init()
{
    keyboard = alloc(1, sizeof(KEYBOARD));

    glfwSetCharCallback(video_get_window(), keyboard_glfw_char_callback);

    glfwSetKeyCallback(video_get_window(), keyboard_glfw_key_callback);
}

/**********/
void keyboard_close()
{
    free(keyboard);
}

/**********/
void keyboard_func(void *data)
{
    uint8_t val = keyboard_pop();

    bus_write(val);

}