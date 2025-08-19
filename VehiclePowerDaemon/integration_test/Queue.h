/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

template <typename T>
class Queue {
public:
    void push(T value)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(std::move(value));
        mCv.notify_all();
    }

    bool pop(T& value, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
    {
        std::unique_lock<std::mutex> lock(mMutex);
        while (mQueue.empty()) {
            auto status = mCv.wait_for(lock, timeout, [this] { return !mQueue.empty(); });
            if (!status) {
                return false;
            }
        }
        value = std::move(mQueue.front());
        mQueue.pop();
        return true;
    }

    bool wait(const T& value, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
    {
        T tmp;
        while (pop(tmp, timeout)) {
            if (value == tmp) {
                return true;
            }
        }
        return false;
    }

    bool empty()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

private:
    std::mutex mMutex;
    std::condition_variable mCv;
    std::queue<T> mQueue;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
