/**
 * @author Caleb Aitken
 * @date 19/04/2020
 *
 * Windows multithreaded TCP/TP socket networking source
 */

#include "core/Network.h"

ReadFunctor::ReadFunctor(const SOCKET& s) {
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
}

WriteFunctor::WriteFunctor(const SOCKET& s) {
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
    } while (send(this->socket, buffer.c_str(), buffer.length(), 0) >= 0);
}

AcceptConnectionFunctor::AcceptConnectionFunctor(const SOCKET& ephSock) {
    this->socket = ephSock;
}

SOCKET AcceptConnectionFunctor::get_socket() {
    return this->socket;
}

void AcceptConnectionFunctor::pop(SOCKET& s) {
    this->queue.pop(s);
}

void AcceptConnectionFunctor::operator()() {
    SOCKET newfd = 0;

    while ((int)(newfd = accept(this->socket, nullptr, nullptr)) > 0) {
        this->queue.push(newfd);
    }
}

Network::Network() {
    int status;

    // start winsock (must be ended for each start)
    if ((status = WSAStartup(MAKEWORD(2, 0), &this->wsaData)) != 0) {
        std::cerr << "WSAStartup() failed: " << status << std::endl;
    }

    // get LAN address
    this->localIP = this->get_localhost_helper();
    //std::cout << this->localIP << std::endl;
}

Network::~Network() {
    if (this->ephOpen) {
        this->close_ephemeral();
    }

    while (!this->connectedPorts.empty()) {
        this->close_socket(this->connectedPorts.at(0));
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

std::string Network::get_IP_of_connected_port(const SOCKET& socket) {
    struct sockaddr_in addr;
    int status, size = sizeof(addr);

    if ((status = getpeername(socket, (sockaddr*) &addr, &size)) < 0) {
        return {};
    }

    return inet_ntoa(addr.sin_addr);
}

bool Network::close_socket(SOCKET socket) {
    if (socket == ephemeralPort) {
        return this->close_ephemeral();
    }

    std::cout << "removing socket: " << socket << std::endl;

    int status;

    auto socketPosition = std::find(this->connectedPorts.begin(), this->connectedPorts.end(), socket);
    if (socketPosition == this->connectedPorts.end()) {
        std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
        std::cerr << "\tcould not find a match for the given socket." << std::endl;
    } else {
        if ((status = closesocket(*socketPosition)) != 0) {
            std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
            std::cerr << "shutdown() failed with err: " << status << std::endl;
            return false;
        }

        this->connectedPorts.erase(socketPosition);
    }

    auto ioThreadsPosition = this->IOThreads.find(socket);
    if (ioThreadsPosition == this->IOThreads.end()) {
        std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
        std::cerr << "\tcould not find a match for the given socket's thread." << std::endl;
    } else {
        (*ioThreadsPosition).second.first.join();
        (*ioThreadsPosition).second.second.join();
        this->IOThreads.erase(ioThreadsPosition);
    }

    auto ioFunctorPosition = this->IOFunctors.find(socket);
    if (ioFunctorPosition == this->IOFunctors.end()) {
        std::cerr << "Network::close_socket(SOCKET socket)" << std::endl;
        std::cerr << "\tcould not find a match for the given socket's functor." << std::endl;
        this->IOFunctors.erase(ioFunctorPosition);
    }

    return true;
}

/**
 * Gets local network IPv4 address.
 * This is achieved by opening a socket to the loopback and then asking for the socket name.
 *
 * @return  a std::string containing the local IP address.
 */
std::string Network::get_localhost_helper() {
    int status;
    SOCKET sockfd;
    sockaddr_in loopback;

    memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = INADDR_LOOPBACK;
    loopback.sin_port = htons(DNS_PORT);

    if ((int)(sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror((int)sockfd) << std::endl;
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

std::string Network::get_eph_port() {
    if (!this->ephOpen) {
        std::cerr << "Network::get_eph_port()" << std::endl << "\tephemeral port not open" << std::endl;
        return {};
    }

    return std::to_string(ntohs(((struct sockaddr_in*) this->ephAddr->ai_addr)->sin_port));
}

bool Network::open_ephemeral() {
    if (this->ephOpen) {
        std::cerr << "Network::open_ephemeral()" << std::endl << "\tephemeral port already open" << std::endl;
        return (this->ephOpen = true);
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

    if ((int)(this->ephemeralPort = socket(this->ephAddr->ai_family, this->ephAddr->ai_socktype, this->ephAddr->ai_protocol)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror((int)this->ephemeralPort) << std::endl;
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

    // TODO
    //std::cout << ntohs(((struct sockaddr_in*) this->ephAddr->ai_addr)->sin_port) << std::endl;

    this->ephAcceptFunctor = AcceptConnectionFunctor(this->ephemeralPort);
    this->ephAcceptThread = std::thread(std::ref(this->ephAcceptFunctor));

    return (this->ephOpen = true);
}

bool Network::close_ephemeral() {
    if (!this->ephOpen) {
        std::cerr << "Network::close_ephemeral()" << std::endl << "\tephemeral port not open or already closed" << std::endl;
        return (this->ephOpen = false);
    }

    int status;

    if ((status = closesocket(this->ephemeralPort)) != 0) {
        std::cerr << "shutdown() failed with err: " << status << std::endl;
        return (this->ephOpen = true);
    } else {
        freeaddrinfo(this->ephAddr);

        this->ephAcceptThread.join();

        return (this->ephOpen = false);
    }
}

/* TODO
bool Network::accept_connections() {
    if (!this->ephOpen) {
        std::cerr << "Network::accept_connections()" << std::endl << "\tEphemeral port has not been opened, cannot accept connections" << std::endl;
        return false;
    }

    SOCKET newfd;
    sockaddr_storage newAddr;

    socklen_t newAddrLen = sizeof(newAddr);
    if ((int)(newfd = accept(this->ephemeralPort, (sockaddr*) &newAddr, &newAddrLen)) < 0) {
        std::cerr << "WARNING: accept() failed to find an incoming connection either because one does not exist or their was an error." << std::endl;
        return false;
    }

    this->connectedPorts.emplace_back(newfd);
    ReadFunctor newRead(newfd);
    WriteFunctor newWrite(newfd);
    this->IOFunctors.emplace(newfd, std::make_pair(std::move(newRead), std::move(newWrite)));
    std::thread newReadThread(std::ref(this->IOFunctors.at(newfd).first));
    std::thread newWriteThread(std::ref(this->IOFunctors.at(newfd).second));
    this->IOThreads.emplace(newfd, std::make_pair(std::move(newReadThread), std::move(newWriteThread)));
    return true;
}
 */

bool Network::accept_connections() {
    SOCKET s;
    this->ephAcceptFunctor.pop(s);
    if (s == 0) {
        return false;
    } else {
        std::cout << "adding socket: " << s << std::endl;
        this->connectedPorts.emplace_back(s);
        ReadFunctor newRead(s);
        WriteFunctor newWrite(s);
        this->IOFunctors.emplace(s, std::make_pair(std::move(newRead), std::move(newWrite)));
        std::thread newReadThread(std::ref(this->IOFunctors.at(s).first));
        std::thread newWriteThread(std::ref(this->IOFunctors.at(s).second));
        this->IOThreads.emplace(s, std::make_pair(std::move(newReadThread), std::move(newWriteThread)));
        return true;
    }
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

std::string Network::get_localhost() {
    return std::string(this->localIP);
}
