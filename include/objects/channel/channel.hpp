#pragma once

#include <common/console.hpp>
#include <common/object.hpp>
#include <common/variant.hpp>

#include <utilities/threads/threads.hpp>

#include <queue>

namespace love
{

    class Channel : public Object
    {
      public:
        static Type type;

        Channel() : sent(0), received(0)
        {}

        virtual ~Channel()
        {}

        uint64_t Push(const Variant& variant);

        bool Supply(const Variant& variant);

        bool Supply(const Variant& variant, double timeout);

        bool Pop(Variant* variant);

        bool Demand(Variant* variant);

        bool Demand(Variant* variant, double timeout);

        bool Peek(Variant* variant);

        bool HasRead(uint64_t id);

        int GetCount();

        void Clear();

        void LockMutex();

        void UnlockMutex();

      protected:
        uint64_t sent;
        uint64_t received;

        love::mutex mutex;
        love::conditional condition;

        std::queue<Variant> queue;
    };
} // namespace love
