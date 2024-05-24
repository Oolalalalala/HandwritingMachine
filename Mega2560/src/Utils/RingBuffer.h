#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stddef.h>

// A constant size ring buffer
template<typename T, size_t Cap>
class RingBuffer 
{
public:
    RingBuffer() 
        : m_Size(0), m_Head(0), m_Tail(0) 
    { 
    }

    ~RingBuffer() = default;

    bool Empty() const 
    {
        return m_Size == 0;
    }

    bool Full() const 
    {
        return m_Size == Cap;
    }

    size_t Size() const 
    {
        return m_Size;
    }

    static size_t Capacity() 
    {
        return Cap;
    }

    void Push(const T& value) 
    {
        m_Buffer[m_Tail] = value;
        m_Tail = (m_Tail + 1) % Cap;
        m_Size++;
    }

    T Pop() 
    {
        T& value = m_Buffer[m_Head];
        m_Head = (m_Head + 1) % Cap;
        m_Size--;
        return value;
    }

    void Clear()
    {
        m_Size = 0;
        m_Head = 0;
        m_Tail = 0;
    }

    T& operator[](size_t index) 
    {
        return m_Buffer[(m_Head + index) % Cap];
    }

private:
    T m_Buffer[Cap];
    size_t m_Size;
    size_t m_Head;
    size_t m_Tail;
};

#endif