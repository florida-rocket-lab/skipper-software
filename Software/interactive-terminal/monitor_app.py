import tkinter as tk
from tkinter.scrolledtext import ScrolledText
from dataclasses import dataclass
import numpy as np
import threading
from typing import *
import serial

BAUD_RATE: int = 115200
REFRESH_RATE: int = 60  # How many times a second the gui refreshes its data.
APPLICATION_NAME: str = "Skipper Information Application"
END_OF_TRANSMISSION: str = "----------------------\n"
STDOUT_HEADER: str = "STDOUT: "
STDERR_HEADER: str = "STDERR: "
STD_CHAR_REPLACE: dict[str, str] = {  # IMPORTANT: \n denotes end of line so rather than using \n, use \t for newline and \\t for tab!
    '\t': '\n',
    '\\t': '\t'
}
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
COMMANDS = {  # formated: "<BUTTON TEXT>": "<SERIAL COMMAND>"; NOTE: the button "object_id" IS the lowercase version of "<SERIAL COMMAND>"!
    "Make Skipper idle": "IDLE",
    "Arm Skipper for takeoff": "ARM",
    "Tell Skipper to takeoff": "TAKEOFF",
    "Tell Skipper to hover": "HOVER",
    "Make Skipper land ": "LAND"
}

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

    _historical_telemetry_data: np.ndarray = np.ndarray([])
    _historical_skipper_state: np.ndarray = np.ndarray([])
    _historical_control_signal: np.ndarray = np.ndarray([])

    _output_buffer: str = ""  # raw output from the Arduino "Ground"
    _input_buffer: str = ""  # input to the Arduino "Ground"
    _stdout_buffer: str = ""  # Arduino's "STDOUT" now that we've hijacked the Serial.
    _stderr_buffer: str = ""  # Arduino's "STDERR" now

    _tkinter_objects: dict[str, tk.BaseWidget] = {}
    _refresh_time: int = int(1000.0 / APPLICATION_FPS)

    def __init__(self):
        # we gotta make a simple tkinter app to view and save the data
        # somewhere in __init__ we need to establish connection with Serial ("Ground" Arduino USB connection)

        super(Monitor, self).__init__(screenName=APPLICATION_NAME, baseName=APPLICATION_NAME, className=APPLICATION_NAME)
        self.geometry("700x600")
        self.resizable(height=None, width=None)
        self.state("zoomed")

        # The following just initializes a scrollable window; just use "self._tkinter_objects['--virtual-root']" rather than "self"
        self._tkinter_objects['--root-frame'] = tk.Frame(self)
        self._tkinter_objects['--root-canvas'] = tk.Canvas(self._tkinter_objects['--root-frame'])
        self._tkinter_objects['--virtual-root'] = tk.Frame(self._tkinter_objects['--root-canvas'])
        self._tkinter_objects['--root-scrollbar'] = tk.Scrollbar(self._tkinter_objects['--root-frame'], orient="vertical", command=self._tkinter_objects['--root-canvas'].yview)
        self._tkinter_objects['--virtual-root'].bind(
            "<Configure>",
            lambda e: self._tkinter_objects['--root-canvas'].configure(
                scrollregion=self._tkinter_objects['--root-canvas'].bbox("all")
            )
        )
        self._tkinter_objects['--root-canvas'].create_window((0, 0), window=self._tkinter_objects['--virtual-root'], anchor="nw")
        self._tkinter_objects['--root-canvas'].configure(yscrollcommand=self._tkinter_objects['--root-scrollbar'].set)

        self._tkinter_objects['--root-frame'].pack(fill="both", expand=True)
        self._tkinter_objects['--root-canvas'].pack(side="left", fill="both", expand=True)
        self._tkinter_objects['--root-scrollbar'].pack(side="right", fill="y")

        self._serial = serial.Serial("COM3", BAUD_RATE, timeout=10.0)
        self._init_tkinter_objects()  # This function initializes the actual Skipper GUI.

    def _init_tkinter_objects(self):
        _text_box_data: dict[str, str] = self._generate_box_text()
        for _obj in DATA_ORDER:
            self._create_readonly_text_obj(_text_box_data[_obj], _obj)
        for _button_text, _button_command in COMMANDS.items():
            self._create_serial_button_obj(text=_button_text, object_id=_button_command.lower(), command=_button_command)
        self._create_readonly_text_obj('Skipper\'s STDOUT:', 'stdout-label')
        self._create_scrollable_text_obj('stdout')
        self._create_readonly_text_obj('Skipper\'s STDERR:', 'stdout-label')
        self._create_scrollable_text_obj('stderr')

    def _create_scrollable_text_obj(self, object_id: str, starting_text: str = ""):
        _text: ScrolledText = ScrolledText(self._tkinter_objects['--virtual-root'], undo=True)
        _text.pack(padx=10, pady=5, fill="both")
        _text.insert(tk.END, starting_text)
        _text.config(state=tk.DISABLED)
        self._tkinter_objects[object_id] = _text

    def _create_serial_button_obj(self, text: str, object_id: str, command: str) -> None:
        def _send_function():
            self._input_buffer = command + "\n"
        _button: tk.Button = tk.Button(self._tkinter_objects['--virtual-root'], text=text, command=_send_function)
        _button.pack(padx=10, pady=5)
        self._tkinter_objects[object_id] = _button

    def _create_readonly_text_obj(self, text: str, object_id: str) -> None:
        _text_box: tk.Label = tk.Label(self._tkinter_objects['--virtual-root'], text=text)
        _text_box.pack(padx=10, pady=5)
        _text_box.config(state=tk.DISABLED)
        self._tkinter_objects[object_id] = _text_box

    def _update_readonly_text_obj(self, text: str, object_id: str) -> None:
        self._tkinter_objects[object_id].config(text=text)

    def _append_readonly_text_obj(self, text: str, object_id: str) -> None:
        self._tkinter_objects[object_id].insert(tk.END, text)

    def _update_std(self):
        self._append_readonly_text_obj(self._stdout_buffer, 'stdout')
        self._stdout_buffer = ""
        self._append_readonly_text_obj(self._stderr_buffer, 'stderr')
        self._stderr_buffer = ""

    def _tkinter_update_loop(self):
        self.unpack_serial()
        self._update_std()
        self.save_state()
        text = self._generate_box_text()
        for object_id, text_value in text.items():
            self._update_readonly_text_obj(text_value, object_id)
        self.after(self._refresh_time, self._tkinter_update_loop)

    def _unwrap_object(self, struct: TelemetryData | SkipperState | ControlSignal):
        output: np.ndarray = np.ndarray([])
        for name, value in vars(struct):
            if name.startswith('_'):
                continue
            if isinstance(value, np.ndarray):
                output = np.append(output, value)
            elif isinstance(value, (float, int)):
                output = np.append(output, (value,))
        return output

    def save_state(self):
        self._historical_telemetry_data = np.append(self._historical_telemetry_data, (self._unwrap_object(self.telemetry_data),))
        self._historical_skipper_state = np.append(self._historical_skipper_state, (self._unwrap_object(self.skipper_state),))
        self._historical_control_signal = np.append(self._historical_control_signal, (self._unwrap_object(self.control_signal),))

        np.save(SAVE_LOCATIONS['telemetry-data'], self._historical_telemetry_data)
        np.save(SAVE_LOCATIONS['control-signal'], self._historical_control_signal)
        np.save(SAVE_LOCATIONS['skipper-state'], self._historical_skipper_state)

    def _generate_box_text(self) -> dict[str, str]:
        _output: dict[str, str] = {
            'acc': "Acceleration:\n{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.acc),
            'vel': "Velocity:\n{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.vel),
            'pos': "Position:\n{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.pos),
            'gyr': "Gyroscopic Data:\n{:6.2f}, {:6.2f}, {:6.2f}".format(*self.telemetry_data.gyr)
        }
        return _output

    @staticmethod
    def _snag_property(property_label: str, incoming_data: str, validate: bool = True) -> str:
        _property_location: int = incoming_data.rfind(property_label)
        if validate: assert _property_location != -1, f"Could not find \"{property_label}\" in parameter \"incoming_data\" in class method Monitor._snag_property(...).\nReceived: \n{incoming_data}"
        if _property_location != -1:  # almost useless except for the no-validation case.
            incoming_data = incoming_data[_property_location+len(property_label):]
            incoming_data = incoming_data[:incoming_data.find('\n')]
        else:
            incoming_data = ""
        if validate: assert incoming_data, f"Found \"{property_label}\" in parameter \"incoming_data\" in class method Monitor._snag_property(...), but no data was found.\nReceived: \n{incoming_data}"
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

    def extract_std(self, input_string: None | str = None) -> str:
        if input_string is None:
            self._move_to_std_buffer("stdout")
            self._move_to_std_buffer("stderr")
            return self._output_buffer
        _i1: str = self._move_to_std_buffer("stdout", input_string)
        _i2: str = self._move_to_std_buffer("stderr", _i1)
        return _i2

    def _move_to_std_buffer(self, std_type: Literal["stdout", "stderr"], input_string: None | str = None) -> str:
        _header = STDOUT_HEADER if std_type == "stdout" else STDERR_HEADER
        _output_buffer_cp: str = self._output_buffer  # remember that self._output_buffer can change really quickly, so a copy is well-advised.
        _read_string: str = self._output_buffer if input_string is None else input_string

        while _header in _read_string:
            _std_start: int = _read_string.find(_header)
            _std: str = _read_string[_std_start:]
            if '\n' not in _std:
                break  # STDOUT message not finished yet; the serial is still reading it in.
            _std_len: int = _std.find('\n')
            _std = _std[len(_header):_std_len-1]
            _read_string = _read_string[:_std_start] + _read_string[_std_start+_std_len:]  # cut STDOUT from the message...
            for ori, repl in STD_CHAR_REPLACE.items():
                _std = _std.replace(ori, repl)
            match std_type:
                case "stdout":
                    self._stdout_buffer += _std
                case _:
                    self._stderr_buffer += _std

        if input_string is None:
            self._output_buffer.replace(_output_buffer_cp, _read_string)  # we replace instead of reset because of potential incoming new data!
        return _read_string

    def unpack_serial(self) -> None | tuple[TelemetryData, SkipperState, ControlSignal]:
        self.extract_std()
        if END_OF_TRANSMISSION in self._output_buffer:  # denotes that message is finished.
            _eot_location: int = self._output_buffer.rfind(END_OF_TRANSMISSION)
            _incoming_data: str = self._output_buffer[:_eot_location]
            _incoming_data = self.extract_std(_incoming_data)  # read "STDOUT" and "STDERR" to catch them from in the middle!
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
            if "\n" in self._input_buffer:  # the newline character is how I know the command is finished.
                print(f"SUCCESSFULLY SENT COMMAND \"{self._input_buffer[:-1]}\"...")
                self._serial.write(self._input_buffer.encode(encoding='ascii'))  # only allow ascii commands, no emoji commands allowed
                self._input_buffer = ""

    def run(self):
        self.after(self._refresh_time, self._tkinter_update_loop)
        self.mainloop()

if __name__ == '__main__':
    monitor = Monitor()
    communicate_thread = threading.Thread(target=monitor.communicate_with_serial)
    communicate_thread.start()  # non-daemon :)
    communicate_thread.join()
    monitor.run()
