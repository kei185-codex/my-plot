
#include "protocol.hpp"
#include "distributor.hpp"
#include "frame.hpp"
#include <print>
#include <stop_token>

namespace distributor
{

DeviceController::DeviceController(
        frame::Type                       type,
        transmitter::Transmitter&         transmiter,
        std::queue<frame::systemMessage>& inQueue)
    : type(type), transmitter(transmiter), inQueue(inQueue) {};

void DeviceController::run(std::stop_token st)
{
        protocol::run(st, this->transmitter, this->inQueue);
}

// template <> void Plotter<frame::systemMessage>::distributeStuff(frame::systemMessage m)
// {
//         std::println("{}", m);
// }

template <> void Plotter<frame::LidarPoint>::distributeStuff(frame::LidarPoint m)
{
        std::println("{}, {}", m.dist, m.angle);
}

} // namespace distributor
