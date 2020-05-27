/*
    
    BraccioI2C.h - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#ifndef BRACCIO_I2C_H_
#define BRACCIO_I2C_H_

#include "RobotArmCmd.h"
#include "Braccio.h"


/*
    Structure to keep robot arm status 
    @soft_start_level - used for inition, 0 value represents disabled arem soft start
    @isinit - true after intition
    @isNewCmd - if values changed
*/

typedef struct RobotArmStatus : robotArmCmd
{
    short soft_start_level;
    bool isinit;
    bool isNewCmd;
    bool isPaused;
    bool isPowerOn;
    bool isStopped;

    RobotArmStatus &operator=(const robotArmCmd &item)
    {
        cmd = item.cmd;
        delay = item.delay;
        base = item.base;
        shoulder = item.shoulder;
        elbow = item.elbow;
        wrist_ver = item.wrist_ver;
        wrist_rot = item.wrist_rot;
        gripper = item.gripper;

        isNewCmd = true;
        return *this;
    }

    void reset(bool init = false, bool newcmd = false)
    {
        isinit = init;
        isNewCmd = newcmd;

        isPaused = false;
        isPowerOn = true;
        isStopped = false;
    }

} RobotArmStatus;

/*
    _BraccioI2C class provides interface for communication over I2C with Braccio robot arm 

*/
class _BraccioI2C : private _BraccioRobot
{

public:
    _BraccioI2C(); // constructor

    void setup(int devicenum);          // inition of I2C communication
    void loop();                        // update status
    RobotArmStatus getRobotArmStatus(); // return actual arm status

    /* 
    Turns off / On power to the servo motors. This only work if you are using a robot shield later than V1.6.
    Note that after a call to init() the power is on 
*/
    void powerOff();
    void powerOn();

    /* 
    Pause exec of command and move of servo motors. 
*/
    void pauseOff();
    void pauseOn();

    /* 
    Stop exec of command and all servo motors. 
*/
    void stop();

    /*
    Return ifo if command is still in process
*/
    bool isProcessing();

protected:
    bool isArmInit = false; // internal - true after intition

private:
    // multithread ...
    bool isPause = false;
    unsigned long previousTime;
    long timeInterval = 1000;

    RobotArmStatus robotStatus; // to keep actual robot status
    void receiveCommand(int count);
};

#endif // BRACCIO_I2C_H_