#ifndef CAMERA_H
#define CAMERA_H

// Initialise the camera controller
void camera_init();
// Special keys callback for camera controller
void camera_special_keys(int key, int x, int y);
// Special keys release callback for camera controller
void camera_special_keys_up(int key, int x, int y);
// Call this in the display method, sets opengl camera location based on camera controller
void camera_draw();
// Update the camera with frame delta time
void camera_update(double delta);
// called on window resize
void camera_reshape_func(int width, int height);

#endif