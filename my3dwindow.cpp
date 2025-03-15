#include "my3dwindow.h"
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <QQuaternion>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>


My3DWindow::My3DWindow(QWidget *parent)
    : QWidget{parent}
{
    auto view = new Qt3DExtras::Qt3DWindow();

    // create a container for Qt3DWindow
    container = createWindowContainer(view,this);

    // background color
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x575757)));

    // Root entity
    auto rootEntity = new Qt3DCore::QEntity();

    // Camera
    auto cameraEntity = view->camera();

    cameraEntity->setPosition(QVector3D(0, 0, 50.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    // Cuboid
    auto cuboidMesh = new Qt3DExtras::QCuboidMesh();

    // CuboidMesh Transform
    CuboidTransform = new Qt3DCore::QTransform();
    CuboidTransform->setScale(10.0f);
    CuboidTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    CuboidTransform->setRotation(QQuaternion(1,1.5,1,0).normalized());

    auto cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x005FFF)));

    // assamble entity
    auto CubeObject = new Qt3DCore::QEntity(rootEntity);
    CubeObject->addComponent(cuboidMesh);
    CubeObject->addComponent(cuboidMaterial);
    CubeObject->addComponent(CuboidTransform);
    // Set root object of the scene
    view->setRootEntity(rootEntity);
}

void
My3DWindow::resizeView(QSize size)
{
    container->resize(size);
}

void My3DWindow::resizeEvent ( QResizeEvent * /*event*/ )
{
    resizeView(this->size());
}

void My3DWindow::updateAttitude(float x)
{
    CuboidTransform->setRotation(QQuaternion(1,1,x,1).normalized());
}
