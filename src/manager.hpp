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
#include "receiver.hpp"

namespace manager
{

struct ParserInfo
{
        std::unique_ptr<parser::ParserBase> pParser;
        std::jthread                        thread;
};

struct DistributorInfo
{
        std::unique_ptr<distributor::DistributorBase> pDistributor;
        std::jthread                                  thread;
};

struct Streams
{
        std::unique_ptr<std::queue<frame::Frame>>         frame;
        std::unique_ptr<std::queue<frame::systemMessage>> system;
        std::unique_ptr<std::queue<frame::LidarPoint>>    lidar;
        Streams();
};

struct Manager
{
        Streams                                streams;
        std::unique_ptr<receiver::Receiver>    pReceiver;
        std::map<frame::Type, ParserInfo>      parsers;
        std::map<frame::Type, DistributorInfo> distributors;

        Manager(const std::string);
        ~Manager();

        template <typename T, typename U>
        static void initParser(ParserInfo&    parserInfo,
                               frame::Type    type,
                               std::queue<T>& inQueue,
                               std::queue<U>& outQueue);

        template <typename T>
        static void
        initDistributor(DistributorInfo& distributorInfo, frame::Type type, std::queue<T>& inQueue);

        static std::expected<void, Error> initNode(std::map<frame::Type, ParserInfo>&,
                                                   std::map<frame::Type, DistributorInfo>&,
                                                   Streams& streams);
};

} // namespace manager
