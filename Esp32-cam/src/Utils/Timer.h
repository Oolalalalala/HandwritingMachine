#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
  Timer();

  float Tick(); // In seconds
  
private:
  unsigned long m_Time;
};

#endif 