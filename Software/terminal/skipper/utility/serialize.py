from abc import ABC, abstractmethod
import io
import ctypes
import os

_CONSTANTS_PATH: str = os.path.join(os.path.abspath(__file__), "..", "..", "..", "shared", "constants.h")
if not os.path.isfile(_CONSTANTS_PATH):
    import warnings
    warnings.warn("Could not find \"/Software/shared/constants.h\" to automatically read MESSAGE_SIZE macro... defaulting to 32.")
    MESSAGE_SIZE: int = 32
else:
    _message_size_label: str = "#define MESSAGE_SIZE "
    src = open(_CONSTANTS_PATH).read()
    if _message_size_label not in src:
        import warnings
        warnings.warn("Found \"/Software/shared/constants.h\", but could not find the \"#define\" constant \"MESSAGE_SIZE\"...")
    src = src[src.find(_message_size_label) + len(_message_size_label):]
    if '\n' in src: src = src[:src.find('\n')]
    MESSAGE_SIZE: int = int(src)

class BaseSerializable(ABC):

    BUFFER_SIZE: int = 0

    @abstractmethod
    def serialize(self, buffer: io.BytesIO = None) -> bytes | None: pass
    @abstractmethod
    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None: pass

class Vector3(BaseSerializable):

    ENTRY_SIZE: int = ctypes.sizeof(ctypes.c_double)
    BUFFER_SIZE: int = 3 * ENTRY_SIZE

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0):
        self.x: ctypes.c_double = ctypes.c_double(x)
        self.y: ctypes.c_double = ctypes.c_double(y)
        self.z: ctypes.c_double = ctypes.c_double(z)

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            return ctypes.string_at(ctypes.byref(self.x), self.ENTRY_SIZE) + \
                ctypes.string_at(ctypes.byref(self.y), self.ENTRY_SIZE) + \
                ctypes.string_at(ctypes.byref(self.z), self.ENTRY_SIZE)
        buffer.write(ctypes.string_at(ctypes.byref(self.x), self.ENTRY_SIZE))
        buffer.write(ctypes.string_at(ctypes.byref(self.y), self.ENTRY_SIZE))
        buffer.write(ctypes.string_at(ctypes.byref(self.z), self.ENTRY_SIZE))

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        ctypes.memmove(ctypes.byref(self.x), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)
        ctypes.memmove(ctypes.byref(self.y), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)
        ctypes.memmove(ctypes.byref(self.z), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)

class IMUData(BaseSerializable):

    BUFFER_SIZE: int = 2 * Vector3.BUFFER_SIZE

    def __init__(self,
                 ax: float = 0.0, ay: float = 0.0, az: float = 0.0,
                 gx: float = 0.0, gy: float = 0.0, gz: float = 0.0):
        self.acc = Vector3(ax, ay, az)
        self.gyr = Vector3(gx, gy, gz)

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            buffer = io.BytesIO()
            self.acc.serialize(buffer)
            self.gyr.serialize(buffer)
            return buffer.getvalue()
        self.acc.serialize(buffer)
        self.gyr.serialize(buffer)

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        self.acc.deserialize(buffer)
        self.gyr.deserialize(buffer)

class StateSpace(BaseSerializable):

    BUFFER_SIZE: int = 4 * Vector3.BUFFER_SIZE

    def __init__(self):
        self.i_pos = Vector3()
        self.b_vel = Vector3()
        self.tb_rot = Vector3()
        self.b_ang_vel = Vector3()

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            buffer = io.BytesIO()
            self.i_pos.serialize(buffer)
            self.b_vel.serialize(buffer)
            self.tb_rot.serialize(buffer)
            self.b_ang_vel.serialize(buffer)
            return buffer.getvalue()
        self.i_pos.serialize(buffer)
        self.b_vel.serialize(buffer)
        self.tb_rot.serialize(buffer)
        self.b_ang_vel.serialize(buffer)

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        self.i_pos.deserialize(buffer)
        self.b_vel.deserialize(buffer)
        self.tb_rot.deserialize(buffer)
        self.b_ang_vel.deserialize(buffer)

class Control(BaseSerializable):

    ENTRY_SIZE: int = ctypes.sizeof(ctypes.c_double)
    BUFFER_SIZE: int = 4 * ENTRY_SIZE

    def __init__(self, T: float = 0.0, xi: float = 0.0, zeta: float = 0.0, tauRCS: float = 0.0):
        self.T: ctypes.c_double = ctypes.c_double(T)
        self.xi: ctypes.c_double = ctypes.c_double(xi)
        self.zeta: ctypes.c_double = ctypes.c_double(zeta)
        self.tauRCS: ctypes.c_double = ctypes.c_double(tauRCS)

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            return ctypes.string_at(ctypes.byref(self.T), self.ENTRY_SIZE) + \
                ctypes.string_at(ctypes.byref(self.xi), self.ENTRY_SIZE) + \
                ctypes.string_at(ctypes.byref(self.zeta), self.ENTRY_SIZE) + \
                ctypes.string_at(ctypes.byref(self.tauRCS), self.ENTRY_SIZE)
        buffer.write(ctypes.string_at(ctypes.byref(self.T), self.ENTRY_SIZE))
        buffer.write(ctypes.string_at(ctypes.byref(self.xi), self.ENTRY_SIZE))
        buffer.write(ctypes.string_at(ctypes.byref(self.zeta), self.ENTRY_SIZE))
        buffer.write(ctypes.string_at(ctypes.byref(self.tauRCS), self.ENTRY_SIZE))

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        ctypes.memmove(ctypes.byref(self.T), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)
        ctypes.memmove(ctypes.byref(self.xi), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)
        ctypes.memmove(ctypes.byref(self.zeta), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)
        ctypes.memmove(ctypes.byref(self.tauRCS), buffer.read(self.ENTRY_SIZE), self.ENTRY_SIZE)

class TelemetryPacket(BaseSerializable):

    BUFFER_SIZE: int = IMUData.BUFFER_SIZE + 4 * StateSpace.BUFFER_SIZE + 4 * Control.BUFFER_SIZE

    def __init__(self):
        self.imu_data = IMUData()

        self.actual_state_space = StateSpace()
        self.reference_state_space = StateSpace()
        self.look_ahead_state_space = StateSpace()
        self.base_point_state_space = StateSpace()

        self.target_control = Control()
        self.actual_control = Control()
        self.derivative_control = Control()
        self.base_point_control = Control()

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            buffer = io.BytesIO()
            for prop in (self.imu_data, self.actual_state_space, self.reference_state_space,
                         self.look_ahead_state_space, self.base_point_state_space, self.target_control,
                         self.actual_control, self.derivative_control, self.base_point_control):
                prop.serialize(buffer)
            return buffer.getvalue()
        for prop in (self.imu_data, self.actual_state_space, self.reference_state_space,
                     self.look_ahead_state_space, self.base_point_state_space, self.target_control,
                     self.actual_control, self.derivative_control, self.base_point_control):
            prop.serialize(buffer)

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        for prop in (self.imu_data, self.actual_state_space, self.reference_state_space,
                     self.look_ahead_state_space, self.base_point_state_space, self.target_control,
                     self.actual_control, self.derivative_control, self.base_point_control):
            prop.deserialize(buffer)

class Message(BaseSerializable):

    BUFFER_SIZE: int = MESSAGE_SIZE * ctypes.sizeof(ctypes.c_char)

    def __init__(self, message_size: int = MESSAGE_SIZE):
        self.BUFFER_SIZE = message_size * ctypes.sizeof(ctypes.c_char)
        self.data = ctypes.create_string_buffer(message_size)
    
    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            return bytes(self.data)
        buffer.write(self.data)

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            self.data = ctypes.create_string_buffer(bytestring)
            return
        self.data = ctypes.create_string_buffer(buffer.read(self.BUFFER_SIZE))

class CommandPacket(BaseSerializable):

    BUFFER_SIZE: int = Message.BUFFER_SIZE

    def __init__(self):
        self.msg = Message()

    def serialize(self, buffer: io.BytesIO = None) -> bytes | None:
        if buffer is None:
            buffer = io.BytesIO()
            self.msg.serialize(buffer)
            return buffer.getvalue()
        self.msg.serialize(buffer)

    def deserialize(self, buffer: io.BytesIO = None, bytestring: bytes = None) -> None:
        if buffer is None:
            buffer = io.BytesIO(bytestring)
        self.msg.deserialize(buffer)
