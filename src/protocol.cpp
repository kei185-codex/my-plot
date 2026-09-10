

#include <queue>
#include <stop_token>
#include "frame.hpp"
#include "transmitter.hpp"
#include "protocol.hpp"

namespace protocol
{

void run(
        std::stop_token                   st,
        transmitter::Transmitter&         transmitter,
        std::queue<frame::systemMessage>& mQueue)
{
        if (transmitter.request(st, frame::OperationType::GET_STATUS, mQueue))
                std::exit(1);

        if (transmitter.request(st, frame::OperationType::START_SCAN, mQueue))
                std::exit(1);
};

} // namespace protocol
