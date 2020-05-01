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
    this->pushCond.notify_all();
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
        this->popCond.notify_all();
    }
}

void ThreadSafeQueue::wait_for_pop() {
    std::unique_lock<std::mutex> lock(this->mutex);
    popCond.wait(lock);
}

void ThreadSafeQueue::wait_for_push() {
    std::unique_lock<std::mutex> lock(this->mutex);
    pushCond.wait(lock);
}

ReadFunctor::ReadFunctor(const SOCKET& s) {
    this->socket = s;
}

void ReadFunctor::set_socket(const SOCKET& s) {
    this->socket = s;
}

SOCKET ReadFunctor::get_socket() {
    return this->socket;
}

void ReadFunctor::pop(std::string &data) {
    this->queue.pop(data);
}

void ReadFunctor::operator()() {
    std::vector<char> buffer(SOCKET_BUFFER_SIZE);
    while (recv(this->socket, &buffer[0], buffer.size(), 0) > 0) {
        this->queue.push(&buffer[0]);
    }
    std::cout << "read ready to join" << std::endl;
}

WriteFunctor::WriteFunctor(const SOCKET& s) {
    this->socket = s;
}

void WriteFunctor::set_socket(const SOCKET &s) {
    this->socket = s;
}

SOCKET WriteFunctor::get_socket() {
    return this->socket;
}

void WriteFunctor::push(const std::string &data) {
    this->queue.push(data);
}

void WriteFunctor::operator()() {
    std::string buffer;
    do {
        //this->queue.wait_for_push();
        this->queue.pop(buffer);
    } while (send(this->socket, buffer.c_str(), buffer.size(), 0) >= 0);
    std::cout << "write ready to join" << std::endl;
}

Network::Network() {
    int status;

    // start winsock (must be ended for each start)
    if ((status = WSAStartup(MAKEWORD(2, 0), &this->wsaData)) != 0) {
        std::cerr << "WSAStartup() failed: " << status << std::endl;
    }

    // get LAN address
    this->localIP = this->get_localhost();
    std::cout << this->localIP << std::endl;
}

Network::~Network() {
    if (this->ephOpen) {
        this->close_ephemeral();
    }

    for (auto& threads: this->IOThreads) {
        threads.second.first.join();
        threads.second.second.join();
    }

    WSACleanup();
}

/**
 * gets a list of connected sockets
 *
 * @return  copy of list of connected sockets
 */
std::vector<SOCKET> Network::get_connected_ports() {
    return this->connectedPorts;
}

bool Network::close_socket(SOCKET socket) {
    auto position = std::find(this->connectedPorts.begin(), this->connectedPorts.end(), socket);
    if (position == this->connectedPorts.end()) {
        std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
        std::cerr << "\tcould not find a match for the given socket." << std::endl;
        return true;
    }

    int status;
    if ((status = closesocket(*position)) != 0) {
        std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
        std::cerr << "shutdown() failed with err: " << status << std::endl;
        return false;
    }

    this->connectedPorts.erase(position);
    return true;
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
    if (this->ephOpen) {
        std::cerr << "Network::open_ephemeral()" << std::endl << "\tephemeral port already open" << std::endl;
        return true;
    }

    int status;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    if ((status = getaddrinfo(this->localIP.c_str(), "0", &hints, &this->ephAddr)) != 0) {
        std::cerr << "getaddrinfo() failed with err: " << gai_strerror(status) << std::endl;
        return (this->ephOpen = false);
    }

    if ((this->ephemeralPort = socket(this->ephAddr->ai_family, this->ephAddr->ai_socktype, this->ephAddr->ai_protocol)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror(this->ephemeralPort) << std::endl;
        return (this->ephOpen = false);
    }

    if ((status = bind(this->ephemeralPort, this->ephAddr->ai_addr, this->ephAddr->ai_addrlen)) != 0) {
        std::cerr << "bind() failed with err: " << gai_strerror(status) << std::endl;
        return (this->ephOpen = false);
    }

    if ((status = listen(this->ephemeralPort, CONNECTION_LIMIT)) != 0) {
        std::cerr << "listen() failed with err: " << gai_strerror(status) << std::endl;
        return (this->ephOpen = false);
    }

    if ((status = getsockname(this->ephemeralPort, this->ephAddr->ai_addr, (int*) &this->ephAddr->ai_addrlen)) != 0) {
        std::cerr << "getsockname() failed with err: " << gai_strerror(status) << std::endl;
        return (this->ephOpen = false);
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

    return (this->ephOpen = true);
}

bool Network::close_ephemeral() {
    if (!this->ephOpen) {
        std::cerr << "Network::close_ephemeral()" << std::endl << "\tephemeral port not open or already closed" << std::endl;
        return true;
    }

    int status;

    if ((status = closesocket(this->ephemeralPort)) != 0) {
        std::cerr << "shutdown() failed with err: " << status << std::endl;
        return (this->ephOpen = false);
    } else {
        freeaddrinfo(this->ephAddr);
    }

    return (this->ephOpen = true);
}

bool Network::accept_connections() {
    if (!this->ephOpen) {
        std::cerr << "Network::accept_connections()" << std::endl << "\tEphemeral port has not been opened, cannot accept connections" << std::endl;
        return false;
    }

    SOCKET newfd;
    sockaddr_storage newAddr;

    socklen_t newAddrLen = sizeof(newAddr);
    if ((newfd = accept(this->ephemeralPort, (sockaddr*) &newAddr, &newAddrLen)) != -1) {
        this->connectedPorts.emplace_back(newfd);
        ReadFunctor newRead(newfd);
        WriteFunctor newWrite(newfd);
        this->IOFunctors.emplace(newfd, std::make_pair(std::move(newRead), std::move(newWrite)));
        std::thread newReadThread(std::ref(this->IOFunctors.at(newfd).first));
        std::thread newWriteThread(std::ref(this->IOFunctors.at(newfd).second));
        this->IOThreads.emplace(newfd, std::make_pair(std::move(newReadThread), std::move(newWriteThread)));
        return true;
    }

    std::cerr << "WARNING: accept() failed to find an incoming connection either because one does not exist or their was an error." << std::endl;
    return false;
}

void Network::read(const SOCKET &s, std::string &data) {
    auto iofunctor = this->IOFunctors.find(s);
    if (iofunctor == this->IOFunctors.end()) {
        std::cerr << "bool Network::read(const SOCKET &s, std::string &data)" << std::endl;
        std::cerr << "\tcould not find socket" << std::endl;
        data.clear();
    }

    (*iofunctor).second.first.pop(data);
}

void Network::write(const SOCKET &s, const std::string &data) {
    auto iofunctor = this->IOFunctors.find(s);
    if (iofunctor == this->IOFunctors.end()) {
        std::cerr << "bool Network::read(const SOCKET &s, std::string &data)" << std::endl;
        std::cerr << "\tcould not find socket" << std::endl;
    }

    (*iofunctor).second.second.push(data);
}