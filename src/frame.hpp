#pragma once
#include <map>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace frame
{

extern const int     OPERATION_TIMEOUT;
extern const uint8_t START_OF_FRAME[];

enum class OperationType
{
        GET_STATUS,
        START_SCAN,
        END_SCAN,
};

constexpr std::string_view toString(OperationType type)
{
        switch (type) {
                case OperationType::GET_STATUS:
                        return "GET_STATUS";
                case OperationType::START_SCAN:
                        return "START_SCAN";
                case OperationType::END_SCAN:
                        return "END_SCAN";
                default:
                        return "UNKNOWN";
        }
}

// TODO
enum class Type : uint8_t
{
        SYSTEM,
        LIDAR,
        // IMU,
        // ENCODER
};

constexpr std::string_view toString(Type type)
{
        switch (type) {
                case Type::SYSTEM:
                        return "SYSTEM";
                case Type::LIDAR:
                        return "LIDAR";
                default:
                        return "UNKNOWN";
        }
}

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

struct Operation
{
        std::array<uint8_t, 2> command;
        std::string            ack;

        Operation(const std::array<uint8_t, 2>& command, const std::string& ack);
};

extern const std::map<OperationType, Operation> TX;

} // namespace frame
