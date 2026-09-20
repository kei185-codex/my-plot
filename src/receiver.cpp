#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <print>
#include <stop_token>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <span>
#include <vector>

#include "receiver.hpp"
#include "error.hpp"
#include "frame.hpp"
#include "io.hpp"

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

Receiver::Receiver(io::Port& port, std::map<frame::Type, std::queue<frame::Frame>>& frameStreams)
    : port(port), frameStreams(frameStreams)
{}

void Receiver::run(std::stop_token st)
{
        frame::FrameHeader frh = {};
        frame::Frame       fr;
        while (1) {
                unwrap(this->findSOF(st, this->port));

                frh = unwrap(this->getFrameHeader(this->port));

                if (Receiver::isValidCRC(frh))
                        continue;

                fr = unwrap(this->getPayload(this->port, frh));

                this->frameStreams[fr.type].push(fr);
        }

        return;
}

std::expected<void, Error> Receiver::findSOF(std::stop_token& st, io::Port& port)
{
        std::array<uint8_t, 1> byte = {};

        while (1) {
                if (st.stop_requested())
                        return {};

                // TODO unwrapをつかう
                if (auto result = port.readRaw(byte); !result)
                        return std::unexpected<Error>(result.error());

                if (byte[0] != frame::START_OF_FRAME[0])
                        continue;

                if (auto result = port.readRaw(byte); !result)
                        return std::unexpected<Error>(result.error());

                if (byte[0] != frame::START_OF_FRAME[1])
                        continue;

                break;
        }

        return {};
};

std::expected<frame::FrameHeader, Error> Receiver::getFrameHeader(io::Port& port)
{
        std::array<uint8_t, 8> rawHeader = {};

        auto result = port.readRaw(rawHeader);
        if (!result)
                return std::unexpected<Error>(result.error());

        frame::FrameHeader frh = {
                .length = io::decodeBigEndian(std::span<const uint8_t, 2>(rawHeader.data(), 2)),
                .crc    = rawHeader[2],
                .type   = static_cast<frame::Type>(rawHeader[3]),
                .timestamp =
                        io::decodeBigEndian(std::span<const uint8_t, 4>(rawHeader.data() + 4, 4)),
        };

        return frh;
}

std::expected<frame::Frame, Error>
Receiver::getPayload(io::Port& port, const frame::FrameHeader& frh)
{
        frame::Frame fr;
        fr.length  = frh.length;
        fr.type    = frh.type;
        fr.payload = std::vector<uint8_t>(fr.length);

        auto result = port.readRaw(std::span<uint8_t>(fr.payload));
        if (!result.has_value())
                return std::unexpected<Error>(result.error());

        return fr;
}

// TODO
bool Receiver::isValidCRC(const frame::FrameHeader& fh) { return true; }

} // namespace receiver
