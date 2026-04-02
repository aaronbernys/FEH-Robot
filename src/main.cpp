
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

void pulseFWD(){
  move_forward(50, 0.25);
}


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
    pulse_forward(50, 0.5);
    arm.SetDegree(80);//go back vertical
   
}

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHRCS.h>
#include <FEHSD.h>

void ERCMain()
{
  //RCS.DisableRateLimit();
  //RCS.InitializeTouchMenu("0150F2QWD");
  //READING THE LOCATION FILE.
  int touch_x, touch_y;
  float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y, E_x, E_y, F_x, F_y;
  float A_heading, B_heading, C_heading, D_heading, E_heading, F_heading;

    // Open SD file for writing
    FEHFile *fptr = SD.FOpen("locations.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    SD.FScanf(fptr, "%f%f", &E_x, &E_y);
    SD.FScanf(fptr, "%f%f", &F_x, &F_y);


  //define headings
    A_heading = 270;
    B_heading = 180;
    C_heading = 180;
    D_heading = 135;
    //E_heading;
    //F_heading;


    //pressStartLight();
    move_forward(50, 15.0);//move over to spot A

    turn_Right(50, 45);
    pickBasket();


    //in case rcs works again

    /* check_y(A_y, PLUS);
    check_heading(A_heading);
    check_x(A_x, PLUS);
    

    pickBasket();

    turn_Right(50, 90);
    move_backward(50, 10.0);//move 
    turn_Right(50, 90);
    move_forward(50, (B_x-A_x));//move over to spot B


    LCD.Clear(GREEN);

 */




    

        
    // Close SD file
    SD.FClose(fptr);
 
 }