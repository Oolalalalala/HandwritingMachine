#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include <IPAddress.h>

class WifiInterface
{
public:
    static void Initialize();

    static void TryConnect(const char* ssid, const char* password); // Non-blocking
    static void Connect(const char* ssid, const char* password); // Blocking
    static void Disconnect();

    static bool IsConnected();
    static IPAddress GetIPAddress();

    static void BeginServer();
    static bool ServerEnabled();
    static bool TryClientConnection(); // Non-blocking
    static void WaitForClientConnection();
    static bool HasPreviousConnection();
    static void DumpClient();

    static void SendBytesToClient(const uint8_t* data, unsigned long size);
    static bool IncomingFromClient(); // Returns true if there is a message incoming
    static uint8_t* ReadBytesFromClient(uint8_t* buffer, unsigned long size); // Returns the end

private:
    static void Acknowledge();
    static uint8_t ReadNextByte();
};

#endif 