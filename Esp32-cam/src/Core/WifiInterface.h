#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include <IPAddress.h>

class WifiInterface
{
public:
    static void Initialize();

    static void Connect(const char* ssid, const char* password); // Blocking
    static void Disconnect();

    static bool IsConnected();
    static IPAddress GetIpAddress();

    static void BeginServer();
    static void WaitForClientConnection();
    static bool HasClient();

    static void SendBytesToClient(const uint8_t* data, int size);
    static bool IncomingFromClient(); // Returns true if there is a message incoming
    static uint8_t* ReadBytesFromClient(uint8_t* buffer, int size); // Returns the end

private:
    static void Acknowledge();
    static uint8_t ReadNextByte();
};

#endif 