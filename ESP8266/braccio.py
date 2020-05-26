# -------------------------------------------------------------------------------
# Name:        BraccioRobotArm
# Purpose:     Manipulate with Braccio robot arm
#
# Author:      Milan Varga
#
# Created:     17.05.2020
# Copyright:   (c) Milan Varga 2020
# Licence:     <your licence>
# -------------------------------------------------------------------------------
import ujson
import ubinascii
from micropython import const


class BraccioArm:

    # You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
    SOFT_START_DISABLED = const(-999)

    # The default value for the soft start
    SOFT_START_DEFAULT = const(0)
    robotSoftStartLevel = int(SOFT_START_DEFAULT)

    robotArmInit = False
    isTurnedOn = False
    isPaused = False
    isStop = False

    # robot arm starting position
    robotArmPosition = {
        "delay": 20,
        "base": 90,
        "shoulder": 45,
        "elbow": 180,
        "wrist_ver": 180,
        "wrist_rot": 90,
        "gripper": 10
    }
    # --------- Delay Property

    @property
    def delay(self):
        return self.robotArmPosition["delay"]

    @delay.setter
    def delay(self, value):
        self.robotArmPosition["delay"] = self._rangeMinMax(value, 10, 30)

    # --------- Base Property
    @property
    def base(self):
        return self.robotArmPosition["base"]

    @base.setter
    def base(self, value):
        self.robotArmPosition["base"] = self._rangeMinMax(value, 0, 180)

    # --------- Shoulder Property
    @property
    def shoulder(self):
        return self.robotArmPosition["shoulder"]

    @shoulder.setter
    def shoulder(self, value):
        self.robotArmPosition["shoulder"] = self._rangeMinMax(value, 15, 165)

# --------- Elbow Property
    @property
    def elbow(self):
        return self.robotArmPosition["elbow"]

    @elbow.setter
    def elbow(self, value):
        self.robotArmPosition["elbow"] = self._rangeMinMax(value, 0, 180)

# --------- Wrist_ver Property
    @property
    def wrist_ver(self):
        return self.robotArmPosition["wrist_ver"]

    @wrist_ver.setter
    def wrist_ver(self, value):
        self.robotArmPosition["wrist_ver"] = self._rangeMinMax(value, 0, 180)

# --------- Wrist_rot Property
    @property
    def wrist_rot(self):
        return self.robotArmPosition["wrist_rot"]

    @wrist_rot.setter
    def wrist_rot(self, value):
        self.robotArmPosition["wrist_rot"] = self._rangeMinMax(value, 0, 180)

# --------- Gripper Property
    @property
    def gripper(self):
        return self.robotArmPosition["gripper"]

    @gripper.setter
    def gripper(self, value):
        self.robotArmPosition["gripper"] = self._rangeMinMax(value, 10, 73)

    # --------- Helpers

    def _rangeMinMax(self, num, minn, maxn):
        if num == None:
            return minn
        return minn if num < minn else maxn if num > maxn else num

    def toJSON(self):
        return ujson.dumps(self.robotArmPosition)

    def toCVS(self):
        txt = str(self.delay) + ',' + str(self.base) + ',' + \
            str(self.shoulder) + ',' + str(self.elbow) + ','
        txt += str(self.wrist_rot) + ',' + \
            str(self.wrist_ver) + ',' + str(self.gripper)
        return txt

    def toBytes(self, cmd_prefix=0x00):

        cmd = [cmd_prefix & 0xFF, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00]  # 8 bytes
        cmd[1] = self.delay & 0xFF
        cmd[2] = self.base & 0xFF
        cmd[3] = self.shoulder & 0xFF
        cmd[4] = self.elbow & 0xFF
        cmd[5] = self.wrist_rot & 0xFF
        cmd[6] = self.wrist_ver & 0xFF
        cmd[7] = self.gripper & 0xFF

        if (cmd_prefix == 0x00):
            cmd.pop(0)

        return cmd

# --------- Conrols
    """
     Braccio initialization and set intial position
     Modifing this function you can set up the initial position of all the
     servo motors of Braccio
     @param soft_start_level: the minimum value is -70, default value is 0 (SOFT_START_DEFAULT)
     You should set begin(SOFT_START_DISABLED) if you are using the Arm Robot shield V1.6
     SOFT_START_DISABLED disable the Braccio movements
 """

    def begin(self, soft_start_level=0):
        #  Calling Braccio.begin(SOFT_START_DISABLED) the Softstart is disabled and you can use the pin 12
        self.robotSoftStartLevel = soft_start_level
        self.robotArmInit = True
        self.isStop = False

    """
     Braccio turns bag to start position
     All the servo motors will be positioned in the "safety" position:
     Base (M1):90 degrees
     Shoulder (M2): 45 degrees
     Elbow (M3): 180 degrees
     Wrist vertical (M4): 180 degrees
     Wrist rotation (M5): 90 degrees
     gripper (M6): 10 degrees
 """

    def defaultpos(self):
        self.movement(20, 90, 45, 180, 180, 90, 10)

    """
     Braccio turns back to default position
     ready to turn off
 """

    def turnoff(self):
        #  Calling Braccio.begin(SOFT_START_DISABLED) the Softstart is disabled and you can use the pin 12
        self.defaultpos()
        self.robotArmInit = False
        self.isTurnedOn = False

    """
     Braccio turns on and reset to default position
     ready to begin
 """

    def turnon(self):
        self.defaultpos()
        self.robotArmInit = False
        self.isTurnedOn = True

    def stop(self):
        self.isPaused = False
        self.isStop = True

    def pauseoff(self):
        isPaused = False

    def pauseon(self):
        isPaused = False

    """
     This functions allow you to control all the servo motors
     
     @param stepDelay The delay between each servo movement
     @param vBase next base servo motor degree
     @param vShoulder next shoulder servo motor degree
     @param vElbow next elbow servo motor degree
     @param vWrist_ver next wrist rotation servo motor degree
     @param vWrist_rot next wrist vertical servo motor degree
     @param vgripper next gripper servo motor degree
 """

    def movement(self, stepDelay=10, vBase=0, vShoulder=15, vElbow=0,  vWrist_rot=0, vWrist_ver=0, vgripper=10):

        # Check values, to avoid dangerous positions for the Braccio
        # ServoMovement(step delay, M1, M2, M3, M4, M5, M6);

        self.delay = stepDelay
        self.base = vBase
        self.shoulder = vShoulder
        self.elbow = vElbow
        self.wrist_rot = vWrist_rot
        self.wrist_ver = vWrist_ver
        self.gripper = vgripper
        self.isStop = False

    # Initialize Test :


def main():
    arm = BraccioArm()
    arm.begin()
    print(arm.delay)
    print(arm.base)
    print(arm.shoulder)
    print(arm.elbow)
    print(arm.wrist_ver)
    print(arm.wrist_rot)
    print(arm.gripper)
    arm.movement(180, 180, 180, 180, 180, 180, 180)

    print("CVS: ", arm.toCVS())
    print("JSON: ", arm.toJSON())
    print("bytes: ", arm.toBytes(0x4D))

    arm.pauseon()
    arm.pauseoff()
    arm.stop()
    arm.defaultpos()
    arm.turnoff()
    arm.turnon()


if __name__ == '__main__':
    main()
