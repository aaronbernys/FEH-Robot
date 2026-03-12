

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>

#define COUNTS_PER_REVOLUTION 180.0
#define WHEEL_DIAMETER_INCHES 3.5
#define COUNTS_PER_INCH (COUNTS_PER_REVOLUTION / (WHEEL_DIAMETER_INCHES * 3.14159))
#define ONE_DEGREE_INCH (7.5*3.14159*(1.0/360.0))
#define COUNTS_PER_DEGREE (COUNTS_PER_INCH * ONE_DEGREE_INCH)


DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin10);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
DigitalInputPin backright(FEHIO::Pin0);
DigitalInputPin backleft(FEHIO::Pin2);
DigitalInputPin frontleft(FEHIO::Pin4);
DigitalInputPin frontright(FEHIO::Pin6);
AnalogInputPin CdS_cell (FEHIO::Pin12);


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


void backalign(){
  while(backright.Value() || backleft.Value()){
    right_motor.SetPercent(50);
    left_motor.SetPercent(-50);//move backwards
  }
  right_motor.Stop();
  left_motor.Stop();
}


void waitForLight(){
    while(CdS_cell.Value() > 3.5){
        LCD.WriteLine("Waiting for light...");
        Sleep(0.1);
    }
}

int detectLightColor(){
    double startTime = TimeNow();
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
      move_forward(80, 2.0);  
      LCD.Clear(RED);
      //hinge right for blue light
      hingeRight(50, 45);
    } else {
      //move forward a little bit, then turn on blue light
      move_forward(80, 2.0);  
      LCD.Clear(BLUE);
      //hinge left for blue light
      hingeLeft(50, 45);
    }
}

void ERCMain()
{ //go forward into button
  move_forward(80, 1.5);
  //back up a little bit
  move_forward(-80, 1.0);
  //turn left 90 degrees
  turn_Left(50, 90);
  //go forward 2 inches
  move_forward(70, 2.0);
  //turn left 45 degrees
  turn_Left(50, 45);
  //go forward 20 inches; get up to the ramp
  move_forward(80, 20.0);
  //turn left 90 degrees
  turn_Left(50, 90);
  //back up inot the wall and align
  backalign();
  //go forward 15.5 inches; move over the light
  move_forward(80, 15.5);
  //wait for the light color, detect it
  //move forward 4 in 
}