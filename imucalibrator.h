#ifndef IMUCALIBRATOR_H
#define IMUCALIBRATOR_H

//camelCase convention
#include <QObject>

enum class CalibratorState {
    Off,  // 0
    Gathering,  // 1
};

struct CalibrationParameters {
    float Sx;
    float Sy;
    float Sz;
    float a12;
    float a13;
    float a21;
    float a23;
    float a31;
    float a32;
    float bx;
    float by;
    float bz;
};

class IMUCalibrator
{
    Q_OBJECT;
private:
    CalibratorState FStatus;
    void CalculateOffsets();

public:
    IMUCalibrator();

public slots:
    void RecordMeasurement(std::vector<float> readings);
};

#endif // IMUCALIBRATOR_H
