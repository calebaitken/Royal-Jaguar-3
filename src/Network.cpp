//
// Created by caleb on 19/04/2020.
//

#include "Network.h"

/**
 * thread safe push string to back of queue.
 *
 * @param data  reference to string to queue
 */
void ThreadSafeQueue::push(const std::string &data) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->queue.push_back(data);
}

/**
 * thread safe pop string from front of queue.
 *
 * @param data  reference to string to load data into
 */
void ThreadSafeQueue::pop(std::string& data){
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->queue.empty()) {
        data.clear();
    } else {
        data.assign(this->queue.front());
        this->queue.pop_front();
    }
}

ReadFunctor::ReadFunctor(const SOCKET& s) {
    this->socket = s;
}

void ReadFunctor::set_socket(const SOCKET &s) {
    this->socket = s;
}

SOCKET& ReadFunctor::get_socket() {
    return this->socket;
}

void ReadFunctor::pop(std::string &data) {
    this->queue.pop(data);
}

void ReadFunctor::operator()() {
    char buffer[2014];

    recv(this->socket, buffer, sizeof(buffer), 0);
    this->queue.push(buffer);
}

void WriteFunctor::push(const std::string &data) {
    this->queue.push(data);
}

Network::Network() {
    int status;

    // start winsock (must be ended for each start)
    if ((status = WSAStartup(MAKEWORD(2, 0), &this->wsaData)) != 0) {
        std::cerr << "WSAStartup() failed." << std::endl;
    }

    // get LAN address
    this->localIP = this->get_localhost();
    std::cout << this->localIP << std::endl;
}

Network::~Network() {
    this->close_ephemeral();
    WSACleanup();
}

/**
 * Gets local network IPv4 address.
 * This is achieved by opening a socket to the loopback and then asking for the socket name.
 *
 * @return  a std::string containing the local IP address.
 */
std::string Network::get_localhost() {
    int status;
    SOCKET sockfd;
    sockaddr_in loopback;

    memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = INADDR_LOOPBACK;
    loopback.sin_port = htons(DNS_PORT);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror(sockfd) << std::endl;
        return NULL;
    }

    if ((status = connect(sockfd, (sockaddr*) &loopback, sizeof(loopback))) < 0) {
        std::cerr << "connect() failed with err: " << gai_strerror(status) << std::endl;
        return NULL;
    }

    socklen_t addrlen = sizeof(loopback);
    if ((status = getsockname(sockfd, (sockaddr*) &loopback, &addrlen)) < 0) {
        std::cerr << "getsockname() failed with err: " << gai_strerror(status) << std::endl;
        return NULL;
    }

    shutdown(sockfd, SD_BOTH);
    closesocket(sockfd);

    //std::cout << inet_ntoa(loopback.sin_addr) << std::endl;

    return inet_ntoa(loopback.sin_addr);
}

bool Network::open_ephemeral() {
    int status;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    if ((status = getaddrinfo(this->localIP.c_str(), "0", &hints, &this->ephAddr)) != 0) {
        std::cerr << "getaddrinfo() failed with err: " << gai_strerror(status) << std::endl;
        return false;
    }

    if ((this->ephemeralPort = socket(this->ephAddr->ai_family, this->ephAddr->ai_socktype, this->ephAddr->ai_protocol)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror(this->ephemeralPort) << std::endl;
        return false;
    }

    if ((status = bind(this->ephemeralPort, this->ephAddr->ai_addr, this->ephAddr->ai_addrlen)) != 0) {
        std::cerr << "bind() failed with err: " << gai_strerror(status) << std::endl;
        return false;
    }

    if ((status = listen(this->ephemeralPort, CONNECTION_LIMIT)) != 0) {
        std::cerr << "listen() failed with err: " << gai_strerror(status) << std::endl;
        return false;
    }

    if ((status = getsockname(this->ephemeralPort, this->ephAddr->ai_addr, (int*) &this->ephAddr->ai_addrlen)) != 0) {
        std::cerr << "getsockname() failed with err: " << gai_strerror(status) << std::endl;
        return false;
    }

    //unsigned long mode = 0;
    //if ((status = ioctlsocket(this->ephemeralPort, FIONBIO, &mode)) < 0) {
    //    std::cerr << "ioctlsocket() failed with err: " << gai_strerror(status) << std::endl;
    //    return false;
    //}

    std::cout << ntohs(((struct sockaddr_in*) this->ephAddr->ai_addr)->sin_port) << std::endl;

    // TODO: convert the sock file descriptor into a read/write stream
    // TODO: hold onto the sockfd reference so that it can be closed on destruction
    // TODO: OR ignore the above todo's and just create a warpper for the send() & recv() funcs
    // TODO: OR use non-blocking IO. not sure what it does?  RESEARCH IT!
    //this->ephemeralPort = std::fstream(fdopen(sockfd, "rw"));

    return true;
}

bool Network::close_ephemeral() {
    int status;

    if ((status = closesocket(this->ephemeralPort)) != 0) {
        std::cerr << "shutdown() failed with err: " << status << std::endl;
        return false;
    } else {
        freeaddrinfo(this->ephAddr);
    }

    return true;
}

bool Network::accept_connections() {
    int status, newfd;
    sockaddr_storage newAddr;

    socklen_t newAddrLen = sizeof(newAddr);
    if ((newfd = accept(this->ephemeralPort, (sockaddr*) &newAddr, &newAddrLen)) != -1) {
        this->connectedPorts.emplace_back(newfd);
        return true;
    }

    std::cerr << "WARNING: accept() failed to find an incoming connection either because one does not exist or their was an error." << std::endl;
    return false;
}