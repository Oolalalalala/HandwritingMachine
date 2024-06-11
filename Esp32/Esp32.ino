#include "src/Core/Esp32Program.h"
#include "src/Core/WifiInterface.h"
#include "src/Core/Camera.h"



const char* ssid = "oolala";
const char* password = "hahahahaha";
//ESP32Program program;

void setup()
{
    Serial.begin(9600);
    Serial.println("Initialized");
    
    
    Camera::Initialize();
    Serial.print("Free heap after camera: ");
    Serial.println(esp_get_free_heap_size());
    delay(1000);

    WifiInterface::Initialize();
    WifiInterface::Connect(ssid, password);

    const char* ip = WifiInterface::GetIpAddress().toString().c_str();
    Serial.print("Local IP: ");
    Serial.println(ip);

    

    WifiInterface::BeginServer();
    WifiInterface::WaitForClientConnection();
    
    
    //program.Initialize();
}

char buffer[1024];

void loop()
{
    //if (WifiInterface::IncomingFromClient())
    //{
    //uint8_t* end = WifiInterface::ReadBytesFromClient((uint8_t*)buffer, 1024);
    //Serial.write((char*)buffer, (char*)end - buffer);
    //}
        
        delay(100);
        Camera::Capture();
        delay(100);
        camera_fb_t* fb = Camera::GetFramebuffer();
        if (!fb) 
        {
          Serial.println("FB error");
          return;
        }
        Serial.println("Image captured");
        Serial.print(fb->width);
        Serial.print("x");
        Serial.println(fb->height);
        Serial.print("Format: ");
        Serial.println(fb->format);
        Serial.print("Size: ");
        Serial.println(fb->len);
        WifiInterface::SendBytesToClient((uint8_t*)(fb->buf), fb->len);
        Camera::ReleaseFramebuffer();
    //program.OnUpdate();
}