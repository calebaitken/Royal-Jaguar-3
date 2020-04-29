//
// Created by caleb on 19/04/2020.
//

#ifndef ROYAL_JAGUAR_3_NETWORK_H
#define ROYAL_JAGUAR_3_NETWORK_H

#include <ws2tcpip.h>
#include <windows.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <thread>

#define CONNECTION_LIMIT 10
#define DNS_PORT 53

/**
 * thread safe queue. contains a mutex that is locked when pushing/popping.
 * cannot be copied.
 */
class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(const std::string& data);
    void pop(std::string& data);

private:
    std::list<std::string> queue;
    std::mutex mutex;
};

class ReadFunctor {
public:
    explicit ReadFunctor(const SOCKET& s);
    ~ReadFunctor() = default;
    ReadFunctor(const ReadFunctor&) = delete;
    ReadFunctor& operator=(const ReadFunctor&) = delete;
    void operator()();

    void set_socket(const SOCKET& s);
    SOCKET& get_socket();

    void pop(std::string& data);

private:
    ThreadSafeQueue queue;
    SOCKET socket;
};

class WriteFunctor {
public:
    explicit WriteFunctor(const SOCKET& s);
    ~WriteFunctor() = default;
    WriteFunctor(const WriteFunctor&) = delete;
    WriteFunctor operator=(const WriteFunctor&) = delete;
    void operator()();

    void set_socket(const SOCKET& s);
    SOCKET& get_socket();

    void push(const std::string& data);

private:
    ThreadSafeQueue queue;
    SOCKET socket;
};

class Network {
public:
    explicit Network();
    explicit Network(int port);
    ~Network();

    void update();

    // server functions
    bool open_ephemeral();
    bool close_ephemeral();
    bool accept_connections();
    bool remove_connection(int port);

    // client functions
    bool connect_to(std::string hostname, std::string port);
    bool disconnect();


    void write(std::string data, std::unique_ptr<std::string> buffer);
    void read(std::string data, std::unique_ptr<std::string> buffer);

    std::string get_localhost();

    SOCKET ephemeralPort;

private:
    std::string localIP;
    std::string ID;
    struct addrinfo* ephAddr;
    std::vector<int> connectedPorts;
    WSADATA wsaData;
};

#endif //ROYAL_JAGUAR_3_NETWORK_H
