#include <Arduino.h>
#include <FEH.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

FEHMotor rightMotor(FEHMotor::Motor0, 7.2);
FEHMotor leftMotor(FEHMotor::Motor1, 7.2);

void drive(int percent);

void fullTurn() {
  rightMotor.SetPercent(70);
  leftMotor.SetPercent(70);
  Sleep(1.5);
  rightMotor.SetPercent(0);
  leftMotor.SetPercent(0);
}

void drive(int percent) {
  rightMotor.SetPercent(-percent + 15);
  leftMotor.SetPercent(percent);
}

void ERCMain() {
  drive(70);
}



