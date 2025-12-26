#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitSendDataBtn();
}

void MainWindow::InitSendDataBtn()
{
    QPushButton* sendDataBtn = new QPushButton(this);

    sendDataBtn -> setText("发送");
}

MainWindow::~MainWindow()
{
    delete ui;
}
