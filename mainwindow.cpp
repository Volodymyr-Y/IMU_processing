#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSerialPortInfo>
#include <qcustomplot.h>

//#include <QDateTime>
//#include <QDebug>
//#include <QElapsedTimer>
//#include "my3dwindow.h"
using namespace std::chrono;

QSerialPort::BaudRate baudFromString(const QString &baudStr) {
    static const QMap<QString, QSerialPort::BaudRate> baudMap = {
        {"1200", QSerialPort::Baud1200},
        {"2400", QSerialPort::Baud2400},
        {"4800", QSerialPort::Baud4800},
        {"9600", QSerialPort::Baud9600},
        {"19200", QSerialPort::Baud19200},
        {"38400", QSerialPort::Baud38400},
        {"57600", QSerialPort::Baud57600},
        {"115200", QSerialPort::Baud115200}
    };

    return baudMap.value(baudStr, QSerialPort::Baud9600);  // Default if not found
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my3dWindow = qobject_cast<My3DWindow*>(ui->cube3DWiew);
    console = qobject_cast<QPlainTextEdit*>(ui->console);
    timer.start();
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    //connect(this, &MainWindow::sendReadings, my3dWindow, &My3DWindow::updateAttitude);
    console->setReadOnly(true);

    setCentralWidget(ui->tabWidget);

    // enter options for baud rate sellection
    ui->baudRateSelect->addItem("9600");
    ui->baudRateSelect->addItem("115200");

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        ui->portSelect->addItem(port.portName());
        qDebug() << "--------------------------------";
        qDebug() << "Port Name: " << port.portName();
        qDebug() << "Description: " << port.description();
        qDebug() << "Manufacturer: " << port.manufacturer();
        qDebug() << "Serial Number: " << port.serialNumber();
        qDebug() << "System Location: " << port.systemLocation();
        qDebug() << "Baud Rates: " << port.standardBaudRates();
    }

    auto customPlot = ui->plotRealTime;

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(255, 0, 0)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    customPlot->addGraph(); // red line
    customPlot->graph(2)->setPen(QPen(QColor(0, 0,255)));
    // set legend
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica",9));
    customPlot->graph(0)->setName("AccX");
    customPlot->graph(1)->setName("AccY");
    customPlot->graph(2)->setName("AccZ");
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    customPlot->xAxis->setLabel("Time (s)");
    customPlot->yAxis->setLabel("m/s^2");
    customPlot->yAxis->setRange(-12, 12);

    auto customPlot2 = ui->plotRealTime_2;
    customPlot2->addGraph(); // blue line
    customPlot2->graph(0)->setPen(QPen(QColor(255, 0, 0)));
    customPlot2->addGraph(); // red line
    customPlot2->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    customPlot2->addGraph(); // red line
    customPlot2->graph(2)->setPen(QPen(QColor(0, 0,255)));
    // set legend
    customPlot2->legend->setVisible(true);
    customPlot2->legend->setFont(QFont("Helvetica",9));
    customPlot2->graph(0)->setName("GyroX");
    customPlot2->graph(1)->setName("GyroY");
    customPlot2->graph(2)->setName("GyroZ");
    customPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    customPlot2->xAxis->setLabel("Time (s)");
    customPlot2->yAxis->setLabel("rad/sec");
    customPlot2->yAxis->setRange(-12, 12);

    //customPlot->setNotAntialiasedElements(QCP::aeAll);

    connect(this, &MainWindow::sendReadings, this, &MainWindow::UpdateGraph);

}
void MainWindow::openSerialPort()
{
    QString portName = ui->portSelect->currentText();


    serial->setPortName(portName);
    serial->setBaudRate(baudFromString(portName));
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //console->document()->setPlainText("opened port");


    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening serial port:" << serial->errorString();
        if (ui->console)
            ui->console->appendPlainText("Failed to open COM5: " + serial->errorString());
    } else {
        qDebug() << "Serial port opened successfully!";
        if (ui->console)
            ui->console->appendPlainText("Opened port successfully!");

        // Now we can safely set DTR/RTS
        serial->setDataTerminalReady(true);
        //serial->setRequestToSend(false);
        serial->clear(QSerialPort::AllDirections);  // Clear any old data
    }
}

void MainWindow::UpdateGraph(std::vector<float> readings)
{
    auto customPlot = ui->plotRealTime;
    auto customPlot2 = ui->plotRealTime_2;
    static double lastPointKey = 0;
    double key = timer.elapsed()/1000.0;

    if ((key-lastPointKey) > 0.01)
    {
        customPlot->graph(0)->addData(key, readings[0]);
        customPlot->graph(1)->addData(key, readings[1]);
        customPlot->graph(2)->addData(key, readings[2]);
        customPlot->xAxis->setRange(key, 8, Qt::AlignRight);

        customPlot2->graph(0)->addData(key, readings[3]);
        customPlot2->graph(1)->addData(key, readings[4]);
        customPlot2->graph(2)->addData(key, readings[5]);
        customPlot2->xAxis->setRange(key, 8, Qt::AlignRight);
        customPlot->replot();
        customPlot2->replot();
        lastPointKey = key;
    }

}

void MainWindow::closeSerialPort()
{

}



void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    //qDebug(QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error));
}

void MainWindow::readData()
{
    serial_buffer.append(serial->readAll());
    int line_break_index = serial_buffer.indexOf('\n');
    QString full_line;
    if (!(line_break_index ==-1)){
        full_line = serial_buffer.left(line_break_index);
        serial_buffer.remove(0,line_break_index+1);
        //console->appendPlainText(full_line);

        QStringList string_list = full_line.split(',', Qt::SkipEmptyParts);

        // **Step 3: Convert to float**
        std::vector<float> values;
        for (const QString &value : string_list) {
            bool ok;
            float num = value.toFloat(&ok);  // Convert to float
            if (ok) {
                values.push_back(num);
            } else {
                qDebug() << "Conversion error for value:" << value;
            }
        }

        // **Step 4: Debug output**
        sendReadings(values);
        //console->appendPlainText(QString::number(line_break_index));

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    qDebug() << "dont touch me there";
    openSerialPort();
}


void MainWindow::on_portSelect_currentTextChanged(const QString &arg1)
{
    ui->baudRateSelect->clear();
    foreach (const int baud, QSerialPortInfo(arg1).standardBaudRates()) {
        ui->baudRateSelect->addItem(QString::number(baud));
    }

    auto currentPort = QSerialPortInfo(ui->portSelect->currentText());
    ui->console->appendPlainText("Port Name: " + currentPort.portName());
    ui->console->appendPlainText("Description: " + currentPort.description());
    ui->console->appendPlainText("Manufacturer: " + currentPort.manufacturer());
    ui->console->appendPlainText("Serial Number: " + currentPort.serialNumber());
    ui->console->appendPlainText("System Location: " + currentPort.systemLocation());
}

