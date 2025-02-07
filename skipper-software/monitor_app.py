from tkinter import Tk
from dataclasses import dataclass
import numpy as np
import threading

@dataclass
class IMUData:
    acc: np.ndarray = np.zeros(3)  # in g's
    gyr: np.ndarray = np.zeros(3)  # in deg/sec

@dataclass
class SkipperState:
    pos: np.ndarray = np.zeros(3)  # displacement (from start position) in m
    vel: np.ndarray = np.zeros(3)  # velocity in m/s
    acc: np.ndarray = np.zeros(3)  # acceleration in m/s^2

    rot: np.ndarray = np.zeros(3)      # rotation as 321 Tait-Bryan angles; stored as rotation around x-axis, y-axis, and z-axis in radians
    ang_vel: np.ndarray = np.zeros(3)  # angular velocity in radians/s
    ang_acc: np.ndarray = np.zeros(3)  # angular acceleration in radians/s^2

@dataclass
class ControlSignal:
    gimbal_a:   float = 0.0
    gimbal_b:   float = 0.0  # these definitions are a bit long;
    thrust_cw:  float = 0.0  # check "teensy.hpp" for definitions.
    thrust_ccw: float = 0.0

class Monitor(Tk):

    imu_data: IMUData = IMUData()
    skipper_state: SkipperState = SkipperState()
    control_signal: ControlSignal = ControlSignal()

    def __init__(self):
        # we gotta make a simple tkinter app to view and save the data
        # somewhere in __init__ we need to establish connection with Serial ("Ground" Arduino USB connection)
        pass

    def communicate_with_serial(self):
        while True:
            # update imu_data, skipper_state, and control_signal from Serial **using a double buffer**
            # so that the data isn't updated mid-read in run.
            pass

    def run(self):
        while True:
            pass

if __name__ == '__main__':
    monitor = Monitor()
    monitor_thread = threading.Thread(target=monitor.run)
    communicate_thread = threading.Thread(target=monitor.communicate_with_serial)

    monitor_thread.start()
    communicate_thread.start()

    communicate_thread.join()
    monitor_thread.join()
