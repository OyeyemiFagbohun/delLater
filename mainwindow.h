#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTimer>
#include <QFile>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}
class QSerialPort;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSqlDatabase db;
    QSerialPort *s_port1;
    QSerialPort *s_port2;

    bool isDbReadStarted;
    QFile dbFile;


public:
    explicit MainWindow(QWidget *parent = 0);
    void openPort1(QString);
    void openPort2(QString);
    ~MainWindow();

private slots:
    void readData1();
    void readData2();
    void stopDbRead();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
