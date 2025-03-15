#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "my3dwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my3dWindow = qobject_cast<My3DWindow*>(ui->widget);
    console = qobject_cast<QPlainTextEdit*>(ui->console);

    serial = new QSerialPort(this);

    //connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);


    //! [2]
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(this, &MainWindow::sendReadings, my3dWindow, &My3DWindow::updateAttitude);
    console->setReadOnly(true);


    //console->document()->setPlainText("Hello");
   // console->document()->setPlainText("Hello2");
    //console->document()->setPlainText("Hello3");
    console->setPlainText("aaa");
    console->setPlainText("aaa2");
    openSerialPort();

}
void MainWindow::openSerialPort()
{
    int a =0;
    serial->setPortName("COM5");
    serial->setBaudRate(QSerialPort::Baud115200);
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

void MainWindow::closeSerialPort()
{
    int a =0;
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
        console->appendPlainText(full_line);

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
        sendReadings(values[2]);
        //console->appendPlainText(QString::number(line_break_index));

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

