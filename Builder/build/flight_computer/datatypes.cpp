
#include "arduino_compat.h"
#include "datatypes.h"

// ======================== BEGIN PRIVATE UTILITY FUNCTIONS ========================

namespace  // anonymous namespace means function is declared only in translation unit, making it private to this source file.
{
    void add_to_buffer(UniquePtr<char[]>& buffer, const Pair<UniquePtr<char[]>, unsigned int>& to_copy, unsigned int& buffer_offset)
    {
        compat_memcpy(buffer.get() + buffer_offset, to_copy.first.get(), to_copy.second);
        buffer_offset += to_copy.second;
    }
    void read_from_buffer(UniquePtr<char[]>& to_copy, const UniquePtr<char[]>& buffer, const unsigned int buffer_len, unsigned int& buffer_offset)
    {
        compat_memcpy(to_copy.get(), buffer.get() + buffer_offset, buffer_len);
        buffer_offset += buffer_len;
    }
}

// ======================== END PRIVATE UTILITY FUNCTIONS ========================

// ======================== BEGIN SERIALIZERS AND DESERIALIZERS FOR ALL TYPES ========================
// TODO: ADD "<???>.deserialize(<???>.serialize().first) == <???>" for all classes to Catch2 Unit Testing.

// Begin: Vector3 Serializer and Deserializer
Pair<UniquePtr<char[]>, unsigned int> Vector3::serialize() const
{
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};
    buffer_info.first = make_unique<char>(Vector3::BUFFER_SIZE);
    buffer_info.second = Vector3::BUFFER_SIZE;

    compat_memcpy(reinterpret_cast<char*>(buffer_info.first.get()), reinterpret_cast<const char*>(this->data), buffer_info.second);


    return buffer_info;
}
void Vector3::deserialize(const UniquePtr<char[]>& buffer)
{
    compat_memcpy(reinterpret_cast<char*>(this->data), reinterpret_cast<const char*>(buffer.get()), Vector3::BUFFER_SIZE);
}
// End: Vector3 Serializer and Deserializer


// Begin: IMUData Serializer and Deserializer
Pair<UniquePtr<char[]>, unsigned int> IMUData::serialize() const
{
    const Pair<UniquePtr<char[]>, unsigned int> acc_info = acc.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> gyr_info = gyr.serialize();
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};

    // These following two lines should be equivalent, but JUST IN CASE,
    //     I've put what it 100% should be. This is the same for all following
    //     instances of <???>.serialize(); however, <???>.deserialize(...) requires
    //     the use of <???>::BUFFER_SIZE, it doesn't really matter either way.
    //     TODO: ADD "<???>::BUFFER_SIZE == <...>.serialize().second" to Catch2 Unit Testing.

    // buffer_info.second = IMUData::BUFFER_SIZE;
    buffer_info.second = acc_info.second + gyr_info.second;
    buffer_info.first = make_unique<char>(buffer_info.second);

    compat_memcpy(buffer_info.first.get(), acc_info.first.get(), acc_info.second);
    compat_memcpy(buffer_info.first.get() + acc_info.second, gyr_info.first.get(), gyr_info.second);

    return buffer_info;
}
void IMUData::deserialize(const UniquePtr<char[]> &buffer)
{
    UniquePtr<char[]> acc_buffer = make_unique<char>(Vector3::BUFFER_SIZE);
    UniquePtr<char[]> gyr_buffer = make_unique<char>(Vector3::BUFFER_SIZE);

    compat_memcpy(acc_buffer.get(), buffer.get(), Vector3::BUFFER_SIZE);
    compat_memcpy(gyr_buffer.get(), buffer.get() + Vector3::BUFFER_SIZE, Vector3::BUFFER_SIZE);

    // Somewhat wasteful as we copy then move rather than copy directly, but it
    // allows us to leverage the Vector3.deserialize() rather than rewriting it.
    this->acc.deserialize(acc_buffer);
    this->gyr.deserialize(gyr_buffer);
}
// End: IMUData Serializer and Deserializer


// Begin: StateSpace Serializer and Deserializer
Pair<UniquePtr<char[]>, unsigned int> StateSpace::serialize() const
{
    const Pair<UniquePtr<char[]>, unsigned int> i_pos_info = i_pos.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> b_vel_info = b_vel.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> tb_rot_info = tb_rot.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> b_ang_vel_info = b_ang_vel.serialize();
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};

    buffer_info.second = i_pos_info.second + b_vel_info.second + tb_rot_info.second + b_ang_vel_info.second;
    buffer_info.first = make_unique<char>(buffer_info.second);

    unsigned int buffer_offset = 0;

    add_to_buffer(buffer_info.first, i_pos_info, buffer_offset);
    add_to_buffer(buffer_info.first, b_vel_info, buffer_offset);
    add_to_buffer(buffer_info.first, tb_rot_info, buffer_offset);
    add_to_buffer(buffer_info.first, b_ang_vel_info, buffer_offset);

    return buffer_info;
}
void StateSpace::deserialize(const UniquePtr<char[]> &buffer)
{
    UniquePtr<char[]> i_pos_buffer = make_unique<char>(Vector3::BUFFER_SIZE);
    UniquePtr<char[]> b_vel_buffer = make_unique<char>(Vector3::BUFFER_SIZE);
    UniquePtr<char[]> tb_rot_buffer = make_unique<char>(Vector3::BUFFER_SIZE);
    UniquePtr<char[]> b_ang_vel_buffer = make_unique<char>(Vector3::BUFFER_SIZE);

    unsigned int buffer_offset = 0;
    read_from_buffer(i_pos_buffer, buffer, Vector3::BUFFER_SIZE, buffer_offset);
    read_from_buffer(b_vel_buffer, buffer, Vector3::BUFFER_SIZE, buffer_offset);
    read_from_buffer(tb_rot_buffer, buffer, Vector3::BUFFER_SIZE, buffer_offset);
    read_from_buffer(b_ang_vel_buffer, buffer, Vector3::BUFFER_SIZE, buffer_offset);

    // Again, a little wasteful, but the overhead is worth, IMO. If you disagree, you are more
    // than welcome to rewrite with C-style pointers as long as you can definitively prove that
    // you 100% do not leak memory.
    this->i_pos.deserialize(i_pos_buffer);
    this->b_vel.deserialize(b_vel_buffer);
    this->tb_rot.deserialize(tb_rot_buffer);
    this->b_ang_vel.deserialize(b_ang_vel_buffer);
}
// End: StateSpace Serializer and Deserializer


// Begin: Control Serializer and Deserializer
Pair<UniquePtr<char[]>, unsigned int> Control::serialize() const
{
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};
    buffer_info.first = make_unique<char>(Control::BUFFER_SIZE);
    buffer_info.second = Control::BUFFER_SIZE;

    compat_memcpy(buffer_info.first.get(), reinterpret_cast<const char*>(this->data), Control::BUFFER_SIZE);

    return buffer_info;
}
void Control::deserialize(const UniquePtr<char[]> &buffer)
{
    compat_memcpy(reinterpret_cast<char*>(this->data), buffer.get(), Control::BUFFER_SIZE);
}
// End: Control Serializer and Deserializer

// Begin: TelemetryPacket Serializer and Deserializer
Pair<UniquePtr<char[]>, unsigned int> TelemetryPacket::serialize() const
{
    // Get serialization information from properties
    const Pair<UniquePtr<char[]>, unsigned int> imu_data_info = imu_data.serialize();

    const Pair<UniquePtr<char[]>, unsigned int> actual_state_space_info = actual_state_space.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> reference_state_space_info = reference_state_space.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> look_ahead_state_space_info = look_ahead_state_space.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> base_point_state_space_info = base_point_state_space.serialize();

    const Pair<UniquePtr<char[]>, unsigned int> target_control_info = target_control.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> actual_control_info = actual_control.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> derivative_control_info = derivative_control.serialize();
    const Pair<UniquePtr<char[]>, unsigned int> base_point_control_info = base_point_control.serialize();

    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};

    buffer_info.second = imu_data_info.second + actual_state_space_info.second + reference_state_space_info.second + look_ahead_state_space_info.second + base_point_state_space_info.second + target_control_info.second + actual_control_info.second + derivative_control_info.second + base_point_control_info.second;
    buffer_info.first = make_unique<char>(buffer_info.second);

    // Write serialized property data to buffer
    unsigned int buffer_offset = 0;
    add_to_buffer(buffer_info.first, imu_data_info, buffer_offset);
    add_to_buffer(buffer_info.first, actual_state_space_info, buffer_offset);
    add_to_buffer(buffer_info.first, reference_state_space_info, buffer_offset);
    add_to_buffer(buffer_info.first, look_ahead_state_space_info, buffer_offset);
    add_to_buffer(buffer_info.first, base_point_state_space_info, buffer_offset);
    add_to_buffer(buffer_info.first, target_control_info, buffer_offset);
    add_to_buffer(buffer_info.first, actual_control_info, buffer_offset);
    add_to_buffer(buffer_info.first, derivative_control_info, buffer_offset);
    add_to_buffer(buffer_info.first, base_point_control_info, buffer_offset);

    return buffer_info;
}
void TelemetryPacket::deserialize(const UniquePtr<char[]> &buffer)
{
    // Make buffer for each property
    UniquePtr<char[]> imu_data_buffer = make_unique<char>(IMUData::BUFFER_SIZE);

    UniquePtr<char[]> actual_state_space_buffer = make_unique<char>(StateSpace::BUFFER_SIZE);
    UniquePtr<char[]> reference_state_space_buffer = make_unique<char>(StateSpace::BUFFER_SIZE);
    UniquePtr<char[]> look_ahead_state_space_buffer = make_unique<char>(StateSpace::BUFFER_SIZE);
    UniquePtr<char[]> base_point_state_space_buffer = make_unique<char>(StateSpace::BUFFER_SIZE);

    UniquePtr<char[]> target_control_buffer = make_unique<char>(Control::BUFFER_SIZE);
    UniquePtr<char[]> actual_control_buffer = make_unique<char>(Control::BUFFER_SIZE);
    UniquePtr<char[]> derivative_control_buffer = make_unique<char>(Control::BUFFER_SIZE);
    UniquePtr<char[]> base_point_control_buffer = make_unique<char>(Control::BUFFER_SIZE);

    // Copy and split the buffer into its corresponding property buffer
    unsigned int buffer_offset = 0;
    read_from_buffer(imu_data_buffer, buffer, IMUData::BUFFER_SIZE, buffer_offset);

    read_from_buffer(actual_state_space_buffer, buffer, StateSpace::BUFFER_SIZE, buffer_offset);
    read_from_buffer(reference_state_space_buffer, buffer, StateSpace::BUFFER_SIZE, buffer_offset);
    read_from_buffer(look_ahead_state_space_buffer, buffer, StateSpace::BUFFER_SIZE, buffer_offset);
    read_from_buffer(base_point_state_space_buffer, buffer, StateSpace::BUFFER_SIZE, buffer_offset);

    read_from_buffer(target_control_buffer, buffer, Control::BUFFER_SIZE, buffer_offset);
    read_from_buffer(actual_control_buffer, buffer, Control::BUFFER_SIZE, buffer_offset);
    read_from_buffer(derivative_control_buffer, buffer, Control::BUFFER_SIZE, buffer_offset);
    read_from_buffer(base_point_control_buffer, buffer, Control::BUFFER_SIZE, buffer_offset);

    // Move each buffer into its corresponding property
    this->imu_data.deserialize(imu_data_buffer);

    this->actual_state_space.deserialize(actual_state_space_buffer);
    this->reference_state_space.deserialize(reference_state_space_buffer);
    this->look_ahead_state_space.deserialize(look_ahead_state_space_buffer);
    this->base_point_state_space.deserialize(base_point_state_space_buffer);

    this->target_control.deserialize(target_control_buffer);
    this->actual_control.deserialize(actual_control_buffer);
    this->derivative_control.deserialize(derivative_control_buffer);
    this->base_point_control.deserialize(base_point_control_buffer);
}
// End: TelemetryPacket Serializer and Deserializer

// Begin: Message Serializer and Deserializer

template<unsigned int packet_size>
Pair<UniquePtr<char[]>, unsigned int> Message<packet_size>::serialize() const
{
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};
    buffer_info.second = Message<packet_size>::BUFFER_SIZE;
    buffer_info.first = make_unique<char>(buffer_info.second);

    compat_memcpy(buffer_info.first.get(), this->data, Message<packet_size>::BUFFER_SIZE);

    return buffer_info;
}
template<unsigned int packet_size>
void Message<packet_size>::deserialize(const UniquePtr<char[]>& buffer)
{
    compat_memcpy(this->data, buffer.get(), Message<packet_size>::BUFFER_SIZE);
}

// End: Message Serializer and Deserializer

// Begin: Command Packet Serializer and Deserializer

Pair<UniquePtr<char[]>, unsigned int> CommandPacket::serialize() const
{
    const Pair<UniquePtr<char[]>, unsigned int> message_info = message.serialize();
    Pair<UniquePtr<char[]>, unsigned int> buffer_info{};

    buffer_info.second = message_info.second;
    buffer_info.first = make_unique<char>(buffer_info.second);

    unsigned int buffer_offset = 0;
    add_to_buffer(buffer_info.first, message_info, buffer_offset);

    return buffer_info;
}
void CommandPacket::deserialize(const UniquePtr<char[]>& buffer)
{
    UniquePtr<char[]> message_buffer = make_unique<char>(Message<MESSAGE_SIZE>::BUFFER_SIZE);

    unsigned int buffer_offset = 0;
    read_from_buffer(message_buffer, buffer, Message<MESSAGE_SIZE>::BUFFER_SIZE, buffer_offset);
    this->message.deserialize(message_buffer);
}

// End: Command Packet Serializer and Deserializer

// ======================== END SERIALIZERS AND DESERIALIZERS FOR ALL TYPES ========================

// ======================== BEGIN DESERIALIZATION CONSTRUCTORS ========================

Vector3::Vector3(UniquePtr<char[]> &&buffer)
{
    Vector3::deserialize(buffer);
}
IMUData::IMUData(UniquePtr<char[]> &&buffer)
{
    IMUData::deserialize(buffer);
}
StateSpace::StateSpace(UniquePtr<char[]> &&buffer)
{
    StateSpace::deserialize(buffer);
}
Control::Control(UniquePtr<char[]> &&buffer)
{
    Control::deserialize(buffer);
}
TelemetryPacket::TelemetryPacket(UniquePtr<char[]> &&buffer)
{
    TelemetryPacket::deserialize(buffer);
}
template<unsigned int packet_size>
Message<packet_size>::Message(UniquePtr<char[]> &&buffer)
{
    Message<packet_size>::deserialize(buffer);
}
CommandPacket::CommandPacket(UniquePtr<char[]> &&buffer)
{
    CommandPacket::deserialize(buffer);
}

// ======================== END DESERIALIZATION CONSTRUCTORS ========================


