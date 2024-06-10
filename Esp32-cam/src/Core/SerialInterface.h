#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <stdint.h>

class SerialInterface
{
public:
    static void Initialize();

    static void SendBytes(const uint8_t* data, int size);
    static bool Incoming(); // Returns true if there is a message incoming
    static uint8_t* ReadBytes(uint8_t* buffer, int size); // Returns the end

private:
    static void Acknowledge();
    static void WaitForAcknowledge();
    static uint8_t ReadNextByte();
};

#endif