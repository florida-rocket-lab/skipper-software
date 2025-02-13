import tkinter as tk
from dataclasses import dataclass
import numpy as np
import threading
import serial

BAUD_RATE: int = 115200
APPLICATION_NAME: str = "Skipper Information Application"
END_OF_TRANSMISSION: str = "----------------------\n"
TELEMETRY_HEADERS = {
    "acc": "IMU Acc: ",
    "gyr": "IMU Gyr: ",
    "pos": "Position: ",
    "vel": "Velocity: "
}
APPLICATION_FPS: int = 60
SAVE_LOCATIONS = {
    "telemetry-data": "telemetry_data.npy",
    "skipper-state": "skipper_state.npy",
    "control-signal": "control_signal.npy"
}
DATA_ORDER = [
    'pos',
    'vel',
    'acc',
    'gyr'
]

@dataclass
class TelemetryData:
    pos: np.ndarray = np.zeros(3)  # in meters?
    vel: np.ndarray = np.zeros(3)  # in meters/second?
    acc: np.ndarray = np.zeros(3)  # in g's?
    gyr: np.ndarray = np.zeros(3)  # in deg/sec?

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
    thrust_cw:  float = 0.0  # check "teensy.h" for definitions.
    thrust_ccw: float = 0.0

class Monitor(tk.Tk):

    telemetry_data: TelemetryData = TelemetryData()
    skipper_state: SkipperState = SkipperState()
    control_signal: ControlSignal = ControlSignal()

    _output_buffer: str = ""
    _tkinter_objects: dict[str, tk.BaseWidget] = {}

    def __init__(self):
        # we gotta make a simple tkinter app to view and save the data
        # somewhere in __init__ we need to establish connection with Serial ("Ground" Arduino USB connection)

        super(Monitor, self).__init__(screenName=APPLICATION_NAME, baseName=APPLICATION_NAME, className=APPLICATION_NAME)
        self._serial = serial.Serial("COM3", BAUD_RATE, timeout=10.0)
        self._init_tkinter_objects()

    def _init_tkinter_objects(self):
        _text_box_data: dict[str, str] = self._generate_box_text()
        for _obj in DATA_ORDER:
            self._create_readonly_text_obj(_text_box_data[_obj], _obj)

    def _create_readonly_text_obj(self, text: str, object_id: str) -> None:
        _text_box: tk.Text = tk.Text(self, wrap=tk.NONE)
        _text_box.pack(padx=10, pady=5)
        _text_box.insert(tk.END, text)
        _text_box.config(state=tk.DISABLED)
        self._tkinter_objects[object_id] = _text_box

    def _update_readonly_text_obj(self, text: str, object_id: str) -> None:
        self._tkinter_objects[object_id].config(text=text)

    def _generate_box_text(self) -> dict[str, str]:
        _output: dict[str, str] = {
            'acc': "Acceleration:\n\t{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.acc),
            'vel': "Velocity:\n\t{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.vel),
            'pos': "Position:\n\t{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.pos),
            'gyr': "Gyroscopic Data:\n\t{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.gyr)
        }
        return _output

    @staticmethod
    def _snag_property(property_label: str, incoming_data: str) -> str:
        _property_location: int = incoming_data.find(property_label)
        assert _property_location != -1, f"Could not find \"{property_label}\" in parameter \"incoming_data\" in class method Monitor._snag_property(...).\nReceived: \n{incoming_data}"
        incoming_data = incoming_data[_property_location+len(property_label):]
        incoming_data = incoming_data[:incoming_data.find('\n')]
        assert incoming_data, f"Found \"{property_label}\" in parameter \"incoming_data\" in class method Monitor._snag_property(...), but no data was found.\nReceived: \n{incoming_data}"
        return incoming_data

    @staticmethod
    def _read_tuple(incoming_data: str) -> np.ndarray:
        return eval('np.array((' + incoming_data + ',))')  # a little dangerous, but all of this is internal code, so there is no shenanigans.

    @staticmethod
    def _read_telemetry(incoming_telemetry: str) -> TelemetryData:
        telemetry_data = TelemetryData()
        telemetry_data.pos = Monitor._read_tuple(
            Monitor._snag_property(TELEMETRY_HEADERS['pos'], incoming_telemetry)
        )
        telemetry_data.vel = Monitor._read_tuple(
            Monitor._snag_property(TELEMETRY_HEADERS['vel'], incoming_telemetry)
        )
        telemetry_data.acc = Monitor._read_tuple(
            Monitor._snag_property(TELEMETRY_HEADERS['acc'], incoming_telemetry)
        )
        telemetry_data.gyr = Monitor._read_tuple(
            Monitor._snag_property(TELEMETRY_HEADERS['gyr'], incoming_telemetry)
        )
        return telemetry_data

    def unpack_serial(self) -> None | tuple[TelemetryData, SkipperState, ControlSignal]:
        if END_OF_TRANSMISSION in self._output_buffer:  # denotes that message is finished.
            _eot_location: int = self._output_buffer.rfind(END_OF_TRANSMISSION)
            _incoming_data: str = self._output_buffer[:_eot_location]
            self._output_buffer = self._output_buffer[_eot_location + len(END_OF_TRANSMISSION):]

            _telemetry_data: TelemetryData = self._read_telemetry(_incoming_data)
            _skipper_state: SkipperState = SkipperState()  # TODO: replace placeholder
            _control_signal: ControlSignal = ControlSignal()  # TODO: replace placeholder

            self.telemetry_data = _telemetry_data
            self.skipper_state = _skipper_state
            self.control_signal = _control_signal

            return self.telemetry_data, self.skipper_state, self.control_signal
        return None

    def communicate_with_serial(self):
        while True:
            self._output_buffer += self._serial.read(self._serial.in_waiting)


    def run(self):
        self.mainloop()

if __name__ == '__main__':
    monitor = Monitor()
    monitor_thread = threading.Thread(target=monitor.run)
    communicate_thread = threading.Thread(target=monitor.communicate_with_serial)

    monitor_thread.start()
    communicate_thread.start()

    communicate_thread.join()
    monitor_thread.join()
