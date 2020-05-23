
/*
    
    RobotArmCmd.h - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#ifndef ROBOTARMCMD_I2C_H_
#define ROBOTARMCMD_I2C_H_

/*
    I2C 8 bytes command structure
    @cmd - command type M - move, B - beggin
    @delay - pause for millisconds
    @base, shoulder, elbow, wrist_rot,wrist_ver, gripper - setting servo motors angle values  

*/
typedef struct robotArmCmd
{

    byte cmd;
    byte delay;
    byte base;
    byte shoulder;
    byte elbow;
    byte wrist_rot;
    byte wrist_ver;
    byte gripper;

    void readbuff(byte *item) // read robotArmCmd from byte stream
    {
        cmd = item[0];
        delay = item[1];
        base = item[2];
        shoulder = item[3];
        elbow = item[4];
        wrist_rot = item[5];
        wrist_ver = item[6];
        gripper = item[7];
    }

    robotArmCmd &operator=(const robotArmCmd &item) // operator (= robotArmCmd) overloading
    {

        cmd = item.cmd;
        delay = item.delay;
        base = item.base;
        shoulder = item.shoulder;
        elbow = item.elbow;
        wrist_rot = item.wrist_rot;
        wrist_ver = item.wrist_ver;
        gripper = item.gripper;

        return *this;
    }

    robotArmCmd &operator=(const byte *item) // operator (= byte array) overloading
    {
        readbuff(item);
        return *this;
    }

    short getSoft_Start_Level() // extract soft start level
    {
        short s = (short)(base << 8 | delay & 0xFF); // short(2bytes) = high + low byte
        return s;
    }

} robotArmCmd;

#endif // ROBOTARMCMD_I2C_H_