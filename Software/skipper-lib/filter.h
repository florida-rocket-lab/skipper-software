
#ifndef _FILTER_H
#define _FILTER_H

#include "datatypes.h"

class EKF // Extended Kalman Filter
{
public:
    EKF() {};
    SkipperState filter_data(const struct IMUData &imu_data);
private:
    // I know there are some internal variables that need to be stored here, hence the class.
};

#endif //_FILTER_H