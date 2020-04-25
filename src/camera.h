#ifndef CAMERA_H
#define CAMERA_H

void camera_init();
void camera_special_keys(int key, int x, int y);
void camera_special_keys_up(int key, int x, int y);
void camera_draw();
void camera_update(double delta);

#endif