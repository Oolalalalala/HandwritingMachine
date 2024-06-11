#ifndef MANUAL_CALIBRATION_STATE_H
#define MANUAL_CALIBRATION_STATE_H

class ManualCalibrationState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt); // Blocking
    static void OnExit();
};

#endif