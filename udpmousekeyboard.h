#ifndef UDPMOUSEKEYBOARD_H
#define UDPMOUSEKEYBOARD_H

#include <QObject>
#include <mkmessage.h>
#include <udpmkmessage.h>
#include <QQueue>
#include <QWidget>
#include <QMap>

class QKeyEvent;
class QWheelEvent;
class QMouseEvent;
class QUdpSocket;
class UdpMouseKeyboardClient: public QWidget
{
    Q_OBJECT
public:
    UdpMouseKeyboardClient(QWidget *parent = nullptr);

private slots:
    void timeout();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    QUdpSocket *m_pUdpClient = nullptr;
    QQueue<MKMessage> m_sendMsgQueue;
    uint16_t m_msgIndex = 0;
    QWidget *m_pWidget;
};

struct MKMessageMarker {
    int32_t ProcessCounter = 0;  // 如果在某处的收到的包的序号是不连续的，则需要等待多少个周期才处理.
    int32_t ProcessIndex = -1;    // 保存不连续的那个序号
};

class UdpMouseKeyboardServer: public QObject
{
    Q_OBJECT
public:
    UdpMouseKeyboardServer(QObject *parent);
public slots:
    void recv();
    void timeout();
private:
    int32_t m_processMsgIndex = -1;
    MKMessageMarker m_msgMarker;
    QUdpSocket *m_pUdpServer = nullptr;
    QQueue<UdpMKMessage> m_rcvMsgQueue;
    void processMKMessage(const MKMessage &msg);
    void processMKMessage(const UdpMKMessage &msg);
};

#endif // UDPMOUSEKEYBOARD_H
