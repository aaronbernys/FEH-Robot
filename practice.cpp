
//encoder testing code
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin10);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
#define COUNTS_PER_REVOLUTION 180
#define WHEEL_DIAMETER_INCHES 3.5
#define COUNTS_PER_INCH (COUNTS_PER_REVOLUTION / (WHEEL_DIAMETER_INCHES * 3.14159))

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
    while(left_encoder.Counts() < counts && right_encoder.Counts() < counts+5); //Allow for some discrepancy between the two encoders

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void ERCMain()
{
    int motor_percent = 50; //Input power level here
    int expected_counts = COUNTS_PER_INCH*15; //Input theoretical counts here

    int x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    move_forward(motor_percent, expected_counts); //see function

    Sleep(2.0); //Wait for counts to stabilize
    //Print out data
    LCD.Write("Theoretical Counts: ");
    LCD.WriteLine(expected_counts);
    LCD.Write("Motor Percent: ");
    LCD.WriteLine(motor_percent);
    LCD.Write("Actual LE Counts: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("Actual RE Counts: ");
    LCD.WriteLine(right_encoder.Counts());
}





//The line following test code
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::Pin2);
AnalogInputPin middle_opto(FEHIO::Pin3);
AnalogInputPin left_opto(FEHIO::Pin4);

void ERCMain()
{
    int x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Analog Optosensor Testing");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Record values for optosensors on and off of the straight line
    // Left Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float leftOptosensorValue = left_opto.Value();
    LCD.Write("Left Optosensor Value:");
    LCD.WriteLine(leftOptosensorValue);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Left Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float leftOptosensorValueOff = left_opto.Value();
    LCD.Write("Left Optosensor Value:");
    LCD.WriteLine(leftOptosensorValueOff);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Repeat process for remaining optosensors, and repeat all three for the curved line values
    // Record values for optosensors on and off of the straight line
    // Left Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place right optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float rightOptosensorValue = right_opto.Value();
    LCD.Write("Right Optosensor Value:");
    LCD.WriteLine(rightOptosensorValue);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Right Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place right optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float rightOptosensorValueOff = right_opto.Value();
    LCD.Write("Right Optosensor Value:");
    LCD.WriteLine(rightOptosensorValueOff);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Print end message to screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Test Finished");


        // Record values for optosensors on and off of the straight line
    // middle Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place middle optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float middleOptosensorValue = middle_opto.Value();
    LCD.Write("Middle Optosensor Value:");
    LCD.WriteLine(middleOptosensorValue);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Right Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place middle optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to the screen
    float middleOptosensorValueOff = middle_opto.Value();
    LCD.Write("Middle Optosensor Value:");
    LCD.WriteLine(middleOptosensorValueOff);
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Print end message to screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Test Finished");
}







//our team's code

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRCS.h>

#define COUNTS_PER_REVOLUTION 180.0
#define WHEEL_DIAMETER_INCHES 3.5
#define COUNTS_PER_INCH (COUNTS_PER_REVOLUTION / (WHEEL_DIAMETER_INCHES * 3.14159))
#define ONE_DEGREE_INCH (7.5*3.14159*(1.0/360.0))
#define COUNTS_PER_DEGREE (COUNTS_PER_INCH * ONE_DEGREE_INCH)


//DigitalEncoder right_encoder(FEHIO::Pin8);
//DigitalEncoder left_encoder(FEHIO::Pin10);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
//DigitalInputPin backright(FEHIO::Pin0);
//DigitalInputPin backleft(FEHIO::Pin2);
//DigitalInputPin frontleft(FEHIO::Pin4);
//DigitalInputPin frontright(FEHIO::Pin6);
//AnalogInputPin CdS_cell (FEHIO::Pin12);
//DigitalOutputPin light(FEHIO::Pin14);


void move_forward(int percent, double inches) //using encoders
{
  double counts = COUNTS_PER_INCH * inches; //Calculate the number of counts needed to move the desired distance  
  
  //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
    while(left_encoder.Counts() < counts && right_encoder.Counts() < counts+5); //Allow for some discrepancy between the two encoders

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


void turnOnLight(){
  light.Toggle();
}

void backalign(){
  while(backright.Value() || backleft.Value()){
    right_motor.SetPercent(50);
    left_motor.SetPercent(-50);//move backwards
    if(!backright.Value() && backleft.Value()){
      right_motor.Stop();
    }
    if(!backleft.Value() && backright.Value()){
      left_motor.Stop();
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
    Sleep(5);
    if(pose->x <= x){
      reached = true;
    }
  }
}

void ERCMain()
{ 

  pressStartLight();
  //go forward until it reaches a certain x position
  moveToXPos(16);

}



//explroation 03

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
  int i = 0;
  if(percent > 0){
    rightMotor.SetPercent(-percent + 10 + i);
    leftMotor.SetPercent(percent);
    i+= 0.6;
  }
  if(percent < 0){
    rightMotor.SetPercent(-percent - 10 - i);
    leftMotor.SetPercent(percent);
    i+= 0.6;
  }
  
}

void stop(){
  rightMotor.SetPercent(0);
  leftMotor.SetPercent(0);
}

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRCS.h>
#include <FEHSD.h>

// RCS Delay time
#define RCS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

// Defines for pulsing the robot
#define PULSE_TIME 0.5
#define PULSE_POWER 50

// Define for the motor power
#define POWER 50

// Orientation of AruCo Code
#define PLUS 0
#define MINUS 1

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
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
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Move forward using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void move_forward(int percent, int counts) //using encoders
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

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

/* 
 * Use RCS to move to the desired heading
 */
void check_heading(float heading)
{
    //You will need to fill out this one yourself and take into account
    //checking for proper RCS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    /*
        SUGGESTED ALGORITHM:
        1. Check the current orientation of the QR code and the desired orientation of the QR code

        2. Check if the robot is within the desired threshold for the heading based on the orientation
        3. Pulse in the correct direction based on the orientation
    */
}

#include <FEHSD.h>

void ERCMain()
{
    int touch_x,touch_y;
    float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    float A_heading, B_heading, C_heading, D_heading;
    int B_C_counts, C_D_counts, turn_90_counts;

    RCS.InitializeTouchMenu("Z1TESTING");

    LCD.WriteLine("RCS & Data Logging Test");
    LCD.WriteLine("Press Screen To Start");
    while(!LCD.Touch(&touch_x,&touch_y));
    while(LCD.Touch(&touch_x,&touch_y));

    // COMPLETE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    FEHFile* fptr = SD.FOpen("RCS_TEST.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);

    // WRITE CODE HERE TO SET THE HEADING DEGREES AND COUNTS VALUES
    A_heading = 90;
    B_heading = 180;
    C_heading = 270;
    D_heading = 0;

    B_C_counts = COUNTS_PER_INCH*10;
    C_D_counts = COUNTS_PER_INCH*10;

    turn_90_counts = COUNTS_PER_DEGREE*90;

    // A --> 
    check_y(B_y, PLUS);
    check_heading(B_heading);

    // B --> C
    move_forward(POWER, B_C_counts);
    check_x(C_x, MINUS);
    turn_counterclockwise(POWER, turn_90_counts);
    check_heading(C_heading);

    // C --> D
    move_forward(POWER, C_D_counts);
    check_y(D_y, MINUS);
    turn_counterclockwise(POWER, turn_90_counts);
    check_heading(D_heading);
}

