#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "optiondialog.h"
#include <QFileDialog>
#include <QLineEdit>

#include <vtkCylinderSource.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //render with Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    //add a render
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    //define cylinder
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    //create object and add to renderer
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    //set color and rotate it around
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);
    //reset camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();



    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton_2);
    /*tree view*/
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    ui->treeView->addAction(ui->actionItem_Options);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);

    /*create or allocate the model list*/
    this->partList = new ModelPartList("PartsList");
    /*link to treeview in GUI*/
    ui->treeView->setModel(this->partList);
    /*manually create a model*/
    ModelPart* rootItem = this->partList->getRootItem();

    /*add top 3 lvl items*/
    for (int i = 0; i < 3; i++) {
        /*create string for both data column*/
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        /*create child item*/
        ModelPart* childItem = new ModelPart({ name, visible });
        /*append to top three*/
        rootItem->appendChild(childItem);
        /*add 5 sub-item*/
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name, visible });

            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton() {
    QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    msgBox.exec();

    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void MainWindow::handleButton_2() {
    OptionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name_text = dialog.getText();
        emit statusUpdateMessage(QString("Dialog accepted: " + name_text), 0);
    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    OptionDialog dialog(this);

    emit statusUpdateMessage(QString("Open File action triggered"), 0);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt"));
    emit statusUpdateMessage(QString(fileName), 0);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* Part = static_cast<ModelPart*>(index.internalPointer());

    QString visible("true");

    //create new model part
    ModelPart* newPart = new ModelPart({ fileName });

    //add child
    Part->appendChild(newPart);

    //load STL
    newPart->loadSTL(fileName);

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (selectedPart != nullptr) {
        selectedPart->appendChild(newPart);
    }
    else {
        this->partList->getRootItem()->appendChild(newPart);
    }
    updateRender();

    //File name display
    dialog.setName(fileName);
    selectedPart->set(0, fileName);
}

void MainWindow::on_actionItem_Options_triggered()
{
    OptionDialog dialog(this);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();
    QString visibility = selectedPart->data(1).toString();

    bool isVisible = (visibility.toLower() == "true");

    int red = selectedPart->getColourR();
    int green = selectedPart->getColourG();
    int blue = selectedPart->getColourB();

    dialog.setName(text);
    dialog.setVisible(visibility);
    dialog.setRGB(red, green, blue);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getText();
        QString visible = QString::number(dialog.isVisible());

        int set_red = dialog.red();
        int set_green = dialog.green();
        int set_blue = dialog.blue();

        selectedPart->set(0, name);
        selectedPart->setColour(set_red, set_green, set_blue);
        selectedPart->setVisible(isVisible);

        if (visible == "1") {
            selectedPart->set(1, "true");
        }
        else {
            selectedPart->set(1, "false");
        }

        emit statusUpdateMessage(QString("Item Option: submitted"), 0);

        updateRender();

        updateChildren(selectedPart, set_red, set_green, set_blue);
    }
    else {
        emit statusUpdateMessage(QString("Item Option: rejected"), 0);
    }
}


void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        /* Retrieve actor from selected part and add to renderer */
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        auto actor = selectedPart->getActor();
        if (actor != nullptr) {
            renderer->AddActor(actor);
        }
    }

    /* Check to see if this part has any children */
    if (!partList->hasChildren(index) || (index.flags() & Qt::
        ItemNeverHasChildren)) {
        return;
    }

    /* Loop through children and add their actors */
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

void MainWindow::updateChildren(ModelPart* parent, int red, int green, int blue) {
    //loop through element
    for (int i = 0; i < parent->childCount(); ++i) {
        ModelPart* child = parent->child(i);

        child->setColour(red, green, blue);

        updateChildren(child, red, green, blue);
    }
}