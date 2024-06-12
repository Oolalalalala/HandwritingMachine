#ifndef ALLOW_CONNECTION_STATE_H
#define ALLOW_CONNECTION_STATE_H

class AllowConnectionState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt);
    static void OnExit();
};

#endif