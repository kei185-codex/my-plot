#include <cstddef>
#include <cstdint>
#include <expected>
#include <print>
#include <stop_token>
#include <thread>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "receiver.hpp"
#include "error.hpp"
#include "frame.hpp"

using namespace error;

namespace receiver
{

inline void abortByError(Error err)
{
        std::println("{}: {}", std::this_thread::get_id(), toString(err));
        std::exit(1);
}

template <typename T> T unwrap(const std::expected<T, Error>&& result)
{
        if (!result.has_value())
                abortByError(result.error());

        return result.value();
}

Receiver::Receiver(
        const std::string                                file,
        std::fstream&                                    f,
        std::map<frame::Type, std::queue<frame::Frame>>& frameStreams)
    : file(file), f(f), frameStreams(frameStreams)
{}

Receiver::~Receiver() { this->f.close(); }

void Receiver::run(std::stop_token st)
{
        this->f.open(this->file, std::ios::binary | std::ios::in);

        if (!(this->f.is_open() && f.good()))
                return exit(1);

        frame::FrameHeader frh;
        frame::Frame       fr;
        Error              err;
        while (1) {
                if (st.stop_requested())
                        return;

                unwrap(this->findSOF(this->f));

                frh = unwrap(this->getFrameHeader(this->f));

                if (Receiver::isValidCRC(frh))
                        continue;

                fr = unwrap(this->getPayload(this->f, frh));

                this->frameStreams[fr.type].push(fr);
        }

        return;
}

std::expected<void, Error> Receiver::findSOF(std::fstream& f)
{
        uint8_t firstByte = -1;

        while (1) {

                if (!f.good())
                        return std::unexpected<Error>(Error::FILE_INTERNAL_ERROR);

                if ((firstByte = f.get()) != frame::START_OF_FRAME[0])
                        continue;

                if (f.get() != frame::START_OF_FRAME[1])
                        continue;

                break;
        }

        return {};
};

std::expected<frame::FrameHeader, Error> Receiver::getFrameHeader(std::fstream& f)
{
        frame::FrameHeader frh = {};
        f.read((char*)&frh, frame::FRAME_HEADER_SIZE);

        if (!f.good())
                return std::unexpected<Error>(Error::FILE_INTERNAL_ERROR);

        return frh;
}

std::expected<frame::Frame, Error>
Receiver::getPayload(std::fstream& f, const frame::FrameHeader& frh)
{
        frame::Frame fr;
        fr.length  = frh.length;
        fr.type    = frh.type;
        fr.payload = std::vector<uint8_t>();

        f.read((char*)fr.payload.data(), fr.length);

        if (!f.good())
                return std::unexpected<Error>(Error::FILE_INTERNAL_ERROR);

        return fr;
}

// TODO
bool Receiver::isValidCRC(const frame::FrameHeader& fh) { return true; }

} // namespace receiver
