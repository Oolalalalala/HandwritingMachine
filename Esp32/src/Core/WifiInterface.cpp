#include "WifiInterface.h"

#include <WiFi.h>

#define PORT 8000
#define ACKNOWLEDGE_BYTE 0x06
#define TRANSMISION_BEGIN_BYTE 0x01
#define TRANSMISION_END_BYTE 0 // Length of 0 means end of transmission

#define CONNECTION_TIMEOUT 10000 // (ms)
#define MAINTAIN_CONNECTION_INTERVAL 1000 // (ms)

struct WifiInterfaceData
{
    WiFiServer Server = WiFiServer(PORT, 1); // 1 client only
    WiFiClient Client = WiFiClient();
    long LastSentByteTime = 0; // (ms)
    bool HasPreviousConnection = false;
};

static WifiInterfaceData s_Data;

void WifiInterface::Initialize()
{
    WiFi.mode(WIFI_STA);
}

void WifiInterface::TryConnect(const char* ssid, const char* password)
{
    WiFi.begin(ssid, password);
}

void WifiInterface::Connect(const char* ssid, const char* password)
{
    /*
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
    */
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        yield();
    }

    Serial.println("Connected to WiFi");
}

void WifiInterface::Disconnect()
{
    s_Data.Client.stop();
    s_Data.Server.end();

    s_Data.Server = WiFiServer(PORT, 1);
    s_Data.Client = WiFiClient();

    WiFi.disconnect();
}

bool WifiInterface::IsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiInterface::GetIPAddress()
{
    return WiFi.localIP();
}

void WifiInterface::BeginServer()
{
    s_Data.Server.begin();
}

bool WifiInterface::ServerEnabled()
{
    return (bool)s_Data.Server;
}

bool WifiInterface::TryClientConnection()
{
    s_Data.Client = s_Data.Server.accept();

    if (s_Data.Client)
        s_Data.HasPreviousConnection = true;

    return s_Data.Client;
}

bool WifiInterface::HasPreviousConnection()
{
    return s_Data.HasPreviousConnection;
}

void WifiInterface::WaitForClientConnection()
{
    while (!s_Data.Client)
    {
        s_Data.Client = s_Data.Server.accept();
    }

    s_Data.HasPreviousConnection = true;
}

void WifiInterface::DumpClient()
{
    if (s_Data.Client)
    {
        s_Data.Client.stop();
    }
    s_Data.Client = WiFiClient();
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

void WifiInterface::SendBytesToClient(const uint8_t* data, unsigned long size)
{
    s_Data.Client.write(TRANSMISION_BEGIN_BYTE);
    
    // 4-byte size (big endian)
    s_Data.Client.write((size >> 24) & 0xFF);
    s_Data.Client.write((size >> 16) & 0xFF);
    s_Data.Client.write((size >> 8) & 0xFF);
    s_Data.Client.write(size & 0xFF);

    s_Data.Client.write(data, size);

    s_Data.LastSentByteTime = millis();
}

bool WifiInterface::IncomingFromClient()
{
    while (s_Data.Client.available())
    {
        if (s_Data.Client.peek() == TRANSMISION_BEGIN_BYTE)
            return true;

        s_Data.Client.read();
    }

    return false;
}

uint8_t* WifiInterface::ReadBytesFromClient(uint8_t* buffer, unsigned long size)
{
    while (true)
    {
        if (ReadNextByte() == TRANSMISION_BEGIN_BYTE)
            break;
    }

    Serial.println("Begin read");

    Acknowledge();

    // Read the main content
    while (true)
    {
        uint8_t segmentLength = ReadNextByte();
        Serial.println(segmentLength);

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