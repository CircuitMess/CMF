#ifndef CMF_UDPLISTENER_H
#define CMF_UDPLISTENER_H

#include "Object/Object.h"

class UDPListener : public Object {
    GENERATED_BODY(UDPListener, Object)

public:
    UDPListener(uint16_t port = 6001, const std::string& ip = "") noexcept;
    virtual ~UDPListener() noexcept override;

    size_t read(std::vector<uint8_t>& buffer) const noexcept;
    size_t read(uint8_t* buffer, size_t count) const noexcept;

private:
    int socket = -1;
};

#endif //CMF_UDPLISTENER_H