#pragma once
#include <queue>
#include <stop_token>
#include "frame.hpp"
#include "transmitter.hpp"

namespace protocol
{

void run(std::stop_token, transmitter::Transmitter&, std::queue<frame::systemMessage>&);

}
