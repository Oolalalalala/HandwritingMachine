#ifndef MANUAL_WRITING_STATE_H
#define MANUAL_WRITING_STATE_H

class ManualWritingState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt); // Blocking
    static void OnExit();
};

#endif