#pragma once

#include <cstddef>
#include <queue>
#include <stop_token>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <expected>

#include "frame.hpp"
#include "error.hpp"

using namespace error;

namespace receiver
{

struct Receiver
{
        std::string                                      file;
        std::fstream&                                    f;
        std::map<frame::Type, std::queue<frame::Frame>>& frameStreams;

        Receiver(
                const std::string,
                std::fstream&,
                std::map<frame::Type, std::queue<frame::Frame>>&);

        ~Receiver();

        void run(std::stop_token);

        static std::expected<void, Error>               findSOF(std::fstream&);
        static std::expected<frame::FrameHeader, Error> getFrameHeader(std::fstream&);
        // TODO 実装する
        static bool isValidCRC(const frame::FrameHeader&);
        // TODO 実装する
        static std::expected<frame::Frame, Error>
        getPayload(std::fstream&, const frame::FrameHeader&);
};
} // namespace receiver
