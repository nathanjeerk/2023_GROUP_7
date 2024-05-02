#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"


#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKRenderWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkWidgetSet.h>
#include <vtkCamera.h>
#include <vtkProperty.h>

#include <vtkLight.h>

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

    ModelPartList* partList;

public slots:
    void handleButton();

    void handleButton2();

    void handleButton3();

    void handleTreeClicked();

    void on_actionItem_Options_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_Directory_triggered();

    void updateRender();
    
    void updateRenderFromTree(const QModelIndex& index);

    void on_horizontalSlider_valueChanged(int value);

    void changeBackground();
signals:
    void statusUpdateMessage( const QString & message, int timeout );

private slots:
    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow *ui;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkSmartPointer<vtkLight> light;
};
#endif // MAINWINDOW_H
