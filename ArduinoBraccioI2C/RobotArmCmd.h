
/*
    
    RobotArmCmd.h - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#ifndef ROBOTARMCMD_I2C_H_
#define ROBOTARMCMD_I2C_H_

/*
    I2C 8 bytes command structure
    @cmd - command type M - move, B - beggin
    @delay - pause for milliseconds
    @base, shoulder, elbow, wrist_rot,wrist_ver, gripper - setting servo motors angle values  

*/
typedef struct robotArmCmd
{

    byte cmd;
    byte delay;
    byte base;
    byte shoulder;
    byte elbow;
    byte wrist_ver;
    byte wrist_rot;
    byte gripper;

    void load(byte vcmd, byte vdelay, byte vbase, byte vshoulder, byte velbow, byte vwrist_ver, byte vwrist_rot, byte vgripper)
    {
        cmd = vcmd;
        delay = vdelay;
        base = vbase;
        shoulder = vshoulder;
        elbow = velbow;
        wrist_ver = vwrist_ver;
        wrist_rot = vwrist_rot;
        gripper = vgripper;
    }

    void readbuff(const byte *item) // read robotArmCmd from byte stream
    {

        load(item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[7]);
    }

    robotArmCmd &operator=(const robotArmCmd &item) // operator (= robotArmCmd) overloading
    {

        load(item.cmd, item.delay, item.base, item.shoulder, item.elbow, item.wrist_ver, item.wrist_rot, item.gripper);
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

    void setSoft_Start_Level(int soft_start_level) // extract soft start level
    {
        base = (soft_start_level >> 8) & 0xFF;
        delay = soft_start_level & 0xFF;
    }

} robotArmCmd;

#endif // ROBOTARMCMD_I2C_H_