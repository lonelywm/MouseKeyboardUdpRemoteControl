Qt version mouse keyboard remote control client & server


Fature:
- Very fast by using udp message.
- Low cpu usage, by sending or receiving message immediate to a queue, but processing them by a QTimer in another thread, you can change the frequency by changing the para of timer's start(msec) function
- Thread safe by using QT's slot, it fundamentally supports thread sync messages, no need the usage of mutex.
- Message order guarantee, by using udpmessage id check.


Usage:
udpmousekeyboard.h defines two classes: UdpMouseKeyboardClient and UdpMouseKeyboardServer. Just promote the widget you want the response the key and mouse message to UdpMouseKeyboardClient, and on the server side, just new an instance of UdpMouseKeyboardServer. You can modify the port of the server and the ip&port of destination in the client.