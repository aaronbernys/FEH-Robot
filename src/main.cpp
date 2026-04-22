
#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRCS.h>
#include <FEHServo.h>

#define COUNTS_PER_REVOLUTION 180.0
#define WHEEL_DIAMETER_INCHES 3.5
#define COUNTS_PER_INCH (COUNTS_PER_REVOLUTION / (WHEEL_DIAMETER_INCHES * 3.14159))
#define ONE_DEGREE_INCH (7*3.14159/(360.0))
#define COUNTS_PER_DEGREE (COUNTS_PER_INCH * ONE_DEGREE_INCH)
// RCS Delay time
#define RCS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

// Defines for pulsing the robot
#define PULSE_TIME 0.1
#define PULSE_POWER 50

// Define for the motor power
#define POWER 50

// Orientation of AruCo Code
#define PLUS 0
#define MINUS 1


DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin12);
FEHMotor right_motor(FEHMotor::Motor0,7.2);
FEHMotor left_motor(FEHMotor::Motor1,7.2);
DigitalInputPin backright(FEHIO::Pin0);
DigitalInputPin backleft(FEHIO::Pin4);///avoid pins 2 and 3 in general!
DigitalInputPin frontleft(FEHIO::Pin6);
DigitalInputPin frontright(FEHIO::Pin6);
AnalogInputPin CdS_cell (FEHIO::Pin7);
FEHServo arm(FEHServo::Servo0);
FEHMotor wheel(FEHMotor::Motor2,7.2);
AnalogInputPin right_opto(FEHIO::Pin2);
AnalogInputPin middle_opto(FEHIO::Pin4);
AnalogInputPin left_opto(FEHIO::Pin6);
DigitalOutputPin led(FEHIO::Pin8);





void move_forward(int percent, double inches) //using encoders
{
  double counts = COUNTS_PER_INCH * (inches/4); //Calculate the number of counts needed to move the desired distance  
  
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent-10);
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

void continuousMoveFWD(int percent){
  while(true){
    move_forward(percent, 1); // Move forward indefinitely
  }
}

void continuousMoveBWD(int percent){


  while(true){
    move_backward(percent, 1); // Move backward indefinitely
  }
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
void timedMove(int percent, double seconds){
  right_motor.SetPercent(-percent);
  left_motor.SetPercent(percent);
  Sleep(seconds);
    right_motor.Stop();
    left_motor.Stop();
}

void turn_Left(int percent, int degrees){
  double leftCounts = COUNTS_PER_DEGREE * degrees/2; 
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(left_encoder.Counts() < leftCounts && right_encoder.Counts() < leftCounts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_Right(int percent, int degrees){
  double rightCounts = COUNTS_PER_DEGREE * degrees/2; 
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(left_encoder.Counts() < rightCounts && right_encoder.Counts() < rightCounts); //Allow for some discrepancy between the two encoders

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
     while(CdS_cell.Value() != 0){
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

    arm.SetDegree(145); //lower arm to press

    //keep running motors
    while(left_encoder.Counts() < Counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    left_motor.Stop();

    left_encoder.ResetCounts();
    left_motor.SetPercent(percent); //GO BACK 
    while(left_encoder.Counts() < Counts);
    left_motor.Stop();

    arm.SetDegree(55); //raise arm back up
}

void hingeLeft(int percent, int degrees){
  double Counts = COUNTS_PER_DEGREE * degrees; 
  //Reset encoder counts
    right_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);

    //keep running motors
    while(right_encoder.Counts() < Counts); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();

    right_encoder.ResetCounts();
    right_motor.SetPercent(percent); //GO BACK 
    while(right_encoder.Counts() < Counts);
    right_motor.Stop();

}

void lightInteraction(){
    waitForLight();
    if (detectLightColor() == 1) {
      //move forward a little bit, then turn on red light
      move_forward(60, 2.0);  
      LCD.Clear(RED);
      //hinge right for blue light
      hingeRight(50, 75);
    } else {
      //move forward a little bit, then turn on blue light
      move_forward(60, 2.0);  
      LCD.Clear(BLUE);
      //hinge left for blue light
      hingeLeft(50, 75);
    }
}

void pressStartLight(){
  //waitForLight();  
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
    arm.SetDegree(55);
    LCD.WriteLine("Reattach the arm at the lowest point it can go on the robot, then press the screen");
    int x, y;
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpresse
    LCD.Clear(GREEN);
    Sleep(0.5);
    LCD.Clear(BLACK);
}

void pulseFWD(){
  move_forward(50, 0.25);
}


/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts) 
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/* 
 * Use RCS to move to the desired x_coordinate based on the orientation of the AruCo code
 */
void check_x(float x_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the AruCo code 
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    RCSPose* pose = RCS.RequestPosition();

    // Check if receiving proper RCS coordinates and whether the robot is within an acceptable range
    for (int i = 0; i < 10; i++) {
        if(pose->x > 0 && (pose->x < x_coordinate - 1 || pose->x > x_coordinate + 1))
        {
            if(pose->x > x_coordinate - 1)
            {
                // Pulse the motors for a short duration in the correct direction
                pulse_forward(-power, PULSE_TIME);
            }
            else if(pose->x < x_coordinate + 1)
            {
                // Pulse the motors for a short duration in the correct direction
                pulse_forward(power, PULSE_TIME);
            }
            Sleep(RCS_WAIT_TIME_IN_SEC);

            pose = RCS.RequestPosition();
        }
}
}


/* 
 * Use RCS to move to the desired y_coordinate based on the orientation of the QR code
 */
void check_y(float y_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    RCSPose* pose = RCS.RequestPosition();

    // Check if receiving proper RCS coordinates and whether the robot is within an acceptable range
    for (int i = 0; i < 10; i++) {
        if(pose->y > 0 && (pose->y < y_coordinate - 1 || pose->y > y_coordinate + 1))
        {
            if(pose->y > y_coordinate + 1)
            {
                // Pulse the motors for a short duration in the correct direction
                pulse_forward(-power, PULSE_TIME);
            }
            else if(pose->y < y_coordinate - 1)
            {
                // Pulse the motors for a short duration in the correct direction
                pulse_forward(power, PULSE_TIME);
            }
            Sleep(RCS_WAIT_TIME_IN_SEC);
            
            pose = RCS.RequestPosition();
        }
    }   
}

void check_heading(float heading)
{
    RCSPose* pose = RCS.RequestPosition();

    // Try up to 10 
    for(int i = 0; i < 10; i++)
    {
        // Ensure valid RCS data
        if(pose->heading < 0)
        {
            Sleep(RCS_WAIT_TIME_IN_SEC);
            pose = RCS.RequestPosition();
            continue;
        }
        float current = pose->heading;
        float error = heading - current;
        if(error > 180) error -= 360;
        if(error < -180) error += 360;

        // If within ±3 degrees, stop correcting
        if(fabs(error) < 3)
            return;

        //direction to pulse
        if(error > 0)
        {
            pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
        }
        else
        {
            pulse_counterclockwise(-PULSE_POWER, PULSE_TIME);
        }

        Sleep(RCS_WAIT_TIME_IN_SEC);
        pose = RCS.RequestPosition();
    }
}

void pickBasket(){
  
 // while(true){
    
    arm.SetDegree(147);//}
    Sleep(0.5);
    for(int i = 0; i < 3; i++){
        pulse_forward(50, 0.5);
        Sleep(0.01);
    }
    arm.SetDegree(80);//go back up-ish
    Sleep(0.5);
   
}


/* TODO:
    - test backalign
    - get from wheel to apple basket
    - figure out ground sequence
      - get apple basket
      - close the doors and open w wheel


*/

void ERCMain(){

  RCS.InitializeTouchMenu("0150F2QWD");
  //WaitForFinalAction();
    //milestone 05 sequence;
   pressStartLight();
 
    //code for the cimpost bin align strategy
    move_forward(50, 8.0); //move forward a little bit to get out of button
    turn_Left(50, 20);
    move_forward(50,15.0);//first call
    turn_Left(50,100);
    move_forward(50, 15.0); //move near compost bin
    wheel.SetPercent(50); //spin wheel to knock over compost bin
    Sleep(2.0);
    wheel.Stop();
    Sleep(0.1);
    wheel.SetPercent(-50); //spin wheel back the other way to reset
    Sleep(2.5);
    wheel.Stop();
    wheel.SetPercent(50); //adjust to go back up
    Sleep(0.5);
    wheel.Stop();

 
    //after compost bin, head to apple basket
    move_backward(50, 3.5); //back up near the basket
    turn_Right(50, 15); //turn to face basket
    move_backward(50, 1.0); //move back a little bit to get out of the way of the basket
    Sleep(0.5);
    arm.SetDegree(145); //lower arm to knock off basket
    turn_Right(50, 100); //move towards basket
    move_forward(50, 15.0); //move into basket
    turn_Right(50, 360); //spin to knock off apples
    arm.SetDegree(55); //lift arm to catch window

    RCSPose* pose = RCS.RequestPosition();//second call
    int orientation = pose->heading;
    int x = pose->x;
    int y = pose->y;
    //face the wall
      turn_Left(50, orientation);
  
    //move to proper y coordinate
    while(!frontleft.Value()){
      move_forward(50, 1.0);
    }


 /* moving to ramp from window
    Sleep(0.5);
    turn_Right(50,90);
    move_forward(50, 10.0); //move forward to catch window
    move_backward(50, 7.0); //back up a little bit to get out of the way of the window
    arm.SetDegree(170); //lower arm to knock off window
    move_forward(50, 10.0); //move forward to scoop w arm
    arm.SetDegree(55); //scoop
    move_backward(50, 8.0); //close window

 */

    //move to the ramp
    move_backward(50, 1.0); //back up a little bit to get out of the way of the wall
    turn_Left(50, 90);
    move_forward(50, 15.0);
    turn_Left(50, 90);
    move_forward(50, 25.0); //move up ramp

    //after ramp
    turn_Left(50, 90);
    backalign();
    move_forward(50, 5.0);//move a little up
    while(CdS_cell.Value() > 4.0){
      pulse_forward(50, 0.1); //pulse forward to slowly catch light
    }
    //detect light
    lightInteraction();  

}