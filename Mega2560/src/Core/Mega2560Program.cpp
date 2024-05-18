#include "Mega2560Program.h"

#include "IO.h"

void Mega2560Program::Initialize()
{
    IO::Initialize();
}

void Mega2560Program::OnUpdate()
{
    IO::PullData();

    float dt = m_Timer.Tick();
    
}