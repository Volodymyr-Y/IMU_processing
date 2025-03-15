#ifndef MY3DWINDOW_H
#define MY3DWINDOW_H

#include <QWidget>
#include <QObject>
#include <Qt3DCore/QEntity>
#include <Qt3DCore>
class My3DWindow : public QWidget
{
    Q_OBJECT

private:
    QWidget *container;
    Qt3DCore::QEntity *CubeObject;
    Qt3DCore::QTransform *CuboidTransform;
public:
    explicit My3DWindow(QWidget *parent = nullptr);

protected:
    // reimplementation needed to handle resize events
    // http://doc.qt.io/qt-5/qwidget.html#resizeEvent
    void
    resizeEvent ( QResizeEvent * event );

public slots:
    void resizeView(QSize size);
    void updateAttitude(float x);
};


#endif // MY3DWINDOW_H
