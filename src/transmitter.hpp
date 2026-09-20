#pragma once
#include "frame.hpp"
#include "io.hpp"
#include <expected>
#include <queue>
#include <stop_token>

namespace transmitter
{

struct Transmitter
{
        io::Port& port;

      public:
        Transmitter(io::Port&);

        std::expected<void, error::Error> transmit(frame::OperationType);

        std::expected<void, error::Error>
        request(std::stop_token                   st,
                frame::OperationType              type,
                std::queue<frame::systemMessage>& mQueue);
};

} // namespace transmitter
