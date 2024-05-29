#include <AccelStepper.h>
#include <MultiStepper.h>

// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
 
#include <AccelStepper.h>
 
AccelStepper stepper(AccelStepper::DRIVER, 31, 33); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
 
void setup()
{  
   stepper.setEnablePin(35);
   
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(200);
}
 
void loop()
{  
   stepper.runSpeed();
}