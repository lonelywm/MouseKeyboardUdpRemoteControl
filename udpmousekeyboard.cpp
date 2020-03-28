#include "udpmousekeyboard.h"
#include <QUdpSocket>
#include <mkmessage.h>
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#include <QVector>
#include <QtAlgorithms>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <qnamespace.h>
#include <qmath.h>
#include <windows.h>
#include <vkmap.h>
#include <QTimer>
#include <utils.h>


#define QUEUE_MAX_COUNT_HOLD 48
#define MARKER_WAIT_LOOP_COUNT 1

QMap<int, char> qtKey2winKeyMap = VKMap();
int qtMouseEventToSendInputEnum(uint16_t key_type, uint8_t action_type) {
    if (action_type == MouseMove) {
        return MOUSEEVENTF_MOVE;
    }
    else if (action_type == MousePress) {
        if (checkBit(key_type, log2(LeftButton)))
            return MOUSEEVENTF_LEFTDOWN;
        else if (checkBit(key_type, log2(RightButton)))
            return MOUSEEVENTF_RIGHTDOWN;
        else if (checkBit(key_type, log2(MiddleButton)))
            return MOUSEEVENTF_MIDDLEDOWN;
        else if (checkBit(key_type, log2(XButton1)))
            return MOUSEEVENTF_XDOWN;
    }
    else if (action_type == MouseRelease) {
        if (checkBit(key_type, log2(LeftButton)))
            return MOUSEEVENTF_LEFTUP;
        else if (checkBit(key_type, log2(RightButton)))
            return MOUSEEVENTF_RIGHTUP;
        else if (checkBit(key_type, log2(MiddleButton)))
            return MOUSEEVENTF_MIDDLEUP;
        else if (checkBit(key_type, log2(XButton1)))
            return MOUSEEVENTF_XUP;
    } else if (action_type == MouseWheelUp) {
        return MOUSEEVENTF_WHEEL;
    } else if (action_type == MouseWheelDown) {
        return MOUSEEVENTF_WHEEL;
    }
}

bool continuous(uint16_t smaller, uint16_t larger) {
    if (smaller == 255*255 && larger == 0) {
        return true;
    } else if (larger == smaller + 1) {
        return true;
    } else {
        return false;
    }
}

bool larger(uint16_t former, uint16_t later) {
    uint16_t min_dist = abs((int32_t)former - later);
    min_dist = min_dist < 255*255 - min_dist ? min_dist : 255*255 - min_dist;
    uint16_t dist = later - former > 0 ? later - former : later - former + 65535;
    if (dist == min_dist) {
        return true;
    } else {
        return false;
    }
}

UdpMouseKeyboardClient::UdpMouseKeyboardClient(QWidget *parent)
    : QWidget(parent), m_msgIndex(0)
{
    setMouseTracking(true);
    m_pUdpClient = new QUdpSocket(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000 / 60);
//    connect();
//    mousePressEvent(QMouseEvent *event);
//    connect(this, &QWidget::mousePressEvent, [=]() {
//        this->ip = ui->cb_ip->currentData().toString();
//        qDebug() << this->ip;
//        ui->stackedWidget->setCurrentIndex(0);
//    });
   // connect((QObject*)m_pWidget, SIGNAL( mousePressEvent(QMouseEvent*) ), this, SLOT(mousePressEvent(QMouseEvent*)));   //信号连接
}

void UdpMouseKeyboardClient::timeout() {
    //调用writeDatagram函数来发送数据(点对点消息和广播消息都适用)
    while(!m_sendMsgQueue.isEmpty()) {
        UdpMKMessage udpMsg;
        udpMsg.index = m_msgIndex; // 给一个编号，为了保证接受到的包是顺序的.
        m_msgIndex += 1;

        while(!m_sendMsgQueue.isEmpty() && udpMsg.size < UDP_MSG_MAX_SIZE) {
            auto msg = m_sendMsgQueue.dequeue();
            udpMsg.push(msg);
        }
//        qDebug() << "send: ";
        m_pUdpClient->writeDatagram((const char*)&udpMsg, sizeof(udpMsg), QHostAddress("10.168.10.223"), 5000);
        //m_pUdpClient->writeDatagram((const char*)&udpMsg, sizeof(udpMsg), QHostAddress("192.168.1.168"), 5000);
    }
}

void UdpMouseKeyboardClient::mousePressEvent(QMouseEvent *e)
{
    MKMessage msg((uint16_t)e->button(), MousePress, e->localPos().x(), e->localPos().y());
    m_sendMsgQueue.enqueue(msg);
    // qDebug() << e;
}


void UdpMouseKeyboardClient::mouseMoveEvent(QMouseEvent *e)
{
    MKMessage msg((uint16_t)e->button(), MouseMove, e->localPos().x(), e->localPos().y());
    m_sendMsgQueue.enqueue(msg);
    // qDebug() << e;
}

void UdpMouseKeyboardClient::mouseReleaseEvent(QMouseEvent *e)
{
    MKMessage msg((uint16_t)e->button(), MouseRelease, e->localPos().x(), e->localPos().y());
    m_sendMsgQueue.enqueue(msg);
    // qDebug() << e;
}

void UdpMouseKeyboardClient::mouseDoubleClickEvent(QMouseEvent *e)
{
    MKMessage msg((uint16_t)e->button(), MouseDoubleClick, e->localPos().x(), e->localPos().y());
    m_sendMsgQueue.enqueue(msg);
    // qDebug() << e;
}

void UdpMouseKeyboardClient::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().ry() > 0) {
        MKMessage msg((uint8_t)MiddleButton, MouseWheelUp, 0, e->angleDelta().ry());
        m_sendMsgQueue.enqueue(msg);
    } else {
        MKMessage msg((uint8_t)MiddleButton, MouseWheelDown, 0, -e->angleDelta().ry());
        m_sendMsgQueue.enqueue(msg);
    }
}

void UdpMouseKeyboardClient::keyPressEvent(QKeyEvent *e)
{
    if (qtKey2winKeyMap.contains(e->key())) {
        qDebug() << (uint16_t)e->key();
        MKMessage msg((uint16_t)qtKey2winKeyMap[e->key()], KeyPress, 0, 0);
        m_sendMsgQueue.enqueue(msg);
    }
}

void UdpMouseKeyboardClient::keyReleaseEvent(QKeyEvent *e) {
    if (qtKey2winKeyMap.contains(e->key())) {
        qDebug() << (uint16_t)e->key();
        MKMessage msg((uint16_t)qtKey2winKeyMap[e->key()], KeyRelease, 0, 0);
        m_sendMsgQueue.enqueue(msg);
    }
}

// ================ server

UdpMouseKeyboardServer::UdpMouseKeyboardServer(QObject *parent): QObject(parent)
{
    m_pUdpServer = new QUdpSocket(this);
    m_pUdpServer->bind(5000, QUdpSocket::ShareAddress);//绑定读数据端口(监听端口)，允许其他服务绑定到此端口
    connect(m_pUdpServer, SIGNAL(readyRead()), this, SLOT(recv()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000 / 60);
}

void UdpMouseKeyboardServer::timeout()
{
    auto msg = m_rcvMsgQueue.toVector();
    // 首先按序号由小到大排列所有的包
    // 如何有顺序不连贯的，出现断续的及其后面如果的如果大于一个阈值QUEUE_MAX_COUNT_HOLD=16，就立即发送这个断续的
    // 否则继续等待，将所有的放入放在队列中，并且标记一次等待（将等待标记设置为该index）
    // 如果发现等待标记不为-1， 即有等待的序号，此时如果还没有收到该包，则立即执行（执行后就不管之前的了）
    // 但是消息的序号不总是连续的，因为整数是有上限的，必须有一种机制盘点最大的数与零也是连续的。
    qSort(m_rcvMsgQueue.begin(), m_rcvMsgQueue.end(), [](UdpMKMessage &msg1, UdpMKMessage &msg2){
        return larger(msg1.index, msg2.index);
    });
    // 上面排序成功，按照循环的序号
    // 然后看是否连续，如果连续，就直接处理
    while(m_rcvMsgQueue.length() > 0) {
        auto udpMsg = m_rcvMsgQueue.dequeue();
        if (m_processMsgIndex == -1 || continuous((uint16_t)m_processMsgIndex, udpMsg.index)) {
            // 如果是第一个包，或者是连续收的包，则立即处理
            processMKMessage(udpMsg); // 处理
            m_processMsgIndex = udpMsg.index; // 更新上一个的标记
            if (m_msgMarker.ProcessIndex == udpMsg.index) {
                m_msgMarker.ProcessIndex = -1;
                m_msgMarker.ProcessCounter = 0;
            } // 如果收到了连续的包，而且正式之前等待的，要注意清理marker标记
        } else {
            // 如果收到的包不是连续的，则分情况，如果队列中的包很多，大于阈值则立即处理，否则等下次在处理
            if (m_rcvMsgQueue.length() > QUEUE_MAX_COUNT_HOLD) {  // 大于阈值立即处理
                processMKMessage(udpMsg);
                m_processMsgIndex = udpMsg.index;
                if (m_msgMarker.ProcessIndex == udpMsg.index) {
                    m_msgMarker.ProcessIndex = -1;
                    m_msgMarker.ProcessCounter = 0;
                } // 清理marker标记
            } else if (m_msgMarker.ProcessIndex == udpMsg.index) {  // 如果还是这个不连续的，而且大于Loop次数，处理
                m_msgMarker.ProcessIndex--;
                if (m_msgMarker.ProcessCounter <= 0) {
                    processMKMessage(udpMsg);
                }
            } else { // 否则不处理，进行标记
                m_msgMarker.ProcessIndex = udpMsg.index;
                m_msgMarker.ProcessCounter = MARKER_WAIT_LOOP_COUNT;   // 在这里制定
            }
        } // end if else
    } // end while

}

void UdpMouseKeyboardServer::processMKMessage(const MKMessage &msg)
{
    execMKMessage(msg);
}

void UdpMouseKeyboardServer::processMKMessage(const UdpMKMessage &udpMsg)
{
    for(int i=0; i<udpMsg.size; i++) {
        processMKMessage(udpMsg.messages[i]);
    }
}

void UdpMouseKeyboardServer::recv()
{
    QByteArray data;
    while(m_pUdpServer->hasPendingDatagrams())
    {
        data.resize(m_pUdpServer->pendingDatagramSize());
        m_pUdpServer->readDatagram(data.data(), data.size());
        UdpMKMessage msg;
        memcpy(&msg, data.data(), data.size());
        m_rcvMsgQueue.enqueue(msg);
    }
}
