#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <utils.h>
#include <mkmessage.h>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pClient = ui->widget;
    UdpMouseKeyboardServer *server = new UdpMouseKeyboardServer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
