#ifndef STATE_H
#define STATE_H

// All states are statically allocated
enum class State
{
    None = 0,
    Menu,
    PCControl,
    ManualWriting,
    TicTacToe,
    Photo,
    AutoCalibration,
    ManualCalibration,
    SetDrawRegion, 
    ConnectToWifi,
    AcceptConnection
};

#endif