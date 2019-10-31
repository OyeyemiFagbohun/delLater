#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QStandardPaths>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    s_port1 = new QSerialPort();
    connect(s_port1, &QSerialPort::readyRead, this, &MainWindow::readData1);
    s_port2 = new QSerialPort();
    connect(s_port2, &QSerialPort::readyRead, this, &MainWindow::readData2);
    openPort1("/dev/serial0");
    openPort2("/dev/ttyUSB0");
    isDbReadStarted = false;
    dbFile.setFileName("db.db");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow :: readData1()
{
    /*
    if(!isDbReadStarted)
    {
        dbFile.open(QFile::WriteOnly | QFile::Truncate);
        dbFile.close();
        dbFile.open(QFile::WriteOnly | QFile::Append);
        ui->statusLabel->setText("Recieving");
        //QTimer::singleShot(100000, this, SLOT(stopDbRead()));
        isDbReadStarted = true;
    }
    QByteArray data = s_port1->readAll();
    if(data.endsWith(QString("THE-END\r\r\n").toStdString().c_str()));
    {
        data.chop(10);
        ui->uStatusLab->setText("Done");
        stopDbRead();
    }
    dbFile.write(data);
    */
    if(!isDbReadStarted)
    {
        QByteArray data = s_port1->readAll();
        if(QString(data) == QString("%%\n"))
        {
            dbFile.open(QFile::WriteOnly | QFile::Truncate);
            dbFile.close();
            dbFile.open(QFile::WriteOnly | QFile::Append);
            ui->statusLabel->setText("Recieving");
            isDbReadStarted = true;
        }
    }else{
        QByteArray data = s_port1->readAll();
        if(QString(data) == QString("END\n"))
        {
            dbFile.close();
            ui->statusLabel->setText("Done");
            isDbReadStarted = false;
        }else{
            dbFile.write(data);
        }
    }

}

void MainWindow :: stopDbRead()
{
    if(isDbReadStarted == true)
    {
        ui->statusLabel->setText("Done");
        dbFile.close();
        isDbReadStarted = false;
    }
}
void MainWindow :: readData2()
{
    QByteArray data = s_port2->readAll();
    ui->idLabel->setText(QString(data));
}

void MainWindow :: openPort1(QString port){
    if(s_port1->isOpen()) s_port1->close(); s_port1->setDataBits(QSerialPort::Data8); s_port1->setParity(QSerialPort::NoParity);
    s_port1->setBaudRate(9600); s_port1->setPortName(port);
    if(s_port1->open(QIODevice::ReadWrite));
}

void MainWindow :: openPort2(QString port){
    if(s_port2->isOpen()) s_port2->close(); s_port2->setDataBits(QSerialPort::Data8); s_port2->setParity(QSerialPort::NoParity);
    s_port2->setBaudRate(9600); s_port2->setPortName(port);

    if(s_port2->open(QIODevice::ReadWrite));
}
