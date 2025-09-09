#include "WiFiStation.h"
#include "Periphery/WiFi.h"

DEFINE_LOG(WiFIStation)

WiFiStation::WiFiStation() noexcept : hysteresis({0, 20, 40, 60, 80}, 1){
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    wifi->startStation();

    wifi->OnScanDone.bind(this, &WiFiStation::onScanDone);
    wifi->OnStationConnected.bind(this, &WiFiStation::onConnect);
    wifi->OnStationDisconnected.bind(this, &WiFiStation::onDisconnect);
}

void WiFiStation::connect() const noexcept {
    if(state != State::Disconnected && state != State::Connected) {
        return;
    }

    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    wifi->startScanning();
}

void WiFiStation::disconnect() noexcept {
    if(state == State::Disconnected) {
        return;
    }

    cachedSSID = "";
    attemptedCachedSSID = false;

    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    switch (state) {
        case State::Connected: {
            wifi->disconnect();
            break;
        }
        case State::Connecting: {
            state = State::Disconnected;
            wifi->disconnect();
            break;
        }
        case State::Disconnected: {
            break;
        }
        case State::Scanning: {
            state = State::ConnectionAbort;
            wifi->stopScanning();
            break;
        }
        default: {
            break;
        }
    }
}

WiFiStation::State WiFiStation::getState() const noexcept {
    return state;
}

WiFiStation::ConnectionStrength WiFiStation::getConnectionStrength() noexcept {
    const Application* app = getApp();
    if(app == nullptr) {
        return ConnectionStrength::None;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return ConnectionStrength::None;
    }

    hysteresis.update(-wifi->getConnectionRSSI());
    return static_cast<ConnectionStrength>(hysteresis.get());
}

void WiFiStation::onScanDone(uint32_t status, uint8_t number, uint8_t id) noexcept {
    if(state == State::ConnectionAbort) {
        state = State::Disconnected;
        OnStationEvent.broadcast(EventType::Connect/*, ""*/, false);
        return;
    }

    if(attemptedCachedSSID) {
        cachedSSID = "";
        attemptedCachedSSID = false;
    }

    std::string ssid = cachedSSID;

    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    if(ssid.empty()) {
        std::vector<wifi_ap_record_t> apRecords = wifi->getAPRecords(ScanListSize);

        wifi_ap_record_t* apRecord = findNetwork(apRecords);

        if(apRecord == nullptr) {
            state = State::Disconnected;
            OnStationEvent.broadcast(EventType::Connect/*, ""*/, false);
            return;
        }

        ssid = cachedSSID = std::string(reinterpret_cast<const char*>(apRecord->ssid));
        attemptedCachedSSID = false;
    }else {
        attemptedCachedSSID = true;
    }

    if(ssid.empty()) {
        state = State::Disconnected;
        OnStationEvent.broadcast(EventType::Connect/*, ""*/, false);
        return;
    }

    state = State::Connecting;
    connectTries = 0;

	CMF_LOG(WiFIStation, LogLevel::Info, "Attempting connect on SSID: %s\n", ssid.c_str());
    wifi->setTargetParameters(ssid, "NeveraNevera");
    wifi->connect();
}

void WiFiStation::onConnect(/*std::string ssid, std::string mac,*/ uint8_t channel, wifi_auth_mode_t authMode, uint16_t aid) noexcept {
	CMF_LOG(WiFIStation, LogLevel::Info, "onConnected event");
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    wifi->resetIPInfo();

    if(state == State::Connected) {
		CMF_LOG(WiFIStation, LogLevel::Info, "early return, already connected");
        wifi->disconnect();
        return;
    }

    state = State::Connected;

    /*if(!cachedSSID.empty() && attemptedCachedSSID){
		TRACE_LOG("");
        return;
    }*/

    OnStationEvent.broadcast(EventType::Connect/*, mac*/, true);
}

void WiFiStation::onDisconnect(/*std::string ssid, std::string mac,*/ uint8_t reason, int8_t rssi) noexcept {
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    if(!cachedSSID.empty() && !attemptedCachedSSID && reason == WIFI_REASON_SA_QUERY_TIMEOUT) {
        OnStationEvent.broadcast(EventType::Probe/*, mac*/, false);

        state = State::Disconnected;
        connect();
    }

    if(state == State::Connecting) {
        if(++connectTries <= ConnectRetries) {
            wifi->connect();
        }else {
            state = State::Disconnected;

            OnStationEvent.broadcast(EventType::Connect/*, mac*/, false);
        }

        return;
    }

    if(state == State::ConnectionAbort) {
        state = State::Disconnected;
        OnStationEvent.broadcast(EventType::Connect/*, mac*/, false);
        return;
    }

    state = State::Disconnected;

    OnStationEvent.broadcast(EventType::Disconnect/*, mac*/, false);
}

wifi_ap_record_t* WiFiStation::findNetwork(std::vector<wifi_ap_record_t>& networks) noexcept {
    std::vector<wifi_ap_record_t*> compatibleNetworks;

    for(wifi_ap_record_t& network : networks) {
        if(strncmp(reinterpret_cast<const char*>(network.ssid), "Nevera Controller", 17) == 0){
            compatibleNetworks.emplace_back(&network);
        }
    }

    if(compatibleNetworks.empty()) {
        return nullptr;
    }

    std::remove(compatibleNetworks.begin(), compatibleNetworks.end(), nullptr);

    std::sort(compatibleNetworks.begin(), compatibleNetworks.end(), [](wifi_ap_record_t* first, wifi_ap_record_t* second) -> bool {
        return first->rssi > second->rssi;
    });

    return compatibleNetworks.front();
}
