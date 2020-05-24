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

/**
 * Braccio initialization and set intial position
 * Modifing this function you can set up the initial position of all the
 * servo motors of Braccio
 * @param soft_start_level: default value is 0 (SOFT_START_DEFAULT)
 * You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
 * SOFT_START_DISABLED disable the Braccio movements
 */

void _BraccioRobot::begin(int soft_start_level)
{
	//Calling Braccio.begin(SOFT_START_DISABLED) the Softstart is disabled and you can use the pin 12
	if (soft_start_level != SOFT_START_DISABLED)
	{
		pinMode(SOFT_START_CONTROL_PIN, OUTPUT);
		digitalWrite(SOFT_START_CONTROL_PIN, LOW);
	}

	//For each step motor this set up the initial degree
	base.write(0);
	shoulder.write(40);
	elbow.write(180);
	wrist_ver.write(170);
	wrist_rot.write(0);
	gripper.write(73);
	//Previous step motor position
	step_base = 0;
	step_shoulder = 40;
	step_elbow = 180;
	step_wrist_ver = 170;
	step_wrist_rot = 0;
	step_gripper = 73;

	if (soft_start_level != SOFT_START_DISABLED)
		softStart(soft_start_level);
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
* This function, used only with the Braccio Shield V4 and greater,
* turn ON the Braccio softly and save it from brokes.
* The SOFT_START_CONTROL_PIN is used as a software PWM
* @param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
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
	{
		return minv;
	}
	else if (value > maxv)
	{
		return maxv;
	}
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

/**
 * This functions allow you to control all the servo motors
 * 
 * @param stepDelay The delay between each servo movement
 * @param vBase next base servo motor degree
 * @param vShoulder next shoulder servo motor degree
 * @param vElbow next elbow servo motor degree
 * @param vWrist_ver next wrist rotation servo motor degree
 * @param vWrist_rot next wrist vertical servo motor degree
 * @param vgripper next gripper servo motor degree
 */
int _BraccioRobot::ServoMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_ver, int vWrist_rot, int vgripper)
{

	// Check values, to avoid dangerous positions for the Braccio
	stepDelay = getlimit(stepDelay, 10, 30);
	vBase = getlimit(vBase, 0, 180);
	vShoulder = getlimit(vShoulder, 15, 165);
	vElbow = getlimit(vElbow, 0, 180);
	vWrist_ver = getlimit(vWrist_ver, 0, 180);
	vWrist_rot = getlimit(vWrist_rot, 0, 180);
	vgripper = getlimit(vgripper, 10, 73);

	int exit = 1;

	//Until the all motors are in the desired position
	while (exit)
	{

		step_base = moveServo(&base, vBase, step_base);
		step_shoulder = moveServo(&shoulder, vShoulder, step_shoulder);
		step_elbow = moveServo(&elbow, vElbow, step_elbow);
		step_wrist_rot = moveServo(&wrist_rot, vWrist_ver, step_wrist_rot);
		step_wrist_ver = moveServo(&wrist_ver, vWrist_rot, step_wrist_ver);
		step_gripper = moveServo(&gripper, vgripper, step_gripper);

		//delay between each movement
		delay(stepDelay);

		//It checks if all the servo motors are in the desired position
		if ((vBase == step_base) && (vShoulder == step_shoulder) && (vElbow == step_elbow) && (vWrist_ver == step_wrist_rot) && (vWrist_rot == step_wrist_ver) && (vgripper == step_gripper))
		{
			step_base = vBase;
			step_shoulder = vShoulder;
			step_elbow = vElbow;
			step_wrist_rot = vWrist_ver;
			step_wrist_ver = vWrist_rot;
			step_gripper = vgripper;
			exit = 0;
		}
		else
		{
			exit = 1;
		}
	}
}

void _BraccioRobot::loopBraccioRobot()
{
	unsigned long currentTime = millis();

	// task 1
	if (currentTime - previousTimeServoMove > timeIntervalServoMove)
	{
		previousTimeServoMove = currentTime;
	}
}