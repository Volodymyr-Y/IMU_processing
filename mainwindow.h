#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "my3dwindow.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QPlainTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();

    void handleError(QSerialPort::SerialPortError error);
signals:
        void sendReadings(float x);

private:
    Ui::MainWindow *ui;
    My3DWindow* my3dWindow;
    QSerialPort* serial;
    QPlainTextEdit* console;
    QString serial_buffer;
};
#endif // MAINWINDOW_H
