#ifndef AUTO_CALIBRATION_STATE_H
#define AUTO_CALIBRATION_STATE_H

class AutoCalibrationState
{
public:
    static void OnEnter();
    static void OnUpdate(float dt);
    static void OnExit();
};

#endif