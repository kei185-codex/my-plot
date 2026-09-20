

#include <print>
#include <queue>
#include <stop_token>
#include <string>
#include <iostream>
#include <thread>
#include "frame.hpp"
#include "transmitter.hpp"
#include "protocol.hpp"

namespace protocol
{

static void waitforReady(std::stop_token& st, std::queue<frame::systemMessage>& mQueue)
{
        std::string sm;
        do {
                if (st.stop_requested())
                        return;

                if (mQueue.empty())
                        continue;

                sm = mQueue.front();

                std::println("{}: {}", std::this_thread::get_id(), sm);

                mQueue.pop();
                // TODO type code を確認するようにする
        } while (sm != frame::TX.at(frame::OperationType::WAIT_READY).ack);
}

void run(
        std::stop_token                   st,
        transmitter::Transmitter&         transmitter,
        std::queue<frame::systemMessage>& mQueue)
{
        waitforReady(st, mQueue);
        if (st.stop_requested())
                return;

        std::println("{}: {}", std::this_thread::get_id(), "press ENTER to start scan");
        std::string s;
        std::getline(std::cin, s);

        std::println("{}: {}", std::this_thread::get_id(), "SCAN STARTED");
        std::println("{}: {}", std::this_thread::get_id(), "SCAN STARTEDってこと!?");

        if (auto result = transmitter.request(st, frame::OperationType::START_SCAN, mQueue);
            !result)
                std::exit(1);
};

} // namespace protocol
