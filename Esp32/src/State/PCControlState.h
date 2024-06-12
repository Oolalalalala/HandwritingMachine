#ifndef PC_CONTROL_STATE_H
#define PC_CONTROL_STATE_H

class PCControlState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt);
    static void OnExit();
};

#endif