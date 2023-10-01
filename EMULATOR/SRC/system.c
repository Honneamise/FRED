#define CHARSET_SIZE 2048
#define PROGRAM_SIZE 0xEFFF

#include "funcs.h"
#include "io.h"
#include "bus.h"
#include "memory.h"
#include "cpu.h"
#include "video.h"
#include "timer.h"
#include "keyboard.h"
#include "clock.h"
#include "audio.h"

#include <stdlib.h>
#include <string.h>

/**********/
void system_init(char *charset_file, char *program_file)
{
    io_init();

    bus_init();

    memory_init();

    cpu_init();

    video_init();

    //init video memory attributes
    // 0xF0 = white on black
    for(int i=1;i<2000;i+=2)
    {
        memory_get_buffer()[i] = 0xF0;
    }

    uint8_t *buffer = NULL;
    size_t buffer_size = 0;

    //load charset
    load_buffer(charset_file, &buffer, &buffer_size);
    if(buffer_size != CHARSET_SIZE){ error("[%s][CHARSET SIZE MISMATCH]", __func__); }
    memcpy(memory_get_buffer()+CHARSET_BASE, buffer, CHARSET_SIZE);
    free(buffer);

    //load program
    load_buffer(program_file, &buffer, &buffer_size);
    if(buffer_size > PROGRAM_SIZE){ error("[%s][PROGRAM EXCEED MAX SIZE]", __func__); }
    memcpy(memory_get_buffer()+PROGRAM_BASE, buffer, buffer_size);
    free(buffer);

    //init remaining devices requiring bus reads/writes
    keyboard_init();
    audio_init();

    //add devices to IO system
    io_add_device(1, keyboard_func, NULL);
    io_add_device(2, clock_func, NULL);
    io_add_device(3, NULL, audio_func);

}

/**********/
void system_close(void)
{
    io_close();

    bus_close();
    
    memory_close();

    cpu_close();

    video_close();

    keyboard_close();

    audio_close();
}

/**********/
void system_run(uint8_t mhz)
{
    double elapsed = 0.0;

    while(video_run())
    {
        double t = timer_get();

        //the screen refresh rate is 60 Hz, so the screen updates every 16.6 ms
        //if we run at 1 Mhz, between 2 screen updates we can execute 16600 cycles

        int32_t available_cycles = 16600 * mhz ;

        while( available_cycles > 0) 
        {
            available_cycles -= cpu_run();
        }

        video_update();

        video_poll();

        elapsed = (timer_get() - t) * 1000.0;

        timer_sleep( (int64_t)(16.6 - elapsed) );        
        
        clock_update();

    }
}