#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "my3dwindow.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QPlainTextEdit>
#include <QElapsedTimer>
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
    void UpdateGraph(std::vector<float> readings);
    void handleError(QSerialPort::SerialPortError error);
    void on_connectButton_clicked();

    void on_portSelect_currentTextChanged(const QString &arg1);

signals:
        void sendReadings(std::vector<float> readings);

private:
    QVector<double> xData, yData;
    Ui::MainWindow *ui;
    My3DWindow* my3dWindow;
    QSerialPort* serial;
    QPlainTextEdit* console;
    QString serial_buffer;
    QElapsedTimer timer;
};
#endif // MAINWINDOW_H
