#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace frame
{

extern const uint8_t START_OF_FRAME[];

enum class Command
{
        GET_STATUS,
        START_SCAN,
        END_SCAN,
};

enum class Type : uint8_t
{
        SYSTEM,
        LIDAR,
        // IMU,
        // ENCODER
};
extern const std::vector<Type> TYPES;

struct FrameHeader
{

        uint16_t length;
        uint8_t  crc;
        Type     type;
        uint32_t timestamp;
};
extern const size_t FRAME_HEADER_SIZE;

struct Frame
{
        uint16_t             length;
        Type                 type;
        std::vector<uint8_t> payload;
};

struct LidarPoint
{
        uint16_t dist;
        int16_t  angle;
};
extern const size_t LIDAR_POINT_SIZE;

using systemMessage = std::string;
} // namespace frame

namespace frame::command
{

extern const uint8_t GET_STATUS[];
extern const uint8_t START_SCAN[];
extern const uint8_t END_SCAN[];
} // namespace frame::command
