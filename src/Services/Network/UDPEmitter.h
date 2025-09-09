#ifndef CMF_UDP_EMITTER_H
#define CMF_UDP_EMITTER_H

#include <lwip/sockets.h>
#include "Object/Object.h"

class UDPEmitter : public Object {
    GENERATED_BODY(UDPEmitter, Object)

public:
    UDPEmitter() noexcept;
    virtual ~UDPEmitter() noexcept override;

    bool write(const std::vector<uint8_t>& buffer) noexcept;
    bool write(const uint8_t* buffer, size_t count) noexcept;

private:
    int socket = -1;
    sockaddr_in dest{};
};

#endif //CMF_UDP_EMITTER_H