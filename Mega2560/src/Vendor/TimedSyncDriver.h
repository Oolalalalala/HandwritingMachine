/*
 * Synchronous Multi-motor group driver
 * All motors reach their target at the same time.
 *
 * Copyright (C)2017-2019 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */

// Modified to add timing as input

#ifndef TIMED_SYNC_DRIVER_H
#define TIMED_SYNC_DRIVER_H
#include <Arduino.h>
#include "MultiDriver.h"

/*
 * Synchronous Multi-motor group driver class.
 * This driver sets up timing so all motors reach their target at the same time.
 */
class TimedSyncDriver : public MultiDriver {
    using MultiDriver::MultiDriver;

public:

    void startMove(long steps1, long steps2, long steps3=0) override;
    void startMoveTimed(long steps1, long steps2, long duration); // Custom function
};
#endif // TIMED_SYNC_DRIVER_H