


#include "esp32-hal-ledc.h"
#include "src/Core/SerialInterface.h"
#include "src/Core/WifiInterface.h"
#include "src/Utils/Timer.h"

#define SSID "makerspace-5G-1"
#define PASSWORD "ntueemakerspace"

void setup()
{
    Serial.begin(9600);
    SerialInterface::Initialize();
    SerialInterface::SendBytes((uint8_t*)"Hello from esp32-cam", 20);
    //program.Initialize();
    ledcAttachChannel(4, 1000, 10, 0);

    WifiInterface::Initialize();
    SerialInterface::SendBytes((uint8_t*)"Wifi initialized", 16);
    WifiInterface::Connect(SSID, PASSWORD);
    SerialInterface::SendBytes((uint8_t*)"Wifi connected", 16);

    const char* ip = WifiInterface::GetIpAddress().toString().c_str();
    SerialInterface::SendBytes((uint8_t*)ip, strlen(ip));

    WifiInterface::BeginServer();
    WifiInterface::WaitForClientConnection();
}

char message[16384];

Timer timer;
float t = 0;
int on = 0;

void loop()
{
    if (WifiInterface::IncomingFromClient())
    {
        uint8_t* end = WifiInterface::ReadBytesFromClient((uint8_t*)message, 16384);
        SerialInterface::SendBytes((uint8_t*)message, end - (uint8_t*)message);
    }

    // led
    t += timer.Tick();
    if (t > 1.0f)
    {
        t -= 1.0f;
        if (on)
            on = 0;
        else
            on = 1;
        
        ledcWrite(4, on);
    }
}