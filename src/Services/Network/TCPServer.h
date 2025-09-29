#ifndef CMF_TCPSERVER_H
#define CMF_TCPSERVER_H

#include "Event/EventBroadcaster.h"
#include "Object/Object.h"
#include "WiFiAccessPoint.h"

class TCPServer : public Object {
    GENERATED_BODY(TCPServer, Object)

public:
    DECLARE_EVENT(ConnectedEvent, TCPServer);
    ConnectedEvent OnConnected{this};

    DECLARE_EVENT(DisconnectedEvent, TCPServer);
    DisconnectedEvent OnDisconnected{this};

public:
    TCPServer(uint16_t port = 6000, const std::string& ip = "") noexcept;

    bool isConnected() const noexcept;

    bool accept() noexcept;
    void disconnect() noexcept;

    bool read(std::vector<uint8_t>& buffer) noexcept;
    bool write(const std::vector<uint8_t>& buffer) noexcept;

    bool read(uint8_t* buffer, size_t count) noexcept;
    bool write(uint8_t* buffer, size_t count) noexcept;

private:
    int socket = -1;
    int client = -1;

private:
    void onAccessPointEvent(WiFiAccessPoint::EventType eventType);
};

#endif //CMF_TCPSERVER_H