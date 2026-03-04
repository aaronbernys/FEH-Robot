#include <Arduino.h>
#include <FEH.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

FEHMotor rightMotor(FEHMotor::Motor0, 7.2);
FEHMotor leftMotor(FEHMotor::Motor1, 7.2);

void drive(int percent);

void ERCMain() {
  drive(50);
  Sleep(2.0);
  drive(0);
}

void drive(int percent) {
  rightMotor.SetPercent(percent);
  leftMotor.SetPercent(percent);
}