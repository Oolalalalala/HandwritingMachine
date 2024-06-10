#include "SerialInterface.h"

#include <HardwareSerial.h>
#include "../Utils/Math.h"

#define RECIEVE_END_BUFFER_SIZE 64 // Serial2 buffer size of ESP32-cam, max value is 256
#define TRANSMISION_BEGIN_BYTE 0x01
#define TRANSMISION_END_BYTE 0 // Length of 0 means end of transmission
#define ACKNOWLEDGE_BYTE 0x06

#define RXD 14
#define TXD 15

void SerialInterface::Initialize()
{
    Serial2.begin(9600, SERIAL_8N1, RXD, TXD);
}

void SerialInterface::Acknowledge()
{
    Serial2.write(ACKNOWLEDGE_BYTE);
}

void SerialInterface::WaitForAcknowledge()
{
    while (true)
    {
        if (Serial2.available())
        {
            if (Serial2.read() == ACKNOWLEDGE_BYTE)
                return;
        }
    }
}

uint8_t SerialInterface::ReadNextByte()
{
    while (true)
    {
        if (Serial2.available())
            return Serial2.read();
    }
}

void SerialInterface::SendBytes(const uint8_t* data, int size)
{  
    // Clear read buffer
    while (Serial2.available())
        Serial2.read();

    Serial2.write(TRANSMISION_BEGIN_BYTE);

    WaitForAcknowledge();

    while (size > 0)
    {
        uint8_t segmentLength = min(size, RECIEVE_END_BUFFER_SIZE - 1);

        Serial2.write(segmentLength);
        Serial2.write(data, segmentLength);

        data += segmentLength;
        size -= segmentLength;

        WaitForAcknowledge();
    }

    Serial2.write(TRANSMISION_END_BYTE);

    WaitForAcknowledge();
}

uint8_t* SerialInterface::ReadBytes(uint8_t* buffer, int size)
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

bool SerialInterface::Incoming()
{
    return Serial2.available() && Serial2.peek() == TRANSMISION_BEGIN_BYTE;
}