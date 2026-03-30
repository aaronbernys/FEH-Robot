
#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRCS.h>
#include <FEHServo.h>

#define COUNTS_PER_REVOLUTION 180.0
#define WHEEL_DIAMETER_INCHES 3.5
#define COUNTS_PER_INCH (COUNTS_PER_REVOLUTION / (WHEEL_DIAMETER_INCHES * 3.14159))
#define ONE_DEGREE_INCH (7*3.14159*(1.0/360.0))
#define COUNTS_PER_DEGREE (COUNTS_PER_INCH * ONE_DEGREE_INCH)


DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin10);
FEHMotor right_motor(FEHMotor::Motor0,7.2);
FEHMotor left_motor(FEHMotor::Motor1,7.2);
DigitalInputPin backright(FEHIO::Pin0);
DigitalInputPin backleft(FEHIO::Pin2);
DigitalInputPin frontleft(FEHIO::Pin4);
DigitalInputPin frontright(FEHIO::Pin6);
AnalogInputPin CdS_cell (FEHIO::Pin12);
FEHServo arm(FEHServo::Servo0);


void continuousMoveFWD(int percent){
  right_motor.SetPercent(-percent-10);
  left_motor.SetPercent(percent);
}

void continuousMoveBWD(int percent){
  right_motor.SetPercent(percent+30);
  left_motor.SetPercent(-percent);
}

void frontAlign(){
    while(frontleft.Value() || frontright.Value()){
    continuousMoveFWD(50);
    if(!frontright.Value() && frontleft.Value()){
      right_motor.Stop();
      left_motor.SetPercent(30);
    }
    if(!frontleft.Value() && frontright.Value()){
      left_motor.Stop();
      right_motor.SetPercent(30);
    }
  }
  right_motor.Stop();
  left_motor.Stop();
}

void move_forward(int percent, double inches) //using encoders
{
  double counts = COUNTS_PER_INCH * (inches+2); //Calculate the number of counts needed to move the desired distance  
  
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent-17);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
    while(left_encoder.Counts() < counts && right_encoder.Counts() < counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void move_backward(int percent, double inches) //using encoders
{
  double counts = COUNTS_PER_INCH * (inches+2); //Calculate the number of counts needed to move the desired distance  
  
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percen
    right_motor.SetPercent(percent+30);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
    while(left_encoder.Counts() < counts && right_encoder.Counts() < counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void timedMove(int percent, double seconds){
  right_motor.SetPercent(-percent);
  left_motor.SetPercent(percent);
  Sleep(seconds);
    right_motor.Stop();
    left_motor.Stop();
}

void turn_Left(int percent, int degrees){
  double leftCounts = COUNTS_PER_DEGREE * degrees; 
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(left_encoder.Counts() < leftCounts && right_encoder.Counts() < leftCounts+5); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_Right(int percent, int degrees){
  double rightCounts = COUNTS_PER_DEGREE * degrees; 
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(left_encoder.Counts() < rightCounts && right_encoder.Counts() < rightCounts+5); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void backalign(){
  while(backright.Value() || backleft.Value()){
    continuousMoveBWD(50);
    if(!backright.Value() && backleft.Value()){
      right_motor.Stop();
      left_motor.SetPercent(15);
    }
    if(!backleft.Value() && backright.Value()){
      left_motor.Stop();
      right_motor.SetPercent(15);
    }
  }
  right_motor.Stop();
  left_motor.Stop();
}


void waitForLight(){
     while(CdS_cell.Value() > 3.5){
      LCD.Clear();
        LCD.WriteLine("Waiting for light...");
        Sleep(0.1);
    } 
}

int detectLightColor(){
    //double startTime = TimeNow();
    //while(TimeNow() - startTime < 10.0){
    int color = 0;
        LCD.Clear();
        if(CdS_cell.Value() < 1){
        LCD.WriteLine("Red!");
        color = 1; // Return 1 for red
        }
        else{
        LCD.WriteLine("Blue!");
        color = 0; // Return 0 for blue
        }
        Sleep(0.1);
    //}
    return color;
}

void hingeRight(int percent, int degrees){
  double Counts = COUNTS_PER_DEGREE * degrees; 
  //Reset encoder counts
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    left_motor.SetPercent(percent);

    //keep running motors
    while(left_encoder.Counts() < Counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    left_motor.Stop();
}

void hingeLeft(int percent, int degrees){
  double Counts = COUNTS_PER_DEGREE * degrees; 
  //Reset encoder counts
    right_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);

    //keep running motors
    while(right_encoder.Counts() < Counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
}

void lightInteraction(){
    waitForLight();
    if (detectLightColor() == 1) {
      //move forward a little bit, then turn on red light
      move_forward(60, 2.0);  
      LCD.Clear(RED);
      //hinge right for blue light
      hingeRight(50, 45);
    } else {
      //move forward a little bit, then turn on blue light
      move_forward(60, 2.0);  
      LCD.Clear(BLUE);
      //hinge left for blue light
      hingeLeft(50, 45);
    }
}

void pressStartLight(){
  waitForLight();  
  //go back into button
  move_forward(-60, 1.5);
  //move up a little bit
  move_forward(60, 0.5);
}

 void moveToXPos(int x){
  //make a while lopo
  //while robot's x position is less than x, keep moving forward
  RCSPose *pose;
  bool reached = false;
  while(!reached){
    pose = RCS.RequestPosition();
    continuousMoveFWD(50);
    Sleep(5);
    if(pose->x == -1){
        LCD.Clear(RED);
        LCD.WriteLine("No RCS signal");
    }
    if(pose->x <= x){
      reached = true;
    }
  }
  right_motor.Stop();
  left_motor.Stop();
}

void moveToYPos(int y){
  //make a while lopo
  //while robot's y position is less than y, keep moving forward
  RCSPose *pose;
  bool reached = false;
  while(!reached){
    pose = RCS.RequestPosition();
    continuousMoveFWD(50);
    Sleep(5);
    if(pose->y >= y){
      reached = true;
    }
  }
  right_motor.Stop();
  left_motor.Stop();
} 

void reattachArm(){
    arm.SetDegree(180);
    LCD.WriteLine("Reattach the arm at the lowest point it can go on the robot, then press the screen");
    int x, y;
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
}

void ERCMain()
{ 

    arm.SetMin(500); 
    arm.SetMax(2500);
    /* Servo Arm Notes:
    
    - When reattaching the arm:
    1. set the servo to 180 degrees
    2. reattach the arm at the lowest/furthest point it 
    can go on the robot.
    
    - Vertical = 55 degrees
    */
   reattachArm();
   arm.SetDegree(55);
  RCS.DisableRateLimit();
  RCS.InitializeTouchMenu("0150F2QWD");

  pressStartLight();
  //go forward until it reaches a certain x position
  frontAlign();
  turn_Right(50, 90);
  frontAlign();
  move_backward(50, 5);
  turn_Right(50, 90);
  move_forward(30, 5);
  move_backward(30, 10);
  arm.SetDegree(180);
  move_forward(30, 10);
  arm.SetDegree(55);
  move_backward(30, 10);

  LCD.Clear(BLUE);
  Sleep(10.0);

  //code for the cimpost bin align startegy
  turn_Right(50, 50);
  backalign();
  moveToYPos(25);//move_forward(50, 25); //front align method????
  turn_Right(50, 90);
  move_forward(50, 5);
  move_forward(-50, 7);
  //rotate servo arm to flat
  move_forward(50, 10);
  //rotate servo arm to highest point
  move_forward(-50, 5);

  //arm at highest poitn is 10.5 in ches high
  //window door is 6.5-7in high
  //the servo hinge is 6 inches high

}