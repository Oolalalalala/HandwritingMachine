#ifndef TIC_TAC_TOE_STATE_H
#define TIC_TAC_TOE_STATE_H

#include "../Utils/Math.h"

class TicTacToeState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt);
    static void OnExit();

private:
    static void CaptureAndSendPhoto();
    static int WaitAndAcceptForResponse();
    static Vector2Int DecideAction();
};

#endif