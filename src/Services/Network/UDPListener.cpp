#include "UDPListener.h"
#include "Log/Log.h"
#include <lwip/sockets.h>

DEFINE_LOG(UDPListener);

UDPListener::UDPListener(uint16_t port /*= 6001*/, const std::string& ip /*= ""*/) noexcept {
    socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(socket == -1) {
        CMF_LOG(UDPListener, Error, "Can't create socket, errno=%d: %s", errno, strerror(errno));
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    if(bind(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0){
        CMF_LOG(UDPListener, Error, "Can't bind address to socket, errno=%d: %s", errno, strerror(errno));
        socket = -1;
        return;
    }

    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

UDPListener::~UDPListener() noexcept {
    close(socket);
}

size_t UDPListener::read(std::vector<uint8_t>& buffer) const noexcept {
   return read(buffer.data(), buffer.size());
}

size_t UDPListener::read(uint8_t* buffer, size_t count) const noexcept {
    if(socket == -1){
        CMF_LOG(UDPListener, Error, "Read, but socket not set-up");
        return -1;
    }

    if(buffer == nullptr || count == 0) {
        return 0;
    }

    const int bytes = ::recv(socket, buffer, count, 0);

    if(bytes < 0){
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }

        CMF_LOG(UDPListener, Error, "Read error, errno=%d: %s", errno, strerror(errno));

        return -1;
    }

    return bytes;
}
