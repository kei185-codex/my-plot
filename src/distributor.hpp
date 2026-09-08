#pragma once
#include "frame.hpp"
#include <queue>
#include <stop_token>

namespace distributor
{

struct DistributorBase
{
        virtual void distribute(std::stop_token) = 0;
        virtual ~DistributorBase()               = default;
};

template <typename T> struct Distributor : DistributorBase
{
        frame::Type   type;
        std::queue<T> inQueue;

        Distributor(frame::Type type, std::queue<T>& inQueue) : type(type), inQueue(inQueue) {}
        ~Distributor() {}

        void        distribute(std::stop_token) override;
        static void distributeStuff(T);
};

template <typename T> void Distributor<T>::distribute(std::stop_token st)
{
        while (1) {
                if (st.stop_requested())
                        return;

                if (this->inQueue.empty())
                        continue;

                Distributor<T>::distributeStuff(this->inQueue.front());

                this->inQueue.pop();
        }
}

} // namespace distributor
