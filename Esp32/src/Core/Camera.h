#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include <esp_camera.h>

class Camera
{
public:
    static void Initialize();
    static void Shutdown();

    static bool Capture();
    static camera_fb_t* GetFramebuffer();
    static void ReleaseFramebuffer();
};

#endif