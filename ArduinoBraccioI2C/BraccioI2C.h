/*
    
    BraccioI2C.h - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#ifndef BRACCIO_I2C_H_
#define BRACCIO_I2C_H_

#include "RobotArmCmd.h"

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

} RobotArmStatus;

/*
    _BraccioI2C class provides interface for communication over I2C with Braccio robot arm 

*/
class _BraccioI2C
{
private:
    unsigned long previousTime;
    long timeInterval = 1000;

    RobotArmStatus robotStatus; // to keep actual robot status
    void receiveCommand(int count);

protected:
    bool isArmInit = false; // internal - true after intition

public:
    _BraccioI2C();                      // constructor
    void setupI2C(int devicenum);       // inition of I2C communication
    void loopI2C();                     // update status
    RobotArmStatus getRobotArmStatus(); // return actual arm status
};

#endif // BRACCIO_I2C_H_