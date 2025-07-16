#ifndef SKIPPER_GNC_DATATYPES_H
#define SKIPPER_GNC_DATATYPES_H

#include "arduino_compat.h"
#include "constants.h"
using fp_t = float;

class BaseSerializable {
public:
    virtual ~BaseSerializable() = default;
    virtual Pair<UniquePtr<char[]>, unsigned int> serialize() const = 0;
    virtual void deserialize(const UniquePtr<char[]>& buffer) = 0;
};

struct Vector3 : public BaseSerializable {
    fp_t data[3] = {0, 0, 0};
    fp_t& x = data[0];
    fp_t& y = data[1];
    fp_t& z = data[2];

    // ctors
    Vector3() = default;
    Vector3(fp_t x_, fp_t y_, fp_t z_)            : data{x_, y_, z_} {}
    explicit Vector3(fp_t* p)                     : data{p[0], p[1], p[2]} {}
    explicit Vector3(const UniquePtr<fp_t[]>& p)   : data{p[0], p[1], p[2]} {}
    explicit Vector3(UniquePtr<char[]>&& buf);

    // (de)serialisers
    static constexpr unsigned int BUFFER_SIZE = 3 * sizeof(fp_t);
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;

    // mutators
    template<typename... Args> void set_data(Args&&... args) {
        fp_t tmp[3] = {static_cast<fp_t>(args)...};
        compat_memcpy(data, tmp, sizeof(tmp));
    }
};

struct IMUData : public BaseSerializable {
    Vector3 acc {};
    Vector3 gyr {};

    IMUData() = default;
    explicit IMUData(UniquePtr<char[]>&& buf);
    explicit IMUData(fp_t* p) : acc{p}, gyr{p + 3} {}

    static constexpr unsigned int BUFFER_SIZE = 2 * Vector3::BUFFER_SIZE;
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

struct StateSpace : public BaseSerializable {
    Vector3 i_pos {}, b_vel {}, tb_rot {}, b_ang_vel {};

    StateSpace() = default;
    explicit StateSpace(UniquePtr<char[]>&& buf);
    explicit StateSpace(fp_t* p)
        : i_pos{p}, b_vel{p + 3}, tb_rot{p + 6}, b_ang_vel{p + 9} {}

    static constexpr unsigned int BUFFER_SIZE = 4 * Vector3::BUFFER_SIZE;
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

struct Control : public BaseSerializable {
    fp_t data[4] = {0,0,0,0};
    fp_t& T      = data[0];
    fp_t& xi     = data[1];
    fp_t& zeta   = data[2];
    fp_t& tauRCS = data[3];

    Control() = default;
    Control(fp_t T_, fp_t x_, fp_t z_, fp_t t_) : data{T_,x_,z_,t_} {}
    explicit Control(fp_t* p)                  : data{p[0],p[1],p[2],p[3]} {}
    explicit Control(const UniquePtr<fp_t[]>& p): data{p[0],p[1],p[2],p[3]}{}
    explicit Control(UniquePtr<char[]>&& buf);

    static constexpr unsigned int BUFFER_SIZE = 4 * sizeof(fp_t);
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

template<unsigned int packet_size>
struct Message : public BaseSerializable {
    char data[packet_size]{};

    Message() = default;
    explicit Message(UniquePtr<char[]>&& buf);

    static constexpr unsigned int BUFFER_SIZE = packet_size;
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

struct TelemetryPacket : public BaseSerializable {
    IMUData     imu_data{};
    StateSpace  actual_state_space{};
    StateSpace  look_ahead_state_space{};
    Control     target_control{};
    Control     actual_control{};
    Control     derivative_control{};
    Message<256> out_stream;
    Message<256> err_stream;

    TelemetryPacket() = default;
    explicit TelemetryPacket(UniquePtr<char[]>&& buf);

    TelemetryPacket(fp_t* d, UniquePtr<char[]>& o, UniquePtr<char[]>& e) :
        imu_data{d},
        actual_state_space{ d + IMUData::BUFFER_SIZE / sizeof(fp_t) },
        look_ahead_state_space{ d + (IMUData::BUFFER_SIZE + StateSpace::BUFFER_SIZE) / sizeof(fp_t) },
        target_control{ d + (IMUData::BUFFER_SIZE + 2*StateSpace::BUFFER_SIZE) / sizeof(fp_t) },
        actual_control{ d + (IMUData::BUFFER_SIZE + 2*StateSpace::BUFFER_SIZE + Control::BUFFER_SIZE) / sizeof(fp_t) },
        derivative_control{ d + (IMUData::BUFFER_SIZE + 2*StateSpace::BUFFER_SIZE + 2*Control::BUFFER_SIZE) / sizeof(fp_t) },
        out_stream(Move(o)), err_stream(Move(e)) { }

    static constexpr unsigned int BUFFER_SIZE =
          IMUData::BUFFER_SIZE
        + 2 * StateSpace::BUFFER_SIZE
        + 3 * Control::BUFFER_SIZE;       // 24 + 96 + 48 = 168 B

    static_assert(BUFFER_SIZE ==
        IMUData::BUFFER_SIZE + 2*StateSpace::BUFFER_SIZE + 3*Control::BUFFER_SIZE,
        "TelemetryPacket size math wrong");

    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

struct CommandPacket : public BaseSerializable {
    Message<MESSAGE_SIZE> message{};

    CommandPacket() = default;
    explicit CommandPacket(UniquePtr<char[]>&& buf);

    static constexpr unsigned int BUFFER_SIZE = Message<MESSAGE_SIZE>::BUFFER_SIZE;
    Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>&) override;
};

#endif
