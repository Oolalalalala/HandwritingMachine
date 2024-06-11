#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

class Camera
{
public:
    static void Initialize();
    static void Shutdown();

    static void Capture();
    static uint8_t* GetFramebuffer();
    static uint32_t GetFramebufferSize();
};

#endif