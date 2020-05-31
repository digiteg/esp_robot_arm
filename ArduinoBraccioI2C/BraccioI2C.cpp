/*
    
    BraccioI2C.cpp - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#include <Arduino.h>
#include <Wire.h>

#include "CircularBuffer.h"
#include "BraccioI2C.h"

//---------------------------------------- // ??? keep me out

_CircularBuffer<robotArmCmd> CircularBuffer;

enum RobotMsg
{
    MPowerOff = 0,
    MPowerOn = 1,
    MStop = 2,
    MPauseOn = 3,
    MPauseOff = 4,
    MInProgress = 5,
    MBegin = 6,
    MMove = 7,
    MWait = 8
};

const char robotmsgtext[9][7] = {"armoff", "armonn", "stoped", "paused", "npause", "inproc", "abegin", "moving", "arwait"};
int robotmsgindex = 0;

//----------------------------------------

#define CMD_POWER 0x4F // cmd 'O' - turn on/off
#define CMD_STOP 0x53  // cmd 'S' - stop
#define CMD_PAUSE 0x50 // cmd 'P' - pause  on/off
#define CMD_BEGIN 0x42 // cmd 'B' - begin
#define CMD_MOVE 0x4D  // cme 'M' - move

//	Braccio I2C object initialization
_BraccioI2C::_BraccioI2C()
{

    robotStatus.cmd = 0;
    robotStatus.soft_start_level = 0;
    robotStatus.isinit = false;
    robotStatus.isNewCmd = false;
}

// Event handler - receive from I2C master
void _BraccioI2C::receiveCommand(int count)
{
    if (count <= 0)
        return;

    byte ReadBuff[10];
    Wire.readBytes(ReadBuff, count < 8 ? count : 8); // read 8 byte command sent by master from I2C

    switch (ReadBuff[0])
    {
    case CMD_POWER:
    case CMD_STOP:
    case CMD_PAUSE:
    case CMD_BEGIN:
    case CMD_MOVE:
        CircularBuffer.push(ReadBuff); // Move cmd & Begin cmd
    default:
        return;
    }
}

// Event handler - request from I2C master

void _BraccioI2C::requestCommantStatus()
{
    Wire.write(robotmsgtext[robotmsgindex], 6); // send msg to Master over I2C
}

// Setup of I2C
void _BraccioI2C::setup(int devicenum)
{

    Wire.begin(devicenum);                            // join i2c bus (devicenum ID for master)
    Wire.onReceive((void (*)(int)) & receiveCommand); // Magic added here = register event

    Wire.onRequest((void (*)()) & requestCommantStatus); // Magic added here = register event
    _BraccioRobot::setup();
}

// Returns robot arm status in structure format
RobotArmStatus _BraccioI2C::getRobotArmStatus()
{
    return robotStatus; // return current robot status
}

// Stops robot arm comman
void _BraccioI2C::stop()
{
    isPause = false;
    robotStatus.reset(true);
    robotStatus.isStopped = true;
    _BraccioRobot::stop();

    robotmsgindex = MStop; // stopped
}

// Turn power off if you are using the Arm Robot shield V1.6
void _BraccioI2C::powerOff()
{
    stop();
    _BraccioRobot::powerOff();
    robotStatus.isinit = false;
    robotStatus.isPowerOn = false;

    robotmsgindex = MPowerOff; // armoff
}

// Turn power on if you are using the Arm Robot shield V1.6
void _BraccioI2C::powerOn()
{
    stop();
    _BraccioRobot::powerOn();
    robotStatus.reset();

    robotmsgindex = MPauseOn; // armonn
}

//  Release pause and continue robot arm movement shield V1.6
void _BraccioI2C::pauseOff()
{
    isPause = false;
    _BraccioRobot::pauseOff();
    robotStatus.isPaused = false;

    robotmsgindex = MPowerOff; // npause
}

// Pause robot arm movement
void _BraccioI2C::pauseOn()
{
    isPause = true;
    _BraccioRobot::pauseOn();
    robotStatus.isPaused = true;

    robotmsgindex = MPauseOn; // paused
}

// Retrn info if there is movement of robot arm in progress
bool _BraccioI2C::isProcessing()
{
    return _BraccioRobot::isProcessing();
}

// Braccio initializations and set inital position
void _BraccioI2C::begin(robotArmCmd *r)
{
    robotStatus = *r;
    robotStatus.soft_start_level = r->getSoft_Start_Level();

    _BraccioRobot::begin(robotStatus.soft_start_level);

    robotStatus.isinit = true;
    isArmInit = true;
    robotmsgindex = MBegin; // abegin
}

/*
     Braccio initializations and set inital position via creating Begin command
  
     Modifing this function you can set up the initial position of all the
     servo motors of Braccio 
     @param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
     You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
 */
void _BraccioI2C::begin(int soft_start_level)
{
    robotArmCmd tmp;
    tmp.cmd = CMD_BEGIN;
    tmp.setSoft_Start_Level(soft_start_level);
    CircularBuffer.push(tmp);
}

// 	This functions allows you to control all the servo motors

void _BraccioI2C::servoMovement(robotArmCmd *r)
{
    robotStatus = *r;
    _BraccioRobot::servoMovement(robotStatus.delay, robotStatus.base, robotStatus.shoulder, robotStatus.elbow, robotStatus.wrist_rot, robotStatus.wrist_ver, robotStatus.gripper);
    robotmsgindex = MMove; // moving";
}

/*
 	This functions allow you to control all the servo motors via creating Move command
  
 	@param stepDelay The delay between each servo movement
 	@param vBase next base servo motor degree
 	@param vShoulder next shoulder servo motor degree
 	@param vElbow next elbow servo motor degree
 	@param vWrist_ver next wrist rotation servo motor degree
 	@param vWrist_rot next wrist vertical servo motor degree
 	@param vgripper next gripper servo motor degree
 */

void _BraccioI2C::servoMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_rot, int vWrist_ver, int vgripper)
{
    robotArmCmd tmp;
    tmp.load(CMD_MOVE, stepDelay, vBase, vShoulder, vElbow, vWrist_rot, vWrist_ver, vgripper);
    CircularBuffer.push(tmp);
}

// running Braccio arm commands in time
void _BraccioI2C::loop()
{

    // put your main code here, to run repeatedly:

    _BraccioRobot::loop();

    bool inproc = isProcessing();

    if (inproc || CircularBuffer.isEmpty())
    {
        if (inproc)
            robotmsgindex = MInProgress; // inproc
        else
            robotmsgindex = MWait; // inproc
        return;
    }

    robotArmCmd *r = CircularBuffer.pop();

    Serial.print("Robot Status: "); // debug
    Serial.println(r->cmd);

    Serial.println(r->delay);
    Serial.println(r->base);
    Serial.println(r->shoulder);
    Serial.println(r->elbow);
    Serial.println(r->wrist_rot);
    Serial.println(r->wrist_ver);
    Serial.println(r->gripper);

    if (r->cmd == CMD_MOVE) // New cmd.. M - move
    {
        servoMovement(r);
    }
    else if (r->cmd == CMD_BEGIN && !isArmInit) // New cmd.. B - begin
    {
        begin(r);
        Serial.println(robotStatus.soft_start_level); // debug
    }
    else if (r->cmd == CMD_PAUSE) // New cmd P - pause  on/off
    {
        Serial.println("Pause on/off: "); // debug

        if (r->delay > 0) // Bool parameter to keep pause On/Off
            pauseOn();    // P: True - pause On command
        else
            pauseOff(); // P: False - pause Off command
    }
    else if (r->cmd == CMD_STOP) // S: - stop command
    {
        Serial.println("Stop: "); // debug
        stop();
    }
    else if (r->cmd == CMD_POWER) // New cmd O - turn on/off command
    {
        Serial.println("Turn on/off: "); // debug

        if (r->delay > 0) // Bool parameter o keep pause On/Off
            powerOn();    // O: True - turn On command
        else
            powerOff(); // O: False - turn Off command
    }
    robotStatus.isNewCmd = false;
}
