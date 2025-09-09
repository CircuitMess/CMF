#include "TCPClient.h"
#include <lwip/sockets.h>
#include "Log/Log.h"

DEFINE_LOG(TCPClient)

TCPClient::TCPClient() noexcept {
    Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFiStation* station = app->getService<WiFiStation>();
    if(station == nullptr) {
        return;
    }

    station->OnStationEvent.bind(this, &TCPClient::onStationEvent);
}

bool TCPClient::isConnected() const noexcept {
    return socket != -1;
}

bool TCPClient::connect() noexcept {
    if(socket != -1) {
        CMF_LOG(TCPClient, Error, "Connect, but already connected");
        return false;
    }

    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(socket == -1) {
        CMF_LOG(TCPClient, Error, "Failed to create socket, errno=%d: %s", errno, strerror(errno));
        return false;
    }

    timeval to{};
    to.tv_sec = 1;
    to.tv_usec = 0;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*) &to, sizeof(to));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    // TODO fix this shit
    /*addr.sin_port = htons(TCPPort);
    inet_pton(AF_INET, ControllerIP, &addr.sin_addr);*/

    if(::connect(socket, (sockaddr*) &addr, sizeof(addr)) != 0){
        CMF_LOG(TCPClient, Error, "Failed to connect, errno=%d: %s", errno, strerror(errno));
        return false;
    }

    OnConnect.broadcast();

    return true;
}

void TCPClient::disconnect() noexcept {
    if(socket == -1) {
        CMF_LOG(TCPClient, Warning, "Disconnect, but not connected");
        return;
    }

    OnDisconnect.broadcast();

    close(socket);
    socket = -1;
}

bool TCPClient::read(std::vector<uint8_t>& buffer) noexcept {
    if(socket == -1) {
        CMF_LOG(TCPClient, Warning, "Read, but socket not connected");
        return false;
    }

    if(buffer.empty()) {
        return true;
    }

    size_t total = 0;
    while(total < buffer.size()) {
        const int now = ::read(socket, buffer.data() + total, buffer.size() - total);

        if(now == 0) {
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
                disconnect();
                return true;
            }
        }

        total += now;
    }

    return true;
}

bool TCPClient::write(const std::vector<uint8_t>& buffer) noexcept {
    if(socket == -1) {
        CMF_LOG(TCPClient, Warning, "Write, but socket not connected");
        return false;
    }

    if(buffer.empty()) {
        return true;
    }

    size_t total = 0;
    while(total < buffer.size()) {
        const int now = ::write(socket, buffer.data() + total, buffer.size() - total);

        if(now == 0){
            disconnect();
            return false;
        }else if(now < 0){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                vTaskDelay(1);
                continue;
            }else{
                disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

bool TCPClient::read(uint8_t* buffer, size_t count) noexcept {
    if(socket == -1) {
        CMF_LOG(TCPClient, Warning, "Read, but socket not connected");
        return false;
    }

    if(buffer == nullptr || count == 0) {
        return true;
    }

    size_t total = 0;
    while(total < count) {
        const int now = ::read(socket, buffer + total, count - total);

        if(now == 0) {
            disconnect();
            return false;
        }else if(now < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(1);
                continue;
            }else {
                disconnect();
                return true;
            }
        }

        total += now;
    }

    return true;
}

bool TCPClient::write(uint8_t* buffer, size_t count) noexcept {
    if(socket == -1) {
        CMF_LOG(TCPClient, Warning, "Write, but socket not connected");
        return false;
    }

    if(buffer == nullptr || count == 0) {
        return true;
    }

    size_t total = 0;
    while(total < count) {
        const int now = ::write(socket, buffer + total, count - total);

        if(now == 0){
            disconnect();
            return false;
        }else if(now < 0){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                vTaskDelay(1);
                continue;
            }else{
                disconnect();
                return false;
            }
        }

        total += now;
    }

    return true;
}

void TCPClient::onStationEvent(WiFiStation::EventType eventType, bool success) noexcept {
    if(socket == -1) {
        return;
    }

    if(eventType != WiFiStation::EventType::Disconnect) {
        return;
    }

    disconnect();
}
