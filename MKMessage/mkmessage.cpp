#include <mkmessage.h>
#include <windows.h>
#include <QMap>
#include <utils.h>
#include <QtDebug>

void execMKMessage(const MKMessage &msg)
{
    if (msg.actionType <= KeyRelease ) { // 键盘
        auto value = msg.keyType;
        INPUT input;
        memset(&input, 0, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = value;
//            input.ki.wScan = value;
        if (msg.actionType == KeyRelease) {
            input.ki.dwFlags = KEYEVENTF_KEYUP; // union code  | 0x04
        } else if (msg.actionType == KeyPress) {
        }
        SendInput(1, &input, sizeof(INPUT));  // 第一个是发送几个，因为input可以是数组。
        qDebug() << msg.keyType << " ;  " << msg.actionType << "  ;  " << value;

    } else { // 鼠标
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        if (msg.actionType == MouseMove) {
            RECT rect;
            ::GetWindowRect(::GetDesktopWindow(),&rect);
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            input.mi.dwFlags |= MOUSEEVENTF_ABSOLUTE;
            input.mi.dx = msg.px * (65535 / float(rect.right));
            input.mi.dy = msg.py * (65535 / float(rect.bottom));
        }
        if (msg.actionType == MousePress && msg.keyType == LeftButton) { // 左键按下
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            qDebug() << "left down";
        }
        else if (msg.actionType == MouseRelease && msg.keyType == LeftButton) { // 左键抬起
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            qDebug() << "left up";
        }
        else if (msg.actionType == MousePress && msg.keyType == RightButton) {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            qDebug() << "right up";
        }
        else if (msg.actionType == MouseRelease && msg.keyType == RightButton) {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            qDebug() << "left up";
        }
        else if (msg.actionType == MousePress && msg.keyType == MiddleButton) {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        }
        else if (msg.actionType == MouseRelease && msg.keyType == MiddleButton) { // 中键抬起
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        }
        else if (msg.actionType == MousePress && msg.keyType == XButton1) {  // 附加键按下
            input.mi.dwFlags = MOUSEEVENTF_XDOWN;
        }
        else if (msg.actionType == MouseRelease && msg.keyType == XButton1) { // 附加键抬起
            input.mi.dwFlags = MOUSEEVENTF_XUP;
        }
        else if (msg.actionType == MouseWheelUp) {
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = 100;
        }
        else if (msg.actionType == MouseWheelDown) {
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = -100;
        }
        qDebug() << "mouse: " << msg.px << "  ;  " << msg.py;
        SendInput(1, &input, sizeof(INPUT));
    }
}
