#ifndef UDP_MOUSE_KEY_MESSAGE_H
#define UDP_MOUSE_KEY_MESSAGE_H

#include <mkmessage.h>

struct UdpMKMessage { // 24 + n * 56
    uint16_t index = 0;
    uint8_t size = 0;
    MKMessage messages[UDP_MSG_MAX_SIZE];
    int push(MKMessage msg) {
        if (size == UDP_MSG_MAX_SIZE) {
            return 0;
        } else {
            messages[size] = msg;
            size++;
        }
        return 1;
    }
};

#endif // UDP_MOUSE_KEY_MESSAGE_H
