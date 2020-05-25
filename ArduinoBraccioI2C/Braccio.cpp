/*
    
    Braccio.cpp - Braccio arm I2C communication  library Version 1.0
    modified by Milan Varga
 	
	original Braccio.cpp - board library Version 2.0
 	Written by Andrea Martino and Angelo Ferrante

*/

#include "Braccio.h"

_BraccioRobot BraccioRobot;

//----------------------------------------

//Initialize Braccio object
_BraccioRobot::_BraccioRobot()
{
	previousTimeServoMove = millis(); // init internal Timer
}

void _BraccioRobot::setupBraccioRobot()
{
	// initialization pin Servo motors
	base.attach(11);
	shoulder.attach(10);
	elbow.attach(9);
	wrist_rot.attach(6);
	wrist_ver.attach(5);
	gripper.attach(3);
}

/*
	Braccio initialization and set intial position
 	Modifing this function you can set up the initial position of all the
 	servo motors of Braccio
 	@param soft_start_level: default value is 0 (SOFT_START_DEFAULT)
 	You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
 	SOFT_START_DISABLED disable the Braccio movements
 */

void _BraccioRobot::begin(int soft_start_level)
{
	isExit = true;

	//Calling Braccio.begin(SOFT_START_DISABLED) the Softstart is disabled and you can use the pin 12
	if (soft_start_level != SOFT_START_DISABLED)
	{
		pinMode(SOFT_START_CONTROL_PIN, OUTPUT);
		digitalWrite(SOFT_START_CONTROL_PIN, LOW);
	}

	//Stop and set current  step motor position
	step_base = 0;
	step_shoulder = 40;
	step_elbow = 180;
	step_wrist_rot = 0;
	step_wrist_ver = 170;
	step_gripper = 73;

	//For each step motor this set up the initial degree
	base.write(0);
	shoulder.write(40);
	elbow.write(180);
	wrist_rot.write(0);
	wrist_ver.write(170);
	gripper.write(73);

	Stop(); // reset all

	if (soft_start_level != SOFT_START_DISABLED)
		softStart(soft_start_level);

}

void _BraccioRobot::Stop()
{
	step_base = new_step_base;
	step_shoulder = new_step_shoulder;
	step_elbow = new_step_elbow;
	step_wrist_rot = new_step_wrist_rot;
	step_wrist_ver = new_step_wrist_ver;
	step_gripper = new_step_gripper;

	isExit = true;
	isPause = false;
}

void _BraccioRobot::powerOff()
{
	Stop();
	digitalWrite(SOFT_START_CONTROL_PIN, LOW);
}

void _BraccioRobot::powerOn()
{
	Stop();
	digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
}

void _BraccioRobot::pauseOff()
{
	isPause = false;
}

void _BraccioRobot::pauseOn()
{
	isPause = true;
}

/*
	Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
	@param high_time: the time in the logic level high
	@param low_time: the time in the logic level low
*/
void _BraccioRobot::softwarePWM(int high_time, int low_time)
{
	digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
	delayMicroseconds(high_time);
	digitalWrite(SOFT_START_CONTROL_PIN, LOW);
	delayMicroseconds(low_time);
}

/*
	This function, used only with the Braccio Shield V4 and greater,
	turn ON the Braccio softly and save it from brokes.
	The SOFT_START_CONTROL_PIN is used as a software PWM
	@param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
*/
void _BraccioRobot::softStart(int soft_start_level)
{
	long int tmp = millis();
	while (millis() - tmp < LOW_LIMIT_TIMEOUT)
		softwarePWM(80 + soft_start_level, 450 - soft_start_level); //the sum should be 530usec

	while (millis() - tmp < HIGH_LIMIT_TIMEOUT)
		softwarePWM(75 + soft_start_level, 430 - soft_start_level); //the sum should be 505usec

	digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
}

int _BraccioRobot::getlimit(int value, int minv, int maxv)
{
	if (value < minv)
		return minv;

	if (value > maxv)
		return maxv;

	return value;
}

int _BraccioRobot::moveServo(Servo *s, int target, int step)
{
	//For each servo motor if next degree is not the same of the previuos than do the movement

	if (target == step) // on position
		return target;

	s->write(step);

	if (target > step)
	{
		step++; //One step forward
	}
	else
	{
		step--; //One step backward
	}
	return step;
}

/*
 	This functions allow you to control all the servo motors
  
 	@param stepDelay The delay between each servo movement
 	@param vBase next base servo motor degree
 	@param vShoulder next shoulder servo motor degree
 	@param vElbow next elbow servo motor degree
 	@param vWrist_ver next wrist rotation servo motor degree
 	@param vWrist_rot next wrist vertical servo motor degree
 	@param vgripper next gripper servo motor degree
 */
int _BraccioRobot::ServoMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_rot, int vWrist_ver, int vgripper)
{

	// Check values, to avoid dangerous positions for the Braccio

	isExit = true;

	new_step_base = getlimit(vBase, 0, 180);
	new_step_shoulder = getlimit(vShoulder, 15, 165);
	new_step_elbow = getlimit(vElbow, 0, 180);
	new_step_wrist_rot = getlimit(vWrist_rot, 0, 180);
	new_step_wrist_ver = getlimit(vWrist_ver, 0, 180);
	new_step_gripper = getlimit(vgripper, 10, 73);

	timeIntervalServoMove = getlimit(stepDelay, 10, 30); // setup delay to milis
	previousTimeServoMove = millis(); // init internal Timer
	
	isExit = false;										 // enable loop
}

void _BraccioRobot::loopBraccioRobot()
{
	if (isExit || isPause)
		return;

	unsigned long currentTime = millis();

	// task 1
	if (currentTime - previousTimeServoMove > timeIntervalServoMove)
	{
		
		Serial.print("time: ");
		Serial.println(currentTime-previousTimeServoMove);


		previousTimeServoMove = currentTime;

		// Start moving all motors to new position

		step_base = moveServo(&base, new_step_base, step_base);
		step_shoulder = moveServo(&shoulder, new_step_shoulder, step_shoulder);
		step_elbow = moveServo(&elbow, new_step_elbow, step_elbow);
		step_wrist_rot = moveServo(&wrist_rot, new_step_wrist_rot, step_wrist_rot);
		step_wrist_ver = moveServo(&wrist_ver, new_step_wrist_ver, step_wrist_ver);
		step_gripper = moveServo(&gripper, new_step_gripper, step_gripper);
	}
	//It checks if all the servo motors are in the desired position
	if ((new_step_base == step_base) && (new_step_shoulder == step_shoulder) && (new_step_elbow == step_elbow) && (new_step_wrist_rot == step_wrist_rot) && (new_step_wrist_ver == step_wrist_ver) && (new_step_gripper == step_gripper))
	{
		Stop();
	}

}