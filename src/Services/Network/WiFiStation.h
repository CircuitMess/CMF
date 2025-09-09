#ifndef CMF_WIFI_STATION_H
#define CMF_WIFI_STATION_H

#include <esp_wifi_types_generic.h>
#include <Event/EventBroadcaster.h>
#include "Object/Object.h"
#include "Util/Hysteresis.h"

class WiFiStation : public Object {
    GENERATED_BODY(WiFiStation, Object)

public:
    enum class State : uint8_t {
        Connected,
        Connecting,
        Disconnected,
        Scanning,
        ConnectionAbort
    };

    enum class EventType : uint8_t {
        Connect,
        Disconnect,
        Probe
    };

    enum class ConnectionStrength : uint8_t {
        None = 4,
        VeryLow = 3,
        Low = 2,
        Medium = 1,
        High = 0
    };

    DECLARE_EVENT(StationEvent, WiFiStation, EventType, /*std::string,*/ bool);
    StationEvent OnStationEvent{this};

public:
    WiFiStation() noexcept;

    void connect() const noexcept;
    void disconnect() noexcept;

    State getState() const noexcept;

    ConnectionStrength getConnectionStrength() noexcept;

private:
    Hysteresis hysteresis;
    State state = State::Disconnected;
    std::string cachedSSID;
    bool attemptedCachedSSID;
    int connectTries;

    static constexpr int ConnectRetries = 2;
    static constexpr uint16_t ScanListSize = 12;

private:
    void onScanDone(uint32_t status, uint8_t number, uint8_t id) noexcept;
    void onConnect(/*std::string ssid, std::string mac,*/ uint8_t channel, wifi_auth_mode_t authMode, uint16_t aid) noexcept;
    void onDisconnect(/*std::string ssid, std::string mac,*/ uint8_t reason, int8_t rssi) noexcept;
    static wifi_ap_record_t* findNetwork(std::vector<wifi_ap_record_t>& networks) noexcept;
};

#endif //CMF_WIFI_STATION_H