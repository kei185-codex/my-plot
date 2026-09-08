#pragma once

#include <cstddef>
#include <expected>
#include <queue>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

#include "error.hpp"
#include "frame.hpp"

using namespace error;

namespace receiver
{

struct Receiver
{
        std::string              file;
        std::ifstream            f;
        std::queue<frame::Frame> outQueue;

        Receiver(const std::string file, std::queue<frame::Frame>& outQueue);
        ~Receiver();

        std::expected<void, Error> receive();
        void                       findSOF(std::ifstream&);
        frame::FrameHeader         getFrameHeader(std::ifstream&);

      private:
        // TODO 実装する
        static bool isValidCRC(const frame::FrameHeader&);
};
} // namespace receiver
