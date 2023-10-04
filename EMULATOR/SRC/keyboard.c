#include "funcs.h"
#include "io.h"
#include "bus.h"
#include "video.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

/*
NOTE FOR CIRCULAR BUFFERS:
if FRONT == BACK --> the buffer is empty
if (FRONT + 1) == BACK --> the buffer is full
*/

#define KEY_BACKSPACE 0x08
#define KEY_TAB       0x09
#define KEY_ENTER     0x0D
#define KEY_DELETE    0x18
#define KEY_ESCAPE    0x1B
#define KEY_RIGHT     0x1C
#define KEY_LEFT      0x1D
#define KEY_UP        0x1E
#define KEY_DOWN      0x1F

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
    io_set(EF2, 1);

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
static uint8_t keyboard_pop(void)
{
    uint8_t next = 0;
    uint8_t data = 0;

    //TODO : keyboard empty, throw an error ?
    if (keyboard->front == keyboard->back) { return data; }

    next = keyboard->back + 1;

    if(next >= KEYBOARD_BUFFER_SIZE){ next = 0; }

    data = keyboard->buffer[keyboard->back];

    keyboard->back = next;

    //if empty set EF2 low
    if (keyboard->front == keyboard->back)
    { 
        io_set(EF2, 0); 
    }

    return data;
}

/**********/
static void keyboard_glfw_char_callback(GLFWwindow *window, unsigned int codepoint)
{
    (void)window;//unused

    if(codepoint < 256)
    {
        keyboard_push((uint8_t)codepoint);
    }
}

/**********/
static void keyboard_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)window;//unused
    (void)scancode;//unused
    (void)mods;//unused

    if(action == GLFW_PRESS)
    {
        uint8_t val = 0;

        switch(key)
        {
            case GLFW_KEY_BACKSPACE:    val = KEY_BACKSPACE;    break;
            case GLFW_KEY_TAB:          val = KEY_TAB;          break;
            case GLFW_KEY_ENTER:        val = KEY_ENTER;        break;
            case GLFW_KEY_DELETE:       val = KEY_DELETE;       break;
            case GLFW_KEY_ESCAPE:       val = KEY_ESCAPE;       break;
            case GLFW_KEY_RIGHT:        val = KEY_RIGHT;        break;
            case GLFW_KEY_LEFT:         val = KEY_LEFT;         break;
            case GLFW_KEY_DOWN:         val = KEY_DOWN;         break;
            case GLFW_KEY_UP:           val = KEY_UP;           break;

            default: break;
        }

        if(val) { keyboard_push(val); }
    }
    
}

/**********/
void keyboard_init(void)
{
    keyboard = alloc(1, sizeof(KEYBOARD));

    glfwSetCharCallback(video_get_window(), keyboard_glfw_char_callback);

    glfwSetKeyCallback(video_get_window(), keyboard_glfw_key_callback);
}

/**********/
void keyboard_close(void)
{
    free(keyboard);
}

/**********/
void keyboard_input_func(void)
{
    uint8_t val = keyboard_pop();

    bus_write(val);

}