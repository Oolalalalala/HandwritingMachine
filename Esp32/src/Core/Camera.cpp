#include "Camera.h"

#include "../Vendor/OV7670-ESP32/OV7670-ESP32/OV7670.h"

#define RESOLUTION QCIF
#define WIDTH 176
#define HEIGHT 144
#define COLOR_MODE RGB565
#define PIXEL_SIZE 2 // How many bytes per pixel

struct CameraData
{
    OV7670 Camera = OV7670();
    uint8_t* Framebuffer;
};
static CameraData s_Data;


void Camera::Initialize()
{
    camera_config_t config;
    config.D0 = 32;
    config.D1 = 33;
    config.D2 = 25;
    config.D3 = 26;
    config.D4 = 27;
    config.D5 = 14;
    config.D6 = 12;
    config.D7 = 13;
    config.XCLK = 17;
    config.PCLK = 5;
    config.VSYNC = 19;
	config.xclk_freq_hz = 10000000;			// XCLK 10MHz
	config.ledc_timer = LEDC_TIMER_0;
	config.ledc_channel = LEDC_CHANNEL_0;

    esp_err_t err = s_Data.Camera.init(&config, RESOLUTION, COLOR_MODE);
    if (err != ESP_OK)
    {
        Serial.println("Failed to initialize camera");
        return;
    }

    s_Data.Framebuffer = (uint8_t*)malloc(WIDTH * HEIGHT * PIXEL_SIZE + 10000);
}

void Camera::Shutdown()
{
    s_Data.Camera.stop();
    free(s_Data.Framebuffer);
}

void Camera::Capture()
{
    s_Data.Camera.getFrame(s_Data.Framebuffer);
}

uint8_t* Camera::GetFramebuffer()
{
    return s_Data.Framebuffer;
}

uint32_t Camera::GetFramebufferSize()
{
    return WIDTH * HEIGHT * PIXEL_SIZE;
}