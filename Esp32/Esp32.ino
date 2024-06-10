#include "src/Core/Esp32Program.h"
#include "src/Core/WifiInterface.h"

#ifdef TEST_WIFI
const char* ssid = "oolala";
const char* password = "hahahahaha";
#else
Esp32Program program;
#endif

void setup()
{
#ifdef TEST_WIFI
    Serial.begin(9600);
    Serial.println("Initialized");
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

#ifdef TEST_WIFI
char buffer[1024];
#endif

void loop()
{
#ifdef TEST_WIFI
    if (WifiInterface::IncomingFromClient())
    {
        uint8_t* end = WifiInterface::ReadBytesFromClient((uint8_t*)buffer, 1024);
        Serial.write((char*)buffer, (char*)end - buffer);
    }
#else
    program.OnUpdate();
#endif
}