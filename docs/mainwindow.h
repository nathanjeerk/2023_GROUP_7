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

/**
 * @file mainwindow.h
 * @brief This file contains the declarations of all exported functions in vtk libraries.
 */

QT_BEGIN_NAMESPACE
namespace Ui {
/**
 * @namespace Ui
 * @brief The Ui namespace contains all the UI classes used in the application.
 */

/**
 * @class MainWindow
 * @brief The MainWindow class is forward declared here. It is defined in the Ui namespace.
 */
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The MainWindow class inherits from QMainWindow and represents the main window of the application.
 * 
 * @param parent is a pointer to the widget that is logically the parent of this window. It is passed to the QMainWindow constructor.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow class.
     * 
     * @param parent is a pointer to the widget that is logically the parent of this window. It is passed to the QMainWindow constructor.
     */
    MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor for the MainWindow class.
     */
    ~MainWindow();

    /**
     * @brief A pointer to the list of model parts.
     */
    ModelPartList* partList;

public slots:
    /**
     * @brief This function handles resetting the view of the model in the graphical application.
     */
    void handleResetModelView();

    /**
     * @brief This function handles changing the model colors.
     */
    void handleModelColorChange();

    /**
     * @brief This function handles starting the VR thread.
     */
    void handleStartVR();

    /**
     * @brief This function handles the action of clicking on an item in a tree view.
     */
    void handleTreeClicked();

    /**
     * @brief This function handles the action triggered by selecting "Item Options" from a menu.
     */
    void on_actionItem_Options_triggered();

    /**
     * @brief This function handles the action of saving a file.
     */
    void on_actionSave_triggered();

    /**
     * @brief This function handles the action of opening a directory.
     */
    void on_actionOpen_Directory_triggered();

    /**
     * @brief This function refreshes the rendering of the scene, clears existing view props and updates the scene.
     */
    void updateRender();

    /**
     * @brief This function recursively updates the rendering of the scene based on the hierarchical structure.
     * 
     * @param index is the index of the item in the tree view.
     */
    void updateRenderFromTree(const QModelIndex& index);

    /**
     * @brief This function handles the light intensity options.
     * 
     * @param value is the new light intensity value.
     */
    void on_horizontalSlider_valueChanged(int value);

    /**
     * @brief This function handles changing the background images.
     */
    void changeBackground();

    /**
     * @brief This function updates the VR rendering from the tree.
     * 
     * @param index is the index of the item in the tree view.
     */
    void updateVRRenderFromTree(const QModelIndex& index);

signals:
    /**
     * @brief This function facilitates the emission of a status update message signal.
     * 
     * @param message is the status update message.
     * @param timeout is the duration for which the message should be displayed.
     */
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    /**
     * @brief This function handles the action of opening one or multiple files.
     */
    void on_actionOpen_File_triggered();

private:
    /**
     * @brief A pointer to the UI of the MainWindow class.
     */
    Ui::MainWindow* ui;

    /**
     * @brief A smart pointer to the renderer.
     */
    vtkSmartPointer<vtkRenderer> renderer;

    /**
     * @brief A smart pointer to the render window.
     */
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    /**
     * @brief A smart pointer to the light.
     */
    vtkSmartPointer<vtkLight> light;

    /**
     * @brief A pointer to the VR render thread.
     */
    VRRenderThread* vrThread;
};

#endif // MAINWINDOW_H
