#include "TCPServer.h"
#include <lwip/sockets.h>

DEFINE_LOG(TCPServer);

TCPServer::TCPServer(uint16_t port /*= 6000*/, const std::string& ip /*= ""*/) noexcept {
    if(ip.empty()) {
        return;
    }

    Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFiAccessPoint* ap = app->getService<WiFiAccessPoint>();
    if(ap == nullptr) {
        return;
    }

    ap->OnConnectionEvent.bind(this, &TCPServer::onAccessPointEvent);

    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(socket == -1) {
        CMF_LOG(TCPServer, Error, "Failed to create socket, errno=%d: %s", errno, strerror(errno));
        return;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

    if(bind(socket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
        CMF_LOG(TCPServer, Error, "Failed to bind address to socket, errno=%d: %s", errno, strerror(errno));
        socket = -1;
        return;
    }

    if(listen(socket, 1) != 0) {
        CMF_LOG(TCPServer, Error, "Failed to listen on socket, errno=%d: %s", errno, strerror(errno));
        return;
    }

    fcntl(socket, F_SETFL, O_NONBLOCK);
}

bool TCPServer::isConnected() const noexcept {
    return client != -1;
}

bool TCPServer::accept() noexcept {
    if(socket == -1) {
        CMF_LOG(TCPServer, Error, "Accept, but socket is invalid");
        return false;
    }

    if(client != -1) {
        CMF_LOG(TCPServer, Error, "Accept, but client is already connected");
        return false;
    }

    sockaddr_in address_client{};
    socklen_t address_client_len = sizeof(address_client);

    client = ::accept(socket, reinterpret_cast<sockaddr*>(&address_client), &address_client_len);

    if(client == -1) {
        CMF_LOG(TCPServer, Warning, "Cannot accept, errno=%d: %s", errno, strerror(errno));
        return false;
    }

    static constexpr int KeepAlive = 1;
    static constexpr int KeepIdle = 4;
    static constexpr int KeepInterval = 2;
    static constexpr int KeepCount = 2;

    setsockopt(client, SOL_SOCKET, SO_KEEPALIVE, &KeepAlive, sizeof(int));
    setsockopt(client, IPPROTO_TCP, TCP_KEEPIDLE, &KeepIdle, sizeof(int));
    setsockopt(client, IPPROTO_TCP, TCP_KEEPINTVL, &KeepInterval, sizeof(int));
    setsockopt(client, IPPROTO_TCP, TCP_KEEPCNT, &KeepCount, sizeof(int));

    char addr_str[32];
    inet_ntoa_r(address_client.sin_addr, addr_str, sizeof(addr_str)-1);

    CMF_LOG(TCPServer, Info, "Client %s connected", addr_str);

    OnConnected.broadcast();

    return true;
}

void TCPServer::disconnect() noexcept {
    if(client == -1) {
        CMF_LOG(TCPServer, Error, "Disconnect, but client is invalid");
        return;
    }

    close(client);
    client = -1;

    OnDisconnected.broadcast();
}

bool TCPServer::read(std::vector<uint8_t>& buffer) noexcept {
    if(client == -1) {
        CMF_LOG(TCPServer, Error, "Read, but client is invalid");
        return false;
    }

    if(buffer.empty()) {
        return true;
    }

    size_t total = 0;
    while(total < buffer.size()) {
        const int now = ::read(client, buffer.data() + total, buffer.size() - total);

        if(now == 0) {
			CMF_LOG(TCPServer, Error, "Received 0, disconnect");
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
				CMF_LOG(TCPServer, Error, "Received err \"%s\", disconnect", strerror(errno));
                disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

bool TCPServer::write(const std::vector<uint8_t>& buffer) noexcept {
    if(client == -1) {
        CMF_LOG(TCPServer, Error, "Write, but client is invalid");
        return false;
    }

    if(buffer.empty()) {
        return true;
    }

    size_t total = 0;
    while(total < buffer.size()) {
        const int now = ::write(client, buffer.data() + total, buffer.size() - total);

        if(now == 0) {
			CMF_LOG(TCPServer, Error, "Received 0 on sending, disconnect");
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
				CMF_LOG(TCPServer, Error, "Received err \"%s\" on sending, disconnect", strerror(errno));
				disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

bool TCPServer::read(uint8_t* buffer, size_t count) noexcept {
    if(client == -1) {
        CMF_LOG(TCPServer, Error, "Read, but client is invalid");
        return false;
    }

    if(buffer == nullptr || count == 0) {
        return true;
    }

    size_t total = 0;
    while(total < count) {
        const int now = ::read(client, buffer + total, count - total);

        if(now == 0) {
			CMF_LOG(TCPServer, Error, "Received 0, disconnect");
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
				CMF_LOG(TCPServer, Error, "Received err \"%s\", disconnect", strerror(errno));
                disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

bool TCPServer::write(uint8_t* buffer, size_t count) noexcept {
    if(client == -1) {
        CMF_LOG(TCPServer, Error, "Write, but client is invalid");
        return false;
    }

    if(buffer == nullptr || count == 0) {
        return true;
    }

    size_t total = 0;
    while(total < count) {
        const int now = ::write(client, buffer + total, count - total);

        if(now == 0) {
			CMF_LOG(TCPServer, Error, "Received 0 on sending, disconnect");
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
				CMF_LOG(TCPServer, Error, "Received err \"%s\" on sending, disconnect", strerror(errno));
                disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

void TCPServer::onAccessPointEvent(WiFiAccessPoint::EventType eventType) {
    if(eventType != WiFiAccessPoint::EventType::Disconnect) {
        return;
    }

    if(client == -1) {
        return;
    }

    disconnect();
}
