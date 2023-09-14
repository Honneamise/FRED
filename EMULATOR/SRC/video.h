#ifndef VIDEO_H
#define VIDEO_H

void video_init();

void video_close();

void video_poll();

int video_run();

void video_update();

void *video_get_window(); // use cast to GLFWwindow * if needed

#endif