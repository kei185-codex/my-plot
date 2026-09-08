
#include "distributor.hpp"
#include "frame.hpp"
#include <print>

namespace distributor
{

template <> void Distributor<frame::systemMessage>::distributeStuff(frame::systemMessage m)
{
        std::println("{}", m);
}

template <> void Distributor<frame::LidarPoint>::distributeStuff(frame::LidarPoint m)
{
        std::println("{}, {}", m.dist, m.angle);
}

} // namespace distributor
