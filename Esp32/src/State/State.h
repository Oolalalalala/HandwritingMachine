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
    AutoCalibration,
    ManualCalibration,
    SetDrawRegion, 
    ConnectToWifi,
    ShowServerIP
};

#endif