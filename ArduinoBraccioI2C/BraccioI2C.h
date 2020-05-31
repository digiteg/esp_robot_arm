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
    @isinit - true after initiation
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

    /*
     inition of I2C communication
      @param devicenum: register with ID on I2C
*/

    void setup(int devicenum);

    // update status
    void loop();

    // return actual arm status
    RobotArmStatus getRobotArmStatus();

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

    /*
     Braccio initializations and set inital position
     Modifing this function you can set up the initial position of all the
     servo motors of Braccio 
     @param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
     You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
 */
    void begin(int soft_start_level = SOFT_START_DEFAULT);

    /*
     This function allow the user to control all the servo motors in the Braccio
 */
    void servoMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_rot, int vWrist_ver, int vgripper);

protected:
    void begin(robotArmCmd *r);
    void servoMovement(robotArmCmd *r);

    bool isArmInit = false; // internal - true after intition

private:
    // multithread ...
    bool isPause = false;

    RobotArmStatus robotStatus; // to keep actual robot status

    void receiveCommand(int count);
    void requestCommantStatus();
};

#endif // BRACCIO_I2C_H_