#include "SetDrawRegionState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"

void SetDrawRegionState::OnEnter()
{

}

void SetDrawRegionState::OnUpdate(float dt)
{
    IO::PullData();
}

void SetDrawRegionState::OnExit()
{

}