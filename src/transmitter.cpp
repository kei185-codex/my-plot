#include "transmitter.hpp"
#include "frame.hpp"
#include <fstream>
#include <print>
#include <queue>
#include <stop_token>
#include <thread>

namespace transmitter
{

Transmitter::Transmitter(std::fstream& f) : f(f) {}

bool Transmitter::transmit(frame::OperationType type)
{
        const auto& command = frame::TX.at(type).command;
        this->f.write(reinterpret_cast<const char*>(command.data()), command.size());
        return this->f.good();
}

bool Transmitter::request(
        std::stop_token                   st,
        frame::OperationType              type,
        std::queue<frame::systemMessage>& mQueue)
{
        if (!this->transmit(type))
                return false;

        std::println("{}: TRANSMIT {}", std::this_thread::get_id(), frame::toString(type));

        while (1) {
                if (st.stop_requested())
                        return true;

                if (mQueue.empty())
                        continue;

                auto res = mQueue.front();
                std::println("{}: RECEIVE {}", std::this_thread::get_id(), res);
                return true;
        }
};

} // namespace transmitter
