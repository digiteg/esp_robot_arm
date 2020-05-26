/*
    
    BraccioI2C.cpp - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#include <Arduino.h>
#include <Wire.h>

#include "Braccio.h"

#include "BraccioI2C.h"
#include "RobotArmCmd.h"
#include "CircularBuffer.h"

volatile int callCounter = 0; // holding count of executed I2C calls

_CircularBuffer<robotArmCmd> CircularBuffer; // ??? keep me out

//----------------------------------------

//Initialize Braccio I2C object
_BraccioI2C::_BraccioI2C()
{

    robotStatus.cmd = 0;
    robotStatus.soft_start_level = 0;
    robotStatus.isinit = false;
    robotStatus.isNewCmd = false;

    previousTime = millis(); // init internal Timer
}

// Event handler - read from I2C
void _BraccioI2C::receiveCommand(int count)
{
    byte ReadBuff[10];

    if (count > 0)
    {
        Wire.readBytes(ReadBuff, count < 8 ? count : 8);

        if (ReadBuff[0] == 0x4D || ReadBuff[0] == 0x42)
            CircularBuffer.writeCircBuff(ReadBuff); // Move cmd & Begin cmd
    }

    callCounter++;
}

// Setup of I2C
void _BraccioI2C::setupI2C(int devicenum)
{

    Wire.begin(devicenum);                            // join i2c bus (devicenum ID for master)
    Wire.onReceive((void (*)(int)) & receiveCommand); // Magic added here = register event

    BraccioRobot.setupBraccioRobot();
}

RobotArmStatus _BraccioI2C::getRobotArmStatus()
{
    return robotStatus; // return current robot status
}

void _BraccioI2C::Stop()
{
    isPause = false;
    robotStatus.reset(true);
    robotStatus.isStopped = true;

    BraccioRobot.Stop();

   
}

void _BraccioI2C::powerOff()
{
    Stop();
    BraccioRobot.powerOff();
    robotStatus.isinit =false;
    robotStatus.isPowerOn = false;
}

void _BraccioI2C::powerOn()
{
    Stop();
    BraccioRobot.powerOn();
    robotStatus.reset();
}

void _BraccioI2C::pauseOff()
{
    isPause = false;
    BraccioRobot.pauseOff();
    robotStatus.isPaused = false;
}

void _BraccioI2C::pauseOn()
{
    isPause = true;
    BraccioRobot.pauseOn();
    robotStatus.isPaused = true;
}

bool _BraccioI2C::IsProcessing()
{
    return BraccioRobot.IsProcessing();
}

void _BraccioI2C::loopI2C()
{

    // put your main code here, to run repeatedly:


    BraccioRobot.loopBraccioRobot();

    if (BraccioRobot.IsProcessing() || CircularBuffer.IsEmpty())
        return;

    robotArmCmd *r = CircularBuffer.readCircBuff();

    Serial.print("Robot Status: "); // debug
    Serial.println(r->cmd);

    Serial.println(r->delay);
    Serial.println(r->base);
    Serial.println(r->shoulder);
    Serial.println(r->elbow);
    Serial.println(r->wrist_rot);
    Serial.println(r->wrist_ver);
    Serial.println(r->gripper);

    if (r->cmd == 0x4D) // New cmd.. M - move
    {
        robotStatus = *r;
        BraccioRobot.ServoMovement(robotStatus.delay, robotStatus.base, robotStatus.shoulder, robotStatus.elbow, robotStatus.wrist_rot, robotStatus.wrist_ver, robotStatus.gripper);
        robotStatus.isNewCmd = false;
    }
    else if (r->cmd == 0x42 && !isArmInit) // New cmd.. B - begin
    {
        robotStatus = *r;
        robotStatus.soft_start_level = r->getSoft_Start_Level();

        BraccioRobot.begin(robotStatus.soft_start_level);
        Serial.println(robotStatus.soft_start_level); // debug

        robotStatus.isinit = true;
        robotStatus.isNewCmd = false;
        isArmInit = true;
    }
    else if (r->cmd == 0x50) // New cmd P - pause  on/off
    {
        Serial.println("Pause on/off: "); // debug

        if (r->delay > 0) // P: True - pause On command
        {
            pauseOn();
        }
        else // P: False - pause Off command
        {
            pauseOff();
        }
        robotStatus.isNewCmd = false;
    }
    else if (r->cmd == 0x53) // S: - stop command
    { 
        Serial.println("Stop: "); // debug
        Stop();
        robotStatus.isNewCmd = false;
    }
    else if (r->cmd == 0x4F) // New cmd O - turn on/off command
    {
        Serial.println("Turn on/off: ");
        if (r->delay > 0) // O: True - turn On command
        {
            powerOn();
        }
        else // O: False - turn Off command
        {
            powerOff();
        }
        robotStatus.isNewCmd = false;
    }
}
