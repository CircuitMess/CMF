#include "WiFiAccessPoint.h"
#include "Periphery/WiFi.h"

WiFiAccessPoint::WiFiAccessPoint() {
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    const uint32_t randID = rand() % 1000000;
    const std::string ssid = "Nevera Controller #" + std::to_string(randID);

    wifi->startAccessPoint(ssid, Password);
    wifi->setHidden(true);

    wifi->OnAccessPointConnection.bind(this, &WiFiAccessPoint::onConnected);
    wifi->OnAccessPointDisconnection.bind(this, &WiFiAccessPoint::onDisconnected);
}

void WiFiAccessPoint::generateNewSSID() noexcept {
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    const uint32_t randID = rand() % 1000000;
    const std::string ssid = "Nevera Controller #" + std::to_string(randID);

    wifi->setNetworkParameters(ssid, Password);
}

void WiFiAccessPoint::onConnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild) {
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    WiFi* wifi = app->getPeriphery<WiFi>();
    if(wifi == nullptr) {
        return;
    }

    if(!wifi->isHidden()) {
        wifi->setHidden(true);
    }

    OnConnectionEvent.broadcast(/*mac,*/ EventType::Connect);
}

void WiFiAccessPoint::onDisconnected(/*const std::string& mac, */uint8_t aid, bool isMeshChild, uint8_t reason) {
    OnConnectionEvent.broadcast(/*mac,*/ EventType::Disconnect);
}
