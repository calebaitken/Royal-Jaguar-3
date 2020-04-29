#include <iostream>
#include <ws2tcpip.h>


int main() {
    std::string IP, port;
    char buffer[1024];
    int status, sockfd;
    WSAData wsaData;
    addrinfo hints, *comAddr;

    if ((status = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) {
        std::cerr << "WSAStartup() failed." << std::endl;
    }

    std::cout << " Enter IP: ";
    std::cin >> IP;
    std::cout << "Enter port: ";
    std::cin >> port;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    if ((status = getaddrinfo(IP.c_str(), port.c_str(), &hints, &comAddr)) < 0) {
        std::cerr << "getaddrinfo() failed with err: " << gai_strerror(status) << std::endl;
    }

    if ((sockfd = socket(comAddr->ai_family, comAddr->ai_socktype, comAddr->ai_protocol)) < 0) {
        std::cerr << "socket() failed with err: " << gai_strerror(sockfd) << std::endl;
    }

    if ((status = connect(sockfd, comAddr->ai_addr, comAddr->ai_addrlen)) < 0) {
        std::cerr << "connect() failed with err: " << gai_strerror(status) << std::endl;
        return NULL;
    }

    std::string pause;
    std::cin >> pause;

    recv(sockfd, buffer, sizeof(buffer), 0);

    std::cout << buffer << std::endl;

    shutdown(sockfd, SD_BOTH);
    closesocket(sockfd);

    return 0;
}