#ifndef TANKS_THREAD_SAFE_QUEUE_H
#define TANKS_THREAD_SAFE_QUEUE_H
// Created by Adam Kecskes
// https://github.com/K-Adam/SafeQueue

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <class T>
class SafeQueue {
    std::queue<T> q;

    std::mutex mtx;
    std::condition_variable cv;

    std::condition_variable sync_wait;
    bool finish_processing = false;
    int sync_counter = 0;

    void DecreaseSyncCounter() {
        if (--sync_counter == 0) {
            sync_wait.notify_one();
        }
    }

public:
    typedef typename std::queue<T>::size_type size_type;

    SafeQueue() {
    }

    ~SafeQueue() {
        Finish();
    }

    void Produce(T &&item) {
        std::lock_guard<std::mutex> lock(mtx);

        q.push(std::move(item));
        cv.notify_one();
    }

    size_type Size() {
        std::lock_guard<std::mutex> lock(mtx);

        return q.size();
    }

    void Produce(const T &item) {
        T buff = item;
        Produce(std::move(buff));
    }

    [[nodiscard]] bool Consume(T &item) {
        try {
            std::lock_guard<std::mutex> lock(mtx);

            if (q.empty()) {
                return false;
            }

            item = std::move(q.front());
            q.pop();

            return true;
        } catch (std::system_error &e) {
            return false;
        }
    }

    [[nodiscard]] bool ConsumeSync(T &item) {
        std::unique_lock<std::mutex> lock(mtx);

        sync_counter++;

        cv.wait(lock, [&] { return !q.empty() || finish_processing; });

        if (q.empty()) {
            DecreaseSyncCounter();
            return false;
        }

        item = std::move(q.front());
        q.pop();

        DecreaseSyncCounter();
        return true;
    }

    void Finish() {
        std::unique_lock<std::mutex> lock(mtx);

        finish_processing = true;
        cv.notify_all();

        sync_wait.wait(lock, [&]() { return sync_counter == 0; });

        finish_processing = false;
    }
};
#endif  // TANKS_THREAD_SAFE_QUEUE_H
