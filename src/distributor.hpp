#pragma once
#include "frame.hpp"
#include "transmitter.hpp"
#include <queue>
#include <stop_token>

namespace distributor
{

struct Distributor
{
        virtual void run(std::stop_token) = 0;
        virtual ~Distributor()            = default;
};

struct DeviceController : Distributor
{
        frame::Type                       type;
        transmitter::Transmitter&         transmitter;
        std::queue<frame::systemMessage>& inQueue;

        void run(std::stop_token) override;

        DeviceController(frame::Type, transmitter::Transmitter&, std::queue<frame::systemMessage>&);
};

template <typename T> struct Plotter : Distributor
{
        frame::Type    type;
        std::queue<T>& inQueue;

        Plotter(frame::Type type, std::queue<T>& inQueue) : type(type), inQueue(inQueue) {}
        ~Plotter() {}

        void        run(std::stop_token) override;
        static void distributeStuff(T);
};

template <typename T> void Plotter<T>::run(std::stop_token st)
{
        while (1) {
                if (st.stop_requested())
                        return;

                if (this->inQueue.empty())
                        continue;

                Plotter<T>::distributeStuff(this->inQueue.front());

                this->inQueue.pop();
        }
}

} // namespace distributor
