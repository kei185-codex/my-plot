
#include <cstdint>
#include <vector>

#include "frame.hpp"
#include "parser.hpp"

namespace parser
{

template <>
std::vector<frame::systemMessage> Parser<frame::systemMessage>::parsePayload(frame::Frame& fr)
{
        return (std::vector<frame::systemMessage>){
                frame::systemMessage(reinterpret_cast<char*>(fr.payload.data()), fr.length)};
}

template <> std::vector<frame::LidarPoint> Parser<frame::LidarPoint>::parsePayload(frame::Frame& fr)
{
        std::vector<frame::LidarPoint> points      = {};
        uint8_t*                       payloadHead = fr.payload.data();

        for (int offset = 0; offset < fr.length; offset += frame::LIDAR_POINT_SIZE) {
                frame::LidarPoint* plp = (frame::LidarPoint*)(payloadHead + offset);

                points.push_back(*plp);
        }

        return points;
}

} // namespace parser
