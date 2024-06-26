#include "Camera.h"

//#include "../Vendor/OV7670-ESP32/OV7670-ESP32/OV7670.h"
#include <Arduino.h>
#include <driver/i2c.h>

/// wrong values below
#define RESOLUTION QCIF
#define WIDTH 176
#define HEIGHT 144
#define COLOR_MODE RGB565
#define PIXEL_SIZE 1 // How many bytes per pixel


static camera_config_t camera_config = {
    .pin_pwdn       = -1,  // 斷電腳
    .pin_reset      = -1,  // 重置腳
    .pin_xclk       = 17,   // 外部時脈腳
    .pin_sscb_sda   = -1, //21,  // I2C資料腳
    .pin_sscb_scl   = -1, //22,  // I2C時脈腳
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
    .fb_location    = CAMERA_FB_IN_DRAM,
    .grab_mode      = CAMERA_GRAB_WHEN_EMPTY,
    .sccb_i2c_port  = I2C_NUM_0 // I2C埠 ///////////////// Use initialized I2C port
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

    sensor_t* s = esp_camera_sensor_get();
}

void Camera::Shutdown()
{
    if (s_Framebuffer)
        esp_camera_fb_return(s_Framebuffer);

    s_Framebuffer = nullptr;
    esp_camera_deinit();
}

camera_fb_t* Camera::Capture()
{
    if (s_Framebuffer)
        esp_camera_fb_return(s_Framebuffer);
    
    delay(200); // Don't even know why delay is needed, but it seems to work

    s_Framebuffer = esp_camera_fb_get();

    return s_Framebuffer;
}