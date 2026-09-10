#pragma once
#include "frame.hpp"
#include <fstream>
#include <queue>
#include <stop_token>

namespace transmitter
{

struct Transmitter
{
        std::fstream& f;

      public:
        Transmitter(std::fstream&);

        bool transmit(frame::OperationType);

        bool
        request(std::stop_token                   st,
                frame::OperationType              type,
                std::queue<frame::systemMessage>& mQueue);
};

// bool request(
//         std::stop_token,
//         Transmitter&,
//         frame::OperationType,
//         std::queue<frame::systemMessage>&);

} // namespace transmitter
