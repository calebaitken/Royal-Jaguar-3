//
// Created by caleb on 19/04/2020.
//

#ifndef ROYAL_JAGUAR_3_NETWORK_H
#define ROYAL_JAGUAR_3_NETWORK_H

#include <ws2tcpip.h>
#include <windows.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <map>

#define CONNECTION_LIMIT 10
#define DNS_PORT 53
#define SOCKET_BUFFER_SIZE 1024

/**
 * thread safe queue. contains a mutex that is locked when pushing/popping.
 * cannot be copied but can be moved.
 */
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

    void push(const std::string& data);
    void pop(std::string& data);

    void wait_for_push();
    void wait_for_pop();

private:
    std::list<std::string> queue;
    std::mutex mutex;
    std::condition_variable pushCond;
    std::condition_variable popCond;
};

class ReadFunctor {
public:
    explicit ReadFunctor(const SOCKET& s);
    ~ReadFunctor() = default;

    // non-copyable
    ReadFunctor(const ReadFunctor&) = delete;
    ReadFunctor operator=(const ReadFunctor&) = delete;

    // moveable
    ReadFunctor(ReadFunctor&&) = default;
    ReadFunctor& operator=(ReadFunctor&&) = default;

    void operator()();

    void set_socket(const SOCKET& s);
    SOCKET get_socket();

    void pop(std::string& data);

private:
    ThreadSafeQueue queue;
    SOCKET socket;
};

class WriteFunctor {
public:
    explicit WriteFunctor(const SOCKET& s);
    ~WriteFunctor() = default;

    // non-copyable
    WriteFunctor(const WriteFunctor&) = delete;
    WriteFunctor operator=(const WriteFunctor&) = delete;

    // moveable
    WriteFunctor(WriteFunctor&&) = default;
    WriteFunctor& operator=(WriteFunctor&&) = default;

    void operator()();

    void set_socket(const SOCKET& s);
    SOCKET get_socket();

    void push(const std::string& data);

private:
    ThreadSafeQueue queue;
    SOCKET socket;
};

class Network {
public:
    explicit Network();
    //explicit Network(int port);
    ~Network();

    // server functions
    bool open_ephemeral();
    bool close_ephemeral();
    bool accept_connections();
    bool close_socket(SOCKET socket);

    // client functions
    bool connect_to(std::string hostname, std::string port);
    bool disconnect();


    void write(const SOCKET& s, const std::string& data);
    void read(const SOCKET& s, std::string& data);

    std::string get_localhost();
    std::vector<SOCKET> get_connected_ports();

private:
    // winsock data
    WSADATA wsaData;

    std::vector<SOCKET> connectedPorts;
    std::map<SOCKET, std::pair<ReadFunctor, WriteFunctor>> IOFunctors;
    std::map<SOCKET, std::pair<std::thread, std::thread>> IOThreads;
    std::string localIP;

    // ephemeral port
    bool ephOpen = false;
    SOCKET ephemeralPort;
    struct addrinfo* ephAddr;
};

#endif //ROYAL_JAGUAR_3_NETWORK_H
