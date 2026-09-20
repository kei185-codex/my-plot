#pragma once

#include <cstddef>
#include <expected>
#include <memory>
#include <queue>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <map>

#include "distributor.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "frame.hpp"
#include "io.hpp"
#include "receiver.hpp"
#include "transmitter.hpp"

using namespace error;

namespace manager
{

template <typename T> struct Worker
{
        std::unique_ptr<T> instance;
        std::jthread       thread;

        std::expected<void, Error> dispatch()
        {
                if (!this->instance)
                        return std::unexpected<Error>(Error::NODE_INIT_FAILED);

                this->thread = std::jthread([component = this->instance.get()](std::stop_token st) {
                        component->run(st);
                });

                return {};
        }

        std::expected<void, Error> abort()
        {
                if (!this->instance)
                        return std::unexpected<Error>(Error::NODE_INIT_FAILED);

                this->thread.request_stop();

                return {};
        }
};

using ReceiverWorker    = Worker<receiver::Receiver>;
using ParserWorker      = Worker<parser::ParserBase>;
using DistributorWorker = Worker<distributor::Distributor>;

struct DataStreams
{
        std::unique_ptr<std::queue<frame::systemMessage>> system;
        std::unique_ptr<std::queue<frame::LidarPoint>>    lidar;
        DataStreams();
};

struct Manager
{
        io::Port port;

        std::unique_ptr<std::map<frame::Type, std::queue<frame::Frame>>> frameStreams;
        DataStreams                                                      dataStreams;

        std::unique_ptr<transmitter::Transmitter> transmitter;
        ReceiverWorker                            receiverWorker;
        std::map<frame::Type, ParserWorker>       parsers;
        std::map<frame::Type, DistributorWorker>  distributors;

        Manager(const std::string);
        ~Manager();
        std::expected<void, Error> run();

        static std::expected<void, Error> initParsers(
                std::map<frame::Type, ParserWorker>&,
                std::map<frame::Type, std::queue<frame::Frame>>&,
                DataStreams&);

        static std::expected<void, Error> initDistributors(
                std::map<frame::Type, DistributorWorker>&,
                DataStreams&,
                transmitter::Transmitter&);
};

} // namespace manager
