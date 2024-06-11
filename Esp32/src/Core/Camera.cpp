#include "Camera.h"

//#include "../Vendor/OV7670-ESP32/OV7670-ESP32/OV7670.h"
#include <Arduino.h>

#define RESOLUTION QCIF
#define WIDTH 176
#define HEIGHT 144
#define COLOR_MODE RGB565
#define PIXEL_SIZE 2 // How many bytes per pixel


static camera_config_t camera_config = {
    .pin_pwdn       = -1,  // 斷電腳
    .pin_reset      = -1,  // 重置腳
    .pin_xclk       = 17,   // 外部時脈腳
    .pin_sscb_sda   = 3, //21,  // I2C資料腳
    .pin_sscb_scl   = 1, //22,  // I2C時脈腳
    .pin_d7         = 13,  // 資料腳
    .pin_d6         = 12,
    .pin_d5         = 14,
    .pin_d4         = 27,
    .pin_d3         = 26,
    .pin_d2         = 25,
    .pin_d1         = 33,
    .pin_d0         = 32,
    .pin_vsync      = 19,   // 垂直同步腳
    .pin_href       = 18,   // 水平同步腳
    .pin_pclk       = 5,   // 像素時脈腳
    .xclk_freq_hz   = 10000000,       // 設定外部時脈：20MHz
    .ledc_timer     = LEDC_TIMER_0,   // 指定產生XCLK時脈的計時器
    .ledc_channel   = LEDC_CHANNEL_0, // 指定產生XCLM時脈的通道
    .pixel_format   = PIXFORMAT_GRAYSCALE, // 設定影像格式：JPEG
    .frame_size     = FRAMESIZE_QVGA, // 設定影像大小：VGA
    .jpeg_quality   = 12,  // 設定JPEG影像畫質，有效值介於0-63，數字越低畫質越高。
    .fb_count       = 1,    // 影像緩衝記憶區數量
    .fb_location    = CAMERA_FB_IN_DRAM
      };

static camera_fb_t* s_Framebuffer = nullptr;

void Camera::Initialize()
{
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) 
    {
        Serial.print("Camera failed to initialize");
        return;
    }
    
}

void Camera::Shutdown()
{
    if (s_Framebuffer)
        esp_camera_fb_return(s_Framebuffer);

    s_Framebuffer = nullptr;
    esp_camera_deinit();
}

bool Camera::Capture()
{
    if (s_Framebuffer)
        esp_camera_fb_return(s_Framebuffer);

    s_Framebuffer = esp_camera_fb_get();

    delay(100); // Don't even know why delay is needed, but it seems to work

    return s_Framebuffer != nullptr;
}

camera_fb_t* Camera::GetFramebuffer()
{
    return s_Framebuffer;
}

void Camera::ReleaseFramebuffer()
{
    if (s_Framebuffer)
        esp_camera_fb_return(s_Framebuffer);

    s_Framebuffer = nullptr;
}