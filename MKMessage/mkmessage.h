#ifndef MKMESSAGE_H
#define MKMESSAGE_H

#include <stdint.h>
#define UDP_MSG_MAX_SIZE 16


#define INPUT_MOUSE 0
#define INPUT_KEYBOARD 1
#define INPUT_HARDWARE 2

#define KEYEVENTF_EXTENDEDKEY 0x0001
#define KEYEVENTF_KEYUP 0x0002
#define KEYEVENTF_UNICODE 0x0004
#define KEYEVENTF_SCANCODE 0x0008

#define MOUSEEVENTF_MOVE 0x0001
#define MOUSEEVENTF_LEFTDOWN 0x0002
#define MOUSEEVENTF_LEFTUP 0x0004
#define MOUSEEVENTF_RIGHTDOWN 0x0008
#define MOUSEEVENTF_RIGHTUP 0x0010
#define MOUSEEVENTF_MIDDLEDOWN 0x0020
#define MOUSEEVENTF_MIDDLEUP 0x0040
#define MOUSEEVENTF_XDOWN 0x0080
#define MOUSEEVENTF_XUP 0x0100
#define MOUSEEVENTF_WHEEL 0x0800

enum MouseButtonType {
    NoButton         = 0x00,
    LeftButton       = 0x01,
    RightButton      = 0x02,
    MidButton        = 0x04, // ### Qt 6: remove me
    MiddleButton     = MidButton,
    BackButton       = 0x08,
    XButton1         = BackButton,
    ExtraButton1     = XButton1,
    ForwardButton    = 0x10,
    XButton2         = ForwardButton,
    ExtraButton2     = ForwardButton,
    TaskButton       = 0x20,
    ExtraButton3     = TaskButton,
    ExtraButton4     = 0x40,
    ExtraButton5     = 0x80,
};

enum ActionType {
    NoAction = 0,
    KeyPress,
    KeyRelease,
    MouseMove,
    MousePress,
    MouseRelease,
    MouseDoubleClick,
    MouseWheelUp,
    MouseWheelDown
};

struct MKMessage { // 56
    uint16_t keyType = 0;
    uint8_t actionType = 0;
    uint16_t px = 0;
    uint16_t py = 0;
    MKMessage() {}
    MKMessage(uint16_t key_type, uint8_t action_type, uint16_t p_x, uint16_t p_y) {
        keyType = key_type;
        actionType = action_type;
        px = p_x;
        py = p_y;
    }
};


void execMKMessage(const MKMessage &msg);

#endif // MKMESSAGE_H
