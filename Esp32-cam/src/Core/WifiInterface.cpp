#include "WifiInterface.h"

#include <WiFi.h>

#define PORT 8000
#define ACKNOWLEDGE_BYTE 0x06
#define TRANSMISION_BEGIN_BYTE 0x01
#define TRANSMISION_END_BYTE 0 // Length of 0 means end of transmission

struct WifiInterfaceData
{
    WiFiServer Server = WiFiServer(PORT, 1); // 1 client only
    WiFiClient Client = WiFiClient();
};

static WifiInterfaceData s_Data;

void WifiInterface::Initialize()
{
    WiFi.mode(WIFI_STA);
}

void WifiInterface::Connect(const char* ssid, const char* password)
{
    int n = WiFi.scanNetworks();
     Serial.println("scan done");
    if (n == 0) {
          Serial.println("no networks found");
    } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        yield();
    }
}

void WifiInterface::Disconnect()
{
    WiFi.disconnect();
}

bool WifiInterface::IsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiInterface::GetIpAddress()
{
    return WiFi.localIP();
}

void WifiInterface::BeginServer()
{
    s_Data.Server.begin();
}

void WifiInterface::WaitForClientConnection()
{
    while (!s_Data.Client)
    {
        s_Data.Client = s_Data.Server.accept();
    }
}

void WifiInterface::Acknowledge()
{
    s_Data.Client.write(ACKNOWLEDGE_BYTE);
}

uint8_t WifiInterface::ReadNextByte()
{
    while (true)
    {
        if (s_Data.Client.available())
            return s_Data.Client.read();
    }
}

void WifiInterface::SendBytesToClient(const uint8_t* data, int size)
{
    s_Data.Client.write(data, size);
}

bool WifiInterface::IncomingFromClient()
{
    return s_Data.Client.available() && s_Data.Client.peek() == TRANSMISION_BEGIN_BYTE;
}

uint8_t* WifiInterface::ReadBytesFromClient(uint8_t* buffer, int size)
{
    while (true)
    {
        if (ReadNextByte() == TRANSMISION_BEGIN_BYTE)
            break;
    }

    Acknowledge();

    // Read the main content
    while (true)
    {
        uint8_t segmentLength = ReadNextByte();

        if (segmentLength == TRANSMISION_END_BYTE)
        {
            Acknowledge();
            return buffer;
        }

        for (int i = 0; i < segmentLength; i++)
        {
            if (size > 0)
                *buffer++ = ReadNextByte();
            else
                ReadNextByte();

            size--;
        }

        Acknowledge();
    }
}