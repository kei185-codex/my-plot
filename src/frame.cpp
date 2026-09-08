#include <cstddef>
#include <cstdint>
#include <vector>
#include "frame.hpp"

namespace frame
{

const size_t            FRAME_HEADER_SIZE = sizeof(FrameHeader);
const size_t            LIDAR_POINT_SIZE  = sizeof(LidarPoint);
const uint8_t           START_OF_FRAME[]  = {0xAA, 0x55};
const std::vector<Type> TYPES             = {
        Type::SYSTEM,
        Type::LIDAR,
        // Type::IMU,
        // Type::ENCODER
};
} // namespace frame

namespace frame::command
{

const uint8_t GET_STATUS[] = {0xAA, 0xA1};
const uint8_t START_SCAN[] = {0xAA, 0xA2};
const uint8_t END_SCAN[]   = {0xAA, 0xA3};
} // namespace frame::command
