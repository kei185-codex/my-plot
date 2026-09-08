#include <cstddef>
#include <expected>
#include <memory>
#include <queue>
#include <stop_token>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <map>

#include "distributor.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "frame.hpp"
#include "receiver.hpp"
#include "manager.hpp"

using namespace error;

namespace manager
{
Streams::Streams()
    : frame(std::make_unique<std::queue<frame::Frame>>()),
      system(std::make_unique<std::queue<frame::systemMessage>>()),
      lidar(std::make_unique<std::queue<frame::LidarPoint>>())
{}

Manager::Manager(const std::string file)
    : streams(Streams()), parsers(std::map<frame::Type, ParserInfo>()),
      distributors(std::map<frame::Type, DistributorInfo>())
{

        this->pReceiver = std::make_unique<receiver::Receiver>(file, *this->streams.frame.get());

        if (!Manager::initNode(this->parsers, this->distributors, this->streams))
                std::exit(1);
}

Manager::~Manager()
{
        for (auto& [_, parser] : this->parsers)
                parser.thread.request_stop();

        for (auto& [_, parser] : this->distributors)
                parser.thread.request_stop();
}

template <typename T, typename U>
void Manager::initParser(ParserInfo&    parserInfo,
                         frame::Type    type,
                         std::queue<T>& inQueue,
                         std::queue<U>& outQueue)
{
        parserInfo.pParser = std::make_unique<parser::Parser<U>>(type, inQueue, outQueue);
        parserInfo.thread  = std::jthread(
                [parser = parserInfo.pParser.get()](std::stop_token st) { parser->parse(st); });
}

template <typename T>
void Manager::initDistributor(DistributorInfo& distributorInfo,
                              frame::Type      type,
                              std::queue<T>&   inQueue)
{
        distributorInfo.pDistributor = std::make_unique<distributor::Distributor<T>>(type, inQueue);
        distributorInfo.thread =
                std::jthread([distributor = distributorInfo.pDistributor.get()](
                                     std::stop_token st) { distributor->distribute(st); });
}

std::expected<void, Error> Manager::initNode(std::map<frame::Type, ParserInfo>&      parsers,
                                             std::map<frame::Type, DistributorInfo>& distributors,
                                             Streams&                                streams)
{
        for (auto type : frame::TYPES) {
                auto [pit, pSuccess] = parsers.try_emplace(type);
                auto [dit, dSuccess] = distributors.try_emplace(type);

                if (!(pSuccess && dSuccess))
                        return std::unexpected<Error>(Error::NODE_INIT_FAILED);

                auto& parserInfo      = pit->second;
                auto& distributorInfo = dit->second;
                switch (type) {
                        case frame::Type::SYSTEM:
                                Manager::initParser(parserInfo,
                                                    type,
                                                    *streams.frame.get(),
                                                    *streams.system.get());

                                Manager::initDistributor(distributorInfo,
                                                         type,
                                                         *streams.system.get());
                                break;
                        case frame::Type::LIDAR:
                                Manager::initParser(parserInfo,
                                                    type,
                                                    *streams.frame.get(),
                                                    *streams.lidar.get());
                                Manager::initDistributor(distributorInfo,
                                                         type,
                                                         *streams.lidar.get());

                                break;
                        default:
                                break;
                }
        }

        return {};
}

} // namespace manager
