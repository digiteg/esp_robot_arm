/*
    
    Braccio.h - Braccio arm I2C communication  library Version 1.0
    modified by Milan Varga
 	
	original Braccio.cpp - board library Version 2.0
 	Written by Andrea Martino and Angelo Ferrante

*/


#ifndef BRACCIO_ ROBOT_H_
#define BRACCIO_ROBOT_H_

#include <Arduino.h>
#include <Servo.h>

// You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
#define SOFT_START_DISABLED -999

//The default value for the soft start
#define SOFT_START_DEFAULT 0

//The software PWM is connected to PIN 12. You cannot use the pin 12 if you are using
//a Braccio shield V4 or newer
#define SOFT_START_CONTROL_PIN 12

//Low and High Limit Timeout for the Software PWM
#define LOW_LIMIT_TIMEOUT 2000
#define HIGH_LIMIT_TIMEOUT 6000

class _BraccioRobot
{

public:
  _BraccioRobot();


  void setupBraccioRobot();


  /**
  * Braccio initializations and set intial position
  * Modifing this function you can set up the initial position of all the
  * servo motors of Braccio 
  *@param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
  * You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
  */
  void begin(int soft_start_level = SOFT_START_DEFAULT);

  /**
   * This function allow the user to control all the servo motors in the Braccio
   */
  int ServoMovement(int delay, int Vbase, int Vshoulder, int Velbow, int Vwrist_ver, int Vwrist_rot, int Vgripper);

  void loopBraccioRobot();

private:
  Servo base;
  Servo shoulder;
  Servo elbow;
  Servo wrist_rot;
  Servo wrist_ver;
  Servo gripper;

  int step_base = 0;
  int step_shoulder = 45;
  int step_elbow = 180;
  int step_wrist_rot = 180;
  int step_wrist_ver = 90;
  int step_gripper = 10;
  int soft_start_level= SOFT_START_DEFAULT;

  /*
  * This function, used only with the Braccio Shield V4 and greater,
  * turn ON the Braccio softly and save Braccio from brokes.
  * The SOFT_START_CONTROL_PIN is used as a software PWM
  * @param soft_start_level: the minimum value is -70, , default value is 0 (SOFT_START_DEFAULT)
  */
  void softStart(int soft_start_level);

  /*
  * Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
  * @param high_time: the time in the logic level high
  * @param low_time: the time in the logic level low
  */
  void softwarePWM(int high_time, int low_time);

  int getlimit(int value, int minv, int maxv);
};

extern _BraccioRobot BraccioRobot;

#endif // BRACCIO_ROBOT_H_
