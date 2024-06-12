#include "TicTacToeState.h"

#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"


void TicTacToeState::OnEnter()
{
}

void TicTacToeState::OnUpdate(float dt)
{
    IO::PullData();
}

void TicTacToeState::OnExit()
{

}