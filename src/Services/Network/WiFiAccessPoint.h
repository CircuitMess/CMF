#ifndef CMF_WIFIACCESSPOINT_H
#define CMF_WIFIACCESSPOINT_H

#include "Event/EventBroadcaster.h"
#include "Object/Object.h"

class WiFiAccessPoint : public Object {
    GENERATED_BODY(WiFiAccessPoint, Object)

public:
    enum class EventType : uint8_t {
        Connect,
        Disconnect
    };

    DECLARE_EVENT(ConnectionEvent, WiFiAccessPoint/*, std::string*/, EventType);
    ConnectionEvent OnConnectionEvent{this};

public:
    WiFiAccessPoint();

    static void generateNewSSID() noexcept;

private:
    static constexpr const char* Password = "NeveraNevera";

private:
    void onConnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild);
    void onDisconnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild, uint8_t reason);
};

#endif //CMF_WIFIACCESSPOINT_H