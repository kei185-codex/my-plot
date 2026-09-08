#pragma once
#include <queue>
#include <stop_token>
#include <vector>

#include "frame.hpp"

namespace parser
{
struct ParserBase
{
        virtual void parse(std::stop_token) = 0;
        virtual ~ParserBase()               = default;
};

template <typename T> struct Parser : public ParserBase
{
        frame::Type              type;
        std::queue<frame::Frame> inQueue;
        std::queue<T>            outQueue;

        Parser(frame::Type type, std::queue<frame::Frame>& inQueue, std::queue<T>& outQueue)
            : type(type), inQueue(inQueue), outQueue(outQueue)
        {}

        void                  parse(std::stop_token) override;
        static std::vector<T> parsePayload(frame::Frame* fr);
};

template <typename T> void Parser<T>::parse(std::stop_token st)
{
        while (1) {
                if (st.stop_requested())
                        return;

                if (this->inQueue.empty())
                        continue;

                frame::Frame fr = this->inQueue.front();

                this->inQueue.pop();
                this->outQueue.push_range(Parser<T>::parsePayload(fr));
        }
};

} // namespace parser
