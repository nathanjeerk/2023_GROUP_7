#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include "optiondialog.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>

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

public slots:
    void handleButton();
    void handleButton_2();
    void handleTreeClicked();



signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow* ui;
    ModelPartList* partList;
    //vtk
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    void updateChildren(ModelPart* parent, int red, int green, int blue);

};
#endif // MAINWINDOW_H