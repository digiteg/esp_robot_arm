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

from time import sleep_ms, ticks_ms
from machine import I2C, Pin
from braccio import BraccioArm


class BraccioArmI2C(BraccioArm):

    sclPin = 5
    sdaPin = 4
    robot_address = 100
    devicefound = False

    # [0x4D,0x3A])) -> M: - movement command
    # [0x42,0x3A])) -> B: - begin command

    def __init__(self, sclpin=5, sdapin=4, comfreq=100000, deviceaddr=100):
        super().__init__()

        self.i2c = I2C(scl=Pin(sclpin, Pin.OUT), sda=Pin(sdapin), freq=comfreq)

        self.sclPin = sclpin
        self.sdaPin = sdapin
        self.robot_address = deviceaddr
        self.devicefound = False

        devices = self.i2c.scan()
        for dev in devices:
            if (dev == deviceaddr):
                self.devicefound = True
                print("Device found")
                break
            else:
                print("Device not found")

    def getdevices(self):
        if (i2c is not None):
            devices = i2c.scan()
            return devices
        return None

    # ------------- I2C Command Helpers:

    def _sendI2Ccommand(self, buff):
        if(self.devicefound):
            self.i2c.writeto(self.robot_address, bytearray(buff), False)
        else:
            print("Device not found")

    def _readI2C(self):
        data=None
        if(self.devicefound):
            data= self.i2c.readfrom(self.robot_address,6)
        else:
            print("Device not found")
        return data


    def _getSimpleCmd(self, cmdid=0x00, param1=0x00, param2=0x00):
        cmd = [cmdid & 0xFF, param1 & 0xFF, param2 &
               0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]
        return cmd

    def _sendSimleCmd(self, cmdid=0x00, param1=0x00, param2=0x00):
        cmd = self._getSimpleCmd(0x42, self.robotSoftStartLevel,
                            self.robotSoftStartLevel >> 8)
        print(cmd) # debug
        self._sendI2Ccommand(cmd)


    def _sendBegin(self):
        self._sendSimleCmd ( 0x42, self.robotSoftStartLevel,
                            self.robotSoftStartLevel >> 8)

    def _sendMove(self):
        self._sendI2Ccommand(self.toBytes(0x4D))  # M: - movement command

    # ------------- Methods:

    def begin(self, soft_start_level=0):
        if (self.robotArmInit):
            return
        # Calling Braccio begin with Softstart level => 0 is disabled
        super().begin(soft_start_level)
        self._sendBegin()

    def movement(self, stepDelay=10, vBase=0, vShoulder=15, vElbow=0, vWrist_rot=0, vWrist_ver=0,  vgripper=10):
        super().movement(stepDelay, vBase, vShoulder,
                         vElbow, vWrist_rot, vWrist_ver, vgripper)
        self._sendMove()  # M: - movement command

    def readACK(self):
        buff= self._readI2C()
        print(buff)
    
    def defaultpos(self):
        super().defaultpos()  # go to default position
        self._sendMove()  # M: - movement command

    def turnoff(self):
        super().turnoff()  # ready to turn off
        self._sendSimleCmd ( 0x4F, 0x00) #  O: False - turn Off command


    def turnon(self):
        super().turnoff()  # ready to turn off
        self._sendSimleCmd ( 0x4F, 0x01) # O: True - turn On command

    def stop(self):
        super.stop()
        self._sendSimleCmd ( 0x53) # S: - stop command 

    def pauseoff(self):
        self._sendSimleCmd ( 0x50, 0x00) # P: False - pause Off command 

    def pauseon(self):
        self._sendSimleCmd ( 0x50, 0x01) # P: True - pause On command 

   # ------------- Destructors:

    def __enter__(self):
        return self

    def __exit__(self, exception_type, exception_value, traceback):
        if (self.i2c is not None):
            self.i2c.deinit()
        self.devicefound = False


arm = BraccioArmI2C()


""""
arm.movement()

arm.movement(20,0,15,180,170,0,73)
print("move1:",arm.toJSON())

arm.movement(20,180,165,0,0,180,10)  
print("move2:",arm.toJSON())

print(arm.delay)
print(arm.base)
print(arm.shoulder)
print(arm.elbow)
print(arm.wrist_ver)
print(arm.wrist_rot)
print(arm.gripper)
"""

# i2c.writeto(robot_arm_address,arm.toJSON(),True)
#arr = bytes(arm.toJSON(),"utf8")
# print(arr)

arm.begin()

arm.readACK()

#arm.movement(180, 180, 180, 180, 180, 180, 180)

for i in range(5):
    arm.movement(20, 0, 15, 175, 0, 175, 73)
    sleep_ms(10)
    arm.movement(20, 175, 165, 0, 175, 0, 10)
    sleep_ms(10)



arm.movement(20, 90,  0, 175,   0, 160,  15)
arm.readACK()
arm.defaultpos()
arm.readACK()
# i2c.start()
# i2c.writeto(robot_arm_address,arm.toBytes())
# i2c.stop()
# sleep_ms(100)
# i2c.writeto(robot_arm_address,buf)

print(arm.toCVS())
print(arm.toJSON())
print("bytes: ", arm.toBytes(0x4D))

# arm.defaultpos()
# arm.turnoff()
# print("move1:",arm.toJSON())
# i2c.writeto(robot_arm_address,arm.toJSON())
# i2c.writeto(robot_arm_address,arr)
