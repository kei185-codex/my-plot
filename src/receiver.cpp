#include <cstddef>
#include <expected>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

#include "receiver.hpp"
#include "error.hpp"
#include "frame.hpp"

using namespace error;

namespace receiver
{

Receiver::Receiver(const std::string file, std::queue<frame::Frame> outQueue)
    : file(file), f(std::ifstream()), outQueue(outQueue)
{}

Receiver::~Receiver() { this->f.close(); }

std::expected<void, Error> Receiver::receive()
{

        this->f.open(this->file, std::ios::binary | std::ios::in);

        if (!this->f.is_open())
                return std::unexpected<Error>(Error::OPEN_FILE_FAILED);

        frame::FrameHeader fh;
        while (1) {
                this->findSOF(this->f);
                frame::FrameHeader fh = this->getFrameHeader(this->f);

                if (Receiver::isValidCRC(fh))
                        continue;

                // TODO frame header をqueueuにおくる
        }

        return {};
}

void Receiver::findSOF(std::ifstream& f)
{
        uint8_t firstByte = -1;

        while (1) {
                if ((firstByte = f.get()) != frame::START_OF_FRAME[0])
                        continue;

                if (f.get() != frame::START_OF_FRAME[1])
                        continue;

                break;
        }

        return;
};

frame::FrameHeader Receiver::getFrameHeader(std::ifstream& f)
{
        frame::FrameHeader fr = {};
        f.read((char*)&fr, frame::FRAME_HEADER_SIZE);
        return fr;
}

} // namespace receiver
