#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

void video_init(void);

void video_close(void);

void video_poll(void);

int video_run(void);

void video_update(void);

void *video_get_window(void); // use cast to GLFWwindow * if needed

void video_input_func(void);

void video_output_func(void);

#endif