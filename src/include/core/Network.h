/**
 * @author Caleb Aitken
 * @date 19/04/2020
 *
 * Windows multithreaded TCP/TP socket networking header
 *
 * @class ReadFunctor Functor object for reading from TCP/IP socket in new thread
 * @class WriteFunctor Functor object for writing to TCP/IP socket in new thread
 * @class Network TCP/IP Windows socket controller for client or server
 */

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
#include <thread>
#include <map>

#include "core/Memory.h"

#define CONNECTION_LIMIT 10
#define DNS_PORT 53
#define SOCKET_BUFFER_SIZE 1024

// forward declarations
class ReadFunctor;
class WriteFunctor;
class AcceptConnectionFunctor;
class Network;

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

    SOCKET get_socket();

    void pop(std::string& data);

private:
    ThreadSafeQueue<std::string> queue;
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

    SOCKET get_socket();

    void push(const std::string& data);

private:
    ThreadSafeQueue<std::string> queue;
    SOCKET socket;
};

class AcceptConnectionFunctor {
public:
    explicit AcceptConnectionFunctor(const SOCKET& ephSock = -1);
    ~AcceptConnectionFunctor() = default;

    // non-copyable
    AcceptConnectionFunctor(const AcceptConnectionFunctor&) = delete;
    AcceptConnectionFunctor operator=(const AcceptConnectionFunctor&) = delete;

    // movable
    AcceptConnectionFunctor(AcceptConnectionFunctor&&) = default;
    AcceptConnectionFunctor& operator=(AcceptConnectionFunctor&&) = default;

    void operator()();

    SOCKET get_socket();

    void pop(SOCKET& s);

private:
    ThreadSafeQueue<SOCKET> queue;
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
    std::string get_eph_port();
    std::vector<SOCKET> get_connected_ports();
    std::string get_IP_of_connected_port(const SOCKET& socket);

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
    AcceptConnectionFunctor ephAcceptFunctor;
    std::thread ephAcceptThread;
    struct addrinfo* ephAddr;
};

#endif //ROYAL_JAGUAR_3_NETWORK_H
