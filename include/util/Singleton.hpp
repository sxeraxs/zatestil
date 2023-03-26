#pragma once

#include <atomic>
#include <mutex>
namespace ztsl {
namespace util {

    template <typename T>
    class Singleton {
       public:
        using Ptr = T*;

       protected:
        Singleton() = default;

       public:
        ~Singleton() = default;
        Singleton(Singleton const&) = delete;
        Singleton(Singleton&&) noexcept = default;
        Singleton& operator=(Singleton const&) = delete;
        Singleton& operator=(Singleton&&) noexcept = default;

       public:
        template <typename... Args>
        static Ptr instance(Args... args) noexcept {
            static std::mutex mutex;
            static std::atomic<T*> instance;

            auto isLoaded = instance.load() != nullptr;
            if (not isLoaded) {
                std::lock_guard _ {mutex};

                isLoaded = instance.load() != nullptr;
                if (not isLoaded) {
                    instance = new T(args...);
                }
            }

            return instance;
        }
    };
}// namespace util
}// namespace ztsl
