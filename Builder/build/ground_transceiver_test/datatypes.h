
#ifndef SKIPPER_GNC_DATATYPES_H
#define SKIPPER_GNC_DATATYPES_H

#include "arduino_compat.h"
#include "constants.h"

struct BaseSerializable
{
    virtual Pair<UniquePtr<char[]>, unsigned int> serialize() const = 0;
    virtual void deserialize(const UniquePtr<char[]>& buffer) = 0;
};

struct Vector3 : public BaseSerializable
{
    // CONSTRUCTORS
    Vector3() = default;
    Vector3(double x_, double y_, double z_): data{x_, y_, z_} {};
    explicit Vector3(double* data_): data{data_[0], data_[1], data_[2]} {};
    explicit Vector3(const UniquePtr<double>& data_): data{data_.get()[0], data_.get()[1], data_.get()[2]} {}
    explicit Vector3(UniquePtr<char[]>&& buffer);  // construct from buffer

    // INTERNAL DATA
    double data[3] = {0, 0, 0};
    double& x = data[0];
    double& y = data[1];
    double& z = data[2];

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = 3 * sizeof(double);
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;

    // MUTATORS
    void set_data(double x_, double y_, double z_)
    {
        this->data[0] = x_;
        this->data[1] = y_;
        this->data[2] = z_;
    };
    void set_data(const double* data_)
    {
        this->data[0] = data_[0];
        this->data[1] = data_[1];
        this->data[2] = data_[2];
    };
    void set_data(const UniquePtr<double>& data_)
    {
    this->data[0] = data_.get()[0];
    this->data[1] = data_.get()[1];
    this->data[2] = data_.get()[2];
    };
    void set_data(const Vector3& data_)
    {
        this->data[0] = data_.x;
        this->data[1] = data_.y;
        this->data[2] = data_.z;
    };
};

struct IMUData : public BaseSerializable
{
    // CONSTRUCTORS
    IMUData() = default;
    explicit IMUData(UniquePtr<char[]>&& buffer);  // construct from buffer
    explicit IMUData(double* data_): acc{data_[0], data_[1], data_[2]}, gyr{data_[3], data_[4], data_[5]} {};

    // INTERNAL DATA
    Vector3 acc{};
    Vector3 gyr{};

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = 2 * Vector3::BUFFER_SIZE;
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;

    // MUTATORS
    template <typename... Args>  // Thanks to Anthony Thisse for coming in clutch
    void set_acc(Args&&... args) {acc.set_data(Forward<Args>(args)...);}
    template <typename... Args>
    void set_gyr(Args&&... args) {gyr.set_data(Forward<Args>(args)...);}
};

struct StateSpace : public BaseSerializable
{
    // CONSTRUCTORS
    StateSpace() = default;
    explicit StateSpace(UniquePtr<char[]>&& buffer);  // construct from buffer
    explicit StateSpace(double* data_):
        i_pos{data_[0], data_[1], data_[2]},
        b_vel{data_[3], data_[4], data_[5]},
        tb_rot{data_[6], data_[7], data_[8]},
        b_ang_vel{data_[9], data_[10], data_[11]} {};

    // INTERNAL DATA
    Vector3 i_pos{};     // position in inertial frame
    Vector3 b_vel{};     // velocity in body frame
    Vector3 tb_rot{};    // rotation in 3-2-1 Tait-Bryan angles
    Vector3 b_ang_vel{}; // angular velocity in body frame

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = 4 * Vector3::BUFFER_SIZE;
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;

    // MUTATORS
    template <typename... Args>  // Thanks to Anthony Thisse for coming in clutch
    void set_i_pos(Args&&... args) {i_pos.set_data(Forward<Args>(args)...);}
    template <typename... Args>
    void set_b_vel(Args&&... args) {b_vel.set_data(Forward<Args>(args)...);}
    template <typename... Args>
    void set_tb_rot(Args&&... args) {tb_rot.set_data(Forward<Args>(args)...);}
    template <typename... Args>
    void set_b_ang_vel(Args&&... args) {b_ang_vel.set_data(Forward<Args>(args)...);}
};

struct Control : public BaseSerializable
{
    // INDICES
    static constexpr unsigned int THRUST_IDX = 0;
    static constexpr unsigned int PRIMARY_GIMBAL_IDX = 1;
    static constexpr unsigned int SECONDARY_GIMBAL_IDX = 2;
    static constexpr unsigned int BODY_TORQUE_IDX = 3;

    // CONSTRUCTORS
    Control() = default;
    Control(double T_, double x_, double z_, double t_): data{T_, x_, z_, t_} {};
    explicit Control(double* data_): data{data_[0], data_[1], data_[2], data_[3]} {};
    explicit Control(const UniquePtr<double[]>& data_): data{data_[0], data_[1], data_[2], data_[3]} {};
    explicit Control(UniquePtr<char[]>&& buffer);  // construct from buffer

    // INTERNAL DATA
    double data[4] = {0, 0, 0, 0};
    double& T = data[THRUST_IDX];
    double& xi = data[PRIMARY_GIMBAL_IDX];
    double& zeta = data[SECONDARY_GIMBAL_IDX];
    double& tauRCS = data[BODY_TORQUE_IDX];

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = 4 * sizeof(double);
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;

    // MUTATORS
    void set_data(double T_, double x_, double z_, double t_)
    {
        this->data[0] = T_;
        this->data[1] = x_;
        this->data[2] = z_;
        this->data[3] = t_;
    };
    void set_data(const double* data_)
    {
        this->data[0] = data_[0];
        this->data[1] = data_[1];
        this->data[2] = data_[2];
        this->data[3] = data_[3];
    };
    void set_data(const UniquePtr<double[]>& data_)
    {
        this->data[0] = data_[0];
        this->data[1] = data_[1];
        this->data[2] = data_[2];
        this->data[3] = data_[3];
    };
    void set_data(const Control& data_)
    {
        this->data[0] = data_.T;
        this->data[1] = data_.xi;
        this->data[2] = data_.zeta;
        this->data[3] = data_.tauRCS;
    };
};
template<unsigned int packet_size>
struct Message : public BaseSerializable
{
    // CONSTRUCTORS
    Message() = default;
    explicit Message(UniquePtr<char[]>&& buffer);

    char data[packet_size]{};

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = packet_size;
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;
};

struct TelemetryPacket : public BaseSerializable
{
    // CONSTRUCTORS
    TelemetryPacket() = default;
    explicit TelemetryPacket(UniquePtr<char[]>&& buffer);  // construct from buffer

    TelemetryPacket(double* data_, UniquePtr<char[]>& out_, UniquePtr<char[]>& err_) :
        imu_data{data_},
         actual_state_space{data_ + StateSpace::BUFFER_SIZE/sizeof(double)},
        reference_state_space{data_ + 2*StateSpace::BUFFER_SIZE/sizeof(double)}, 
        look_ahead_state_space{data_ + 3*StateSpace::BUFFER_SIZE/sizeof(double)},
        base_point_state_space{data_ + 4*StateSpace::BUFFER_SIZE/sizeof(double)},
        target_control{data_ + 5*StateSpace::BUFFER_SIZE/sizeof(double)},
        actual_control{data_ + (5*StateSpace::BUFFER_SIZE + Control::BUFFER_SIZE)/sizeof(double)},
        derivative_control{data_ + (5*StateSpace::BUFFER_SIZE + 2*Control::BUFFER_SIZE)/sizeof(double)}, 
        base_point_control{data_ + (5*StateSpace::BUFFER_SIZE + 3*Control::BUFFER_SIZE)/sizeof(double)},
        out_stream(Move(out_)), err_stream(Move(err_)) 
        {};

    // INTERNAL DATA
    IMUData imu_data{};

    StateSpace actual_state_space{};
    StateSpace reference_state_space{};
    StateSpace look_ahead_state_space{};
    StateSpace base_point_state_space{};

    Control target_control{};
    Control actual_control{};
    Control derivative_control{};
    Control base_point_control{};

    Message<256> out_stream;
    Message<256> err_stream;

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = 
    IMUData::BUFFER_SIZE +
    4 * StateSpace::BUFFER_SIZE +
    4 * Control::BUFFER_SIZE;

    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;
};


struct CommandPacket : public BaseSerializable
{
    // CONSTRUCTORS
    CommandPacket() = default;
    explicit CommandPacket(UniquePtr<char[]>&& buffer);

    Message<MESSAGE_SIZE> message{};

    // SERIALIZERS
    static constexpr unsigned int BUFFER_SIZE = Message<MESSAGE_SIZE>::BUFFER_SIZE;
    [[nodiscard]] Pair<UniquePtr<char[]>, unsigned int> serialize() const override;
    void deserialize(const UniquePtr<char[]>& buffer) override;
};

#endif //SKIPPER_GNC_DATATYPES_H
