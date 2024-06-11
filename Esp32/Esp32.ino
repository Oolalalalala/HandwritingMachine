#include "src/Core/Esp32Program.h"
#include "src/Core/WifiInterface.h"
#include "src/Core/Camera.h"

#define TESTING

#ifdef TESTING
const char* ssid = "oolala";
const char* password = "hahahahaha";
#else
ESP32Program program;
#endif

void setup()
{
#ifdef TESTING
    Serial.begin(9600);
    Serial.println("Initialized");

    Camera::Initialize();
    Serial.println("Attempt take image");
    Camera::Capture();
    Serial.println("Image taken");

    WifiInterface::Initialize();
    WifiInterface::Connect(ssid, password);

    const char* ip = WifiInterface::GetIpAddress().toString().c_str();
    Serial.print("Local IP: ");
    Serial.println(ip);

    WifiInterface::BeginServer();
    WifiInterface::WaitForClientConnection();
    
    
#else
    program.Initialize();
#endif
}

#ifdef TESTING
char buffer[1024];
#endif

void loop()
{
#ifdef TESTING
    if (WifiInterface::IncomingFromClient())
    {
        uint8_t* end = WifiInterface::ReadBytesFromClient((uint8_t*)buffer, 1024);
        Serial.write((char*)buffer, (char*)end - buffer);

        Camera::Capture();
        Serial.println("Image captured");
        WifiInterface::SendBytesToClient(Camera::GetFramebuffer(), Camera::GetFramebufferSize());
    }
#else
    program.OnUpdate();
#endif
}