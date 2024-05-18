#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stddef.h>

// A constant size ring buffer
template<typename T, size_t N>
class RingBuffer 
{
public:
    using Capacity = N;

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
        return m_Size == Capacity;
    }

    std::size_t Size() const 
    {
        return m_Size;
    }

    static std::size_t Capacity() const 
    {
        return Capacity;
    }

    void Push(const T& value) 
    {
        m_Buffer[m_Tail] = value;
        m_Tail = (m_Tail + 1) % Capacity;
        m_Size++;
    }

    T Pop() 
    {
        T& value = m_Buffer[m_Head];
        m_Head = (m_Head + 1) % Capacity;
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
        return m_Buffer[(m_Head + index) % Capacity];
    }

private:
    T m_Buffer[Capacity];
    std::size_t m_Size;
    std::size_t m_Head;
    std::size_t m_Tail;
};

#endif