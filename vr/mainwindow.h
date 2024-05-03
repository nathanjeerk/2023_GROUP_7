#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include "VRRenderThread.h"


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

/** @file mainwindow.h
* This file contains the declarations of all exported functions in vtk libraries.
*/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    ModelPartList* partList;


public slots:
    /**This function handle resetting the view of model in graphical application.**/
    void handleResetModelView();

    /**This function handle the function where it change the model colors**/
    void handleModelColorChange();

    /**This function handle the function where it starts thread for VR**/
    void handleStartVR();

    /**this function handles the action of clicking on an item in a tree view**/
    void handleTreeClicked();

    /**this function handles the action triggered by selecting "Item Options" from a menu**/
    void on_actionItem_Options_triggered();

    /**this function handles the action of saving a file**/
    void on_actionSave_triggered();

    /**this function handles the action of open directory**/
    void on_actionOpen_Directory_triggered();

    /**this function refreshes the rendering of the scene, clears existing view props and updates the scene.**/
    void updateRender();

    /**this function recursively updates the rendering of the scene based on the hierarchical structure.
    * @param index is the first parameter
    * @return updateRender from tree
    */
    void updateRenderFromTree(const QModelIndex& index);

    /**this function handle the light intensity options
    * @param value is the first parameter
    */
    void on_horizontalSlider_valueChanged(int value);

    /**this function handle when change the background images**/
    void changeBackground();

    /*asdasd*/
    void updateVRRenderFromTree(const QModelIndex& index);

signals:
    /**this function facilitates the emission of a status update message signal.
    * @param message is the first parameter
    * @param timeout is the first parameter
    */
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    /**this function handles the action of opening one or multiple files**/
    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow* ui;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkSmartPointer<vtkLight> light;
    VRRenderThread* vrThread;
};
#endif // MAINWINDOW_H
