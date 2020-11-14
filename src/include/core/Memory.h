/**
 * @author Caleb Aitken
 * @date 16/06/2020
 *
 * Memory management
 *
 * @class ThreadSafeQueue Template implementation of a cross-thread queue
 */

#ifndef ROYAL_JAGUAR_3_MEMORY_H
#define ROYAL_JAGUAR_3_MEMORY_H

#include <mutex>
#include <queue>
#include <cassert>
#include <sstream>

// forward declaration
template <typename T> class ThreadSafeQueue;

/**
 * thread safe queue. contains a mutex that is locked when pushing/popping.
 * cannot be copied but can be moved.
 */
template <typename T>
class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    // non-copyable
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // moveable
    ThreadSafeQueue(ThreadSafeQueue&& other) noexcept {
        std::unique_lock<std::mutex> lock(other.mutex);
        this->queue = std::move(other.queue);
    };

    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) noexcept {
        if (this != &other) {
            std::unique_lock<std::mutex> lockThis(this->mutex, std::defer_lock);
            std::unique_lock<std::mutex> lockOther(other.mutex, std::defer_lock);
            std::lock(lockThis, lockOther);
            this->queue = std::move(other.queue);
        }

        return *this;
    };

    /**
     * thread safe push string to back of queue.
     *
     * @param data  reference to string to queue
     */
    void push(const T& data) {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->queue.push(data);
    }

    /**
     * thread safe pop string from front of queue.
     *
     * @param data  reference to string to load data into
     */
    void pop(T &data) {
        assert(std::is_copy_assignable<T>::value);
        std::lock_guard<std::mutex> lock(this->mutex);
        if (this->queue.empty()) {
            memset(&data, 0, sizeof(data));
        } else {
            data = std::move(this->queue.front());
            this->queue.pop();
        }
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
};

// FIXME: i hate this. i hate it. i hate it so much. why can't i just specialise a single method reeee
template <>
class ThreadSafeQueue<std::stringstream> {
public:
    explicit ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    // non-copyable
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // moveable
    ThreadSafeQueue(ThreadSafeQueue&& other) noexcept {
        std::unique_lock<std::mutex> lock(other.mutex);
        this->queue = std::move(other.queue);
    };

    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) noexcept {
        if (this != &other) {
            std::unique_lock<std::mutex> lockThis(this->mutex, std::defer_lock);
            std::unique_lock<std::mutex> lockOther(other.mutex, std::defer_lock);
            std::lock(lockThis, lockOther);
            this->queue = std::move(other.queue);
        }

        return *this;
    };

    void push(const std::stringstream& data) {
        std::lock_guard<std::mutex> lock(this->mutex);
        std::stringstream buffer(std::ios::app | std::ios::out | std::ios::in | std::ios::binary);
        buffer.str(data.str());
        this->queue.push(std::move(buffer));
    }

    void pop(std::stringstream &data) {
        std::lock_guard<std::mutex> lock(this->mutex);
        if (this->queue.empty()) {
            data.clear();
        } else {
            data = std::move(this->queue.front());
            this->queue.pop();
        }
    }

private:
    std::queue<std::stringstream> queue;
    std::mutex mutex;
};

#endif //ROYAL_JAGUAR_3_MEMORY_H

