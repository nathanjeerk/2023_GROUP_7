//light
#include <vtkLight.h>
#include "VRRenderThread.h"

#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "optiondialog.h"
#include <vtkPlaneSource.h>
#include <vtkTextureMapToPlane.h>
#include <vtkTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkSphereSource.h>
#include <vtkTextureMapToSphere.h>



//for color pallete
#include <QColorDialog>
#include <QColor>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add button on main widget window
    connect( ui->resetModelView, &QPushButton::released, this, &MainWindow::handleResetModelView);
    connect( ui->changeModelColour, &QPushButton::released, this, &MainWindow::handleModelColorChange);
    connect(ui->toggleVR, &QPushButton::released, this, &MainWindow::handleStartVR);
    //status bar signal
    connect( this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage );
    //connect background button
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::changeBackground);
    //handle tree when clicked
    connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    //add action to right click treeview
    ui->treeView->addAction(ui->actionItem_Options);

    //Initialises ModelPartList and link to treeView
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);
    ModelPart *rootItem = this->partList->getRootItem();

    //add top 3 level item
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        ModelPart *childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);

        /*for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart *childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }*/


    }

    //link render to qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    //add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    light = vtkSmartPointer<vtkLight>::New();
    renderer->AddLight(light);
    light->SetIntensity(0.5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//handle for reset model view button
void MainWindow::handleResetModelView() {
    QMessageBox msgBox;
    msgBox.setText("Model Reset successfully");
    msgBox.exec();
    //status bar
    emit statusUpdateMessage( QString("Reset Model View was clicked"), 0);

    //reset camera
    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
}

//handle for model color changes
void MainWindow::handleModelColorChange() {

    emit statusUpdateMessage(QString("Model Color Changing"), 0);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if (part == nullptr) {
        return;
    }

    // Get the current palette from application or widget
    QPalette palette = this->palette();

    // Get the color role from the palette
    QColor color = palette.color(QPalette::WindowText);

    // Show color dialog with the obtained color value
    QColor ColorValue = QColorDialog::getColor(color, this, tr("Select color: "));
    qDebug() << ColorValue;

    if (ColorValue.isValid()) {
        // You can set the color of the part using the color value obtained from the color dialog
        part->setColour(ColorValue.red(), ColorValue.green(), ColorValue.blue()); // Set RGB values
        updateRender();
        emit statusUpdateMessage(QString("Model Color Change accepted"), 0);
    } else {
        emit statusUpdateMessage(QString("Model Color Change rejected"), 0);
    }
}

void MainWindow::handleStartVR() {
    //QModelIndex index = ui->treeView->currentIndex();
    //ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    vrThread = new VRRenderThread(this);
   /* auto actor = selectedPart->getNewActor();
    if (actor == nullptr) {
        qDebug() << "Actor is null";
        return;
    }

    vrThread->addActorOffline(actor);

    */

    updateVRRenderFromTree(partList->index(0, 0, QModelIndex()));

    vrThread->start();

    emit statusUpdateMessage(QString("VR LOADING.."), 0);
}

void MainWindow::updateVRRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        vtkSmartPointer<vtkActor> actor = selectedPart->getNewActor();
        if (actor != nullptr && selectedPart->visible()) {
            vrThread->addActorOffline(actor);

        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateVRRenderFromTree(partList->index(i, 0, index));
    }
}

void MainWindow::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    //get name string from internal QVariant data array
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}


//open file
void MainWindow::on_actionOpen_File_triggered()
{
    // Check if working
    emit statusUpdateMessage(QString("Open File action triggered"), 0);

    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)")
    );

    //logic
    if (!fileNames.isEmpty()) {
        for (const QString& fileName : fileNames) {
            emit statusUpdateMessage(QString("File " + fileName + " was opened"), 0);

            QFileInfo fileInfo(fileName);

            QModelIndex index = ui->treeView->currentIndex();
            QModelIndex part = partList->appendChild(index, { fileInfo.fileName(), QString("true") });

            ModelPart* viewPart = static_cast<ModelPart*>(part.internalPointer());
            viewPart->loadSTL(fileName);
        }

        updateRender();
    }
}

//update render from treeView
void MainWindow::updateRenderFromTree( const QModelIndex& index ) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor != nullptr && selectedPart->visible()) {
            renderer->AddActor(actor);

        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

//update the render
void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    for (int i = 0; i < partList->rowCount(QModelIndex()); i++){
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }
    renderer->Render();

    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
}

//right click option triggered handle
void MainWindow::on_actionItem_Options_triggered() {
    
    ui->treeView->addAction(ui->actionItem_Options);
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if (part == nullptr) {
        return;
    }

    OptionDialog dialog(this);
    struct DialogData MenuData;
    MenuData.R = part->getColourR();
    MenuData.G = part->getColourG();
    MenuData.B = part->getColourB();
    MenuData.name = part->data(0).toString();
    MenuData.isVisible = part->data(1).toBool();

    dialog.setMenuData(MenuData);


    if (dialog.exec() == QDialog::Accepted) {
        struct DialogData colour = dialog.getMenuData();
        part->setColour(colour.R, colour.G, colour.B);
        part->set(0, colour.name);
        part->set(1, QVariant(colour.isVisible).toString());
        part->setVisible(colour.isVisible);

        //updateRender();
        emit statusUpdateMessage(QString("Dialog accepted"), 0);
    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

//save file function
void MainWindow::on_actionSave_triggered()
{   
    emit statusUpdateMessage("Save As action Triggered", 0);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));
    if (!fileName.isEmpty()) {
        // Saving logic
        emit statusUpdateMessage("File " + fileName + " was saved", 0);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Write to the file
            QTextStream out(&file);
            out << "test";
            file.close();
        }
        else {
            // Handle error if cannot open file for writing
            emit statusUpdateMessage("Error: Couldn't save the file", 1);
        }
    }
}

//open directory function
void MainWindow::on_actionOpen_Directory_triggered()
{
    // Check if working
    emit statusUpdateMessage("Open Directory action triggered", 0);

    QString directory = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "C:\\",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!directory.isEmpty()) {
        emit statusUpdateMessage("Directory " + directory + " was opened", 0);

        QDir dir(directory);
        QStringList filters;
        filters << "*.stl"; // Add more extensions if needed

        QStringList fileList = dir.entryList(filters, QDir::Files);

        foreach(QString fileName, fileList) {
            QString filePath = dir.filePath(fileName);

            // Add file to the treeView
            QModelIndex index = ui->treeView->currentIndex();
            QModelIndex part = partList->appendChild(index, { fileName, QString("true") });

            ModelPart* viewPart = static_cast<ModelPart*>(part.internalPointer());
            viewPart->loadSTL(filePath);
        }

        updateRender();
    }
}

//light intensity
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    double intense = (value / 100.0);
    light->SetIntensity(intense);

    emit statusUpdateMessage(QString("Adjusting light intensity"), 0);
}

//Background changing
void MainWindow::changeBackground() {
    emit statusUpdateMessage(QString("Changing background"), 0);

    // Open file dialog to select an image
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg)"));

    emit statusUpdateMessage(QString("File " + imagePath + " was changed to background"), 0);

    if (!imagePath.isEmpty()) {
        // Load the image using VTK's image reader
        vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
        vtkSmartPointer<vtkImageReader2> reader = readerFactory->CreateImageReader2(imagePath.toStdString().c_str());
        if (!reader) {
            qDebug() << "Failed to load image.";
            return;
        }
        reader->SetFileName(imagePath.toStdString().c_str());
        reader->Update();

        // Create a texture from the image
        vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
        texture->SetInputConnection(reader->GetOutputPort());

        // Create a sphere geometry
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetCenter(0, 0, 0);
        sphereSource->SetRadius(100000); // Adjust radius as needed
        sphereSource->SetPhiResolution(100); // Adjust resolution for smoother surface
        sphereSource->SetThetaResolution(100);
        sphereSource->Update();

        // Map the texture onto the sphere
        vtkSmartPointer<vtkTextureMapToSphere> mapToSphere = vtkSmartPointer<vtkTextureMapToSphere>::New();
        mapToSphere->SetInputConnection(sphereSource->GetOutputPort());

        // Create a sphere actor
        vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        sphereMapper->SetInputConnection(mapToSphere->GetOutputPort());
        vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
        sphereActor->SetMapper(sphereMapper);
        sphereActor->SetTexture(texture);

        // Rotate the sphere 180 degrees around the y-axis
        sphereActor->RotateY(180.0);

        // Add the actor to the renderer
        renderer->AddActor(sphereActor);

        // Set the material properties of the sphere actor to not receive lighting
        vtkSmartPointer<vtkProperty> sphereProperty = sphereActor->GetProperty();
        sphereProperty->SetAmbient(1.0);
        sphereProperty->SetDiffuse(0.0);
        sphereProperty->SetSpecular(0.0);

        // Adjust camera clipping range to ensure the background sphere is always visible
        renderer->ResetCameraClippingRange();

        // Update the rendering window
        renderWindow->Render();
    }
}