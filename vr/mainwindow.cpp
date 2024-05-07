// Include the necessary libraries
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

// For color palette
#include <QColorDialog>
#include <QColor>
#include <QPalette>

//for filters
/*
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <vtkDataSetMapper.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>
#include <vtkPlane.h>
#include <vtkActor.h>
*/

/**
 * @file mainwindow.h
 * @brief This file contains the declarations of all exported functions in vtk libraries.
 */

/**
 * @class MainWindow
 * @brief The MainWindow class inherits from QMainWindow and represents the main window of the application.
 *
 * @param parent is a pointer to the widget that is logically the parent of this window. It is passed to the QMainWindow constructor.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add button on main widget window
    connect( ui->resetModelView, &QPushButton::released, this, &MainWindow::handleResetModelView);
    connect( ui->changeModelColour, &QPushButton::released, this, &MainWindow::handleModelColorChange);
    connect(ui->toggleVR, &QPushButton::released, this, &MainWindow::handleStartVR);
    // Status bar signal
    connect( this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage );
    // Connect background button
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::changeBackground);
    // Handle tree when clicked
    connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    // Add action to right click treeview
    ui->treeView->addAction(ui->actionItem_Options);

    // Initialises ModelPartList and link to treeView
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);
    ModelPart *rootItem = this->partList->getRootItem();

    // Add top 3 level item
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        ModelPart *childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);
    }

    // Link render to qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    light = vtkSmartPointer<vtkLight>::New();
    renderer->AddLight(light);
    light->SetIntensity(0.5);
}

/**
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief This function handles resetting the view of the model in the graphical application.
 */
void MainWindow::handleResetModelView() {
    QMessageBox msgBox;
    msgBox.setText("Model Reset successfully");
    msgBox.exec();
    // Status bar
    emit statusUpdateMessage( QString("Reset Model View was clicked"), 0);

    // Reset camera
    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
}

/**
 * @brief This function handles changing the model colors.
 */
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

/**
 * @brief This function handles starting the VR thread.
 */
void MainWindow::handleStartVR() {
    vrThread = new VRRenderThread(this);
    updateVRRenderFromTree(partList->index(0, 0, QModelIndex()));
    vrThread->start();
    emit statusUpdateMessage(QString("VR LOADING.."), 0);
}

/**
 * @brief This function updates the VR rendering from the tree.
 *
 * @param index is the index of the item in the tree view.
 */
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

/**
 * @brief This function handles the action of clicking on an item in a tree view.
 */
void MainWindow::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    // Get name string from internal QVariant data array
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}


/**
 * @brief This function handles the action of opening one or multiple files.
 */
void MainWindow::on_actionOpen_File_triggered()
{
    // Emit status update message
    emit statusUpdateMessage(QString("Open File action triggered"), 0);

    // Open file dialog to select one or multiple STL or TXT files
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)")
    );

    // If files are selected
    if (!fileNames.isEmpty()) {
        // For each selected file
        for (const QString& fileName : fileNames) {
            // Emit status update message
            emit statusUpdateMessage(QString("File " + fileName + " was opened"), 0);

            // Get file info
            QFileInfo fileInfo(fileName);

            // Get current index in the tree view
            QModelIndex index = ui->treeView->currentIndex();
            // Append child to the part list
            QModelIndex part = partList->appendChild(index, { fileInfo.fileName(), QString("true") });

            // Get pointer to the model part
            ModelPart* viewPart = static_cast<ModelPart*>(part.internalPointer());
            // Load STL file
            viewPart->loadSTL(fileName);
        }

        // Update render
        updateRender();
    }
}

/**
 * @brief This function updates the render from the tree view.
 *
 * @param index is the index of the item in the tree view.
 */
void MainWindow::updateRenderFromTree( const QModelIndex& index ) {
    // If index is valid
    if (index.isValid()) {
        // Get pointer to the selected model part
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        // Get actor of the selected model part
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        // If actor is not null and selected part is visible
        if (actor != nullptr && selectedPart->visible()) {
            // Add actor to the renderer
            renderer->AddActor(actor);
        }
    }

    // If part list has no children or index flags indicate that item never has children
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    // Get row count of the part list
    int rows = partList->rowCount(index);
    // For each row
    for (int i = 0; i < rows; i++) {
        // Update render from the tree
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

/**
 * @brief This function updates the render.
 */
void MainWindow::updateRender() {
    // Remove all view props from the renderer
    renderer->RemoveAllViewProps();
    // For each row in the part list
    for (int i = 0; i < partList->rowCount(QModelIndex()); i++){
        // Update render from the tree
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }
    // Render
    renderer->Render();

    // Reset camera and camera clipping range
    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
}

/**
 * @brief This function handles the action triggered by selecting "Item Options" from a menu.
 */
void MainWindow::on_actionItem_Options_triggered() {

    // Add action to the tree view
    ui->treeView->addAction(ui->actionItem_Options);
    // Get current index in the tree view
    QModelIndex index = ui->treeView->currentIndex();
    // Get pointer to the model part
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    // If part is null, return
    if (part == nullptr) {
        return;
    }

    // Create option dialog
    OptionDialog dialog(this);
    // Create dialog data
    struct DialogData MenuData;
    // Set dialog data
    MenuData.R = part->getColourR();
    MenuData.G = part->getColourG();
    MenuData.B = part->getColourB();
    MenuData.name = part->data(0).toString();
    MenuData.isVisible = part->data(1).toBool();

    // Set menu data to the dialog
    dialog.setMenuData(MenuData);

    // If dialog is accepted
    if (dialog.exec() == QDialog::Accepted) {
        // Get menu data from the dialog
        struct DialogData colour = dialog.getMenuData();
        // Set colour to the part
        part->setColour(colour.R, colour.G, colour.B);
        // Set name and visibility to the part
        part->set(0, colour.name);
        part->set(1, QVariant(colour.isVisible).toString());
        // Set visibility to the part
        part->setVisible(colour.isVisible);

        // Update render
        //updateRender();
        // Emit status update message
        emit statusUpdateMessage(QString("Dialog accepted"), 0);
    }
    else {
        // Emit status update message
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

/**
 * @brief This function handles the action of saving a file.
 */
void MainWindow::on_actionSave_triggered()
{
    // Emit status update message
    emit statusUpdateMessage("Save As action Triggered", 0);
    // Open save file dialog to select a STL or TXT file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));
    // If file name is not empty
    if (!fileName.isEmpty()) {
        // Saving logic
        emit statusUpdateMessage("File " + fileName + " was saved", 0);
        // Open file for writing
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

/**
 * @brief This function handles the action of opening a directory.
 */
void MainWindow::on_actionOpen_Directory_triggered()
{
    // Emit status update message
    emit statusUpdateMessage("Open Directory action triggered", 0);

    // Open directory dialog to select a directory
    QString directory = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "C:\\",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    // If directory is not empty
    if (!directory.isEmpty()) {
        // Emit status update message
        emit statusUpdateMessage("Directory " + directory + " was opened", 0);

        // Open directory
        QDir dir(directory);
        // Set filters to select STL files
        QStringList filters;
        filters << "*.stl"; // Add more extensions if needed

        // Get list of files in the directory
        QStringList fileList = dir.entryList(filters, QDir::Files);

        // For each file in the list
        foreach(QString fileName, fileList) {
            // Get file path
            QString filePath = dir.filePath(fileName);

            // Add file to the tree view
            QModelIndex index = ui->treeView->currentIndex();
            QModelIndex part = partList->appendChild(index, { fileName, QString("true") });

            // Get pointer to the model part
            ModelPart* viewPart = static_cast<ModelPart*>(part.internalPointer());
            // Load STL file
            viewPart->loadSTL(filePath);
        }

        // Update render
        updateRender();
    }
}

/**
 * @brief This function handles the change of light intensity.
 *
 * @param value is the new light intensity value.
 */
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // Get current index in the tree view
    QModelIndex index = ui->treeView->currentIndex();
    // Get pointer to the selected model part
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    // Calculate light intensity
    double intense = (value / 100.0);
    // Set light intensity
    light->SetIntensity(intense);

    // Emit status update message
    emit statusUpdateMessage(QString("Adjusting light intensity"), 0);
}

/**
 * @brief This function handles changing the background.
 */
void MainWindow::changeBackground() {
    // Emit status update message
    emit statusUpdateMessage(QString("Changing background"), 0);

    // Open file dialog to select an image
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg)"));

    // Emit status update message
    emit statusUpdateMessage(QString("File " + imagePath + " was changed to background"), 0);

    // If image path is not empty
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

//for filters
/*
void MainWindow::on_checkBox_stateChanged(int arg1){
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if(arg1){
        emit statusUpdateMessage(QString("Clip filter applier"),0);
        isClippingApplied = 1;
        if (selectedPart != nullptr)
            if (selectedPart->getSource()!=nullptr)
                applyPartFilters();
    }
    else{
        emit statusUpdateMessage(QString("Clip filter not applier"),0);
        isClippingApplied = 0;
        if (selectedPart != nullptr)
            if (selectedPart->getSource()!=nullptr)
                applyPartFilters();
    }
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if(arg1){
        emit statusUpdateMessage(QString("Shrink filter applier"),0);
        isShrinkApplied = 1;
        if (selectedPart != nullptr)
            if (selectedPart->getSource()!=nullptr)
                applyPartFilters();
    }
    else{
        emit statusUpdateMessage(QString("Shrink filter not applier"),0);
        if (selectedPart != nullptr)
            if (selectedPart->getSource()!=nullptr)
                applyPartFilters();
    }
}

void MainWindow::applyPartFilters()
{
    vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (isClippingApplied && isShrinkApplied)
    {
        // This will apply a clipping plane whose normal is the x-axis that crosses the x-axis at x=0
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(-1.0, 0.0, 0.0);
        //clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(selectedPart->getSource()->GetOutputPort());
        clipFilter->SetClipFunction(planeLeft.Get());

        // Create a vtkShrinkFilter object
        //shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        // Set the input connection for the shrink filter
        shrinkFilter->SetInputConnection(clipFilter->GetOutputPort());
        // Set the shrink factor (0.8 in this case, meaning 20% shrinkage)
        shrinkFilter->SetShrinkFactor(0.8);
        // Update the shrink filter to process the input data
        shrinkFilter->Update();

        selectedPart->getMapper()->SetInputConnection(shrinkFilter->GetOutputPort());
    }
    else if (isClippingApplied)
    {
        // This will apply a clipping plane whose normal is the x-axis that crosses the x-axis at x=0
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(-1.0, 0.0, 0.0);
        //clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(selectedPart->getSource()->GetOutputPort());
        clipFilter->SetClipFunction(planeLeft.Get());
        selectedPart->getMapper()->SetInputConnection(clipFilter->GetOutputPort());

    }
    else if (isShrinkApplied)
    {
        // Create a vtkShrinkFilter object
        //shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        // Set the input connection for the shrink filter
        shrinkFilter->SetInputConnection(selectedPart->getSource()->GetOutputPort());
        // Set the shrink factor (0.8 in this case, meaning 20% shrinkage)
        shrinkFilter->SetShrinkFactor(0.8);
        // Update the shrink filter to process the input data
        shrinkFilter->Update();

        selectedPart->getMapper()->SetInputConnection(shrinkFilter->GetOutputPort());
    }
    else
    {
        selectedPart->getMapper()->SetInputConnection(selectedPart->getSource()->GetOutputPort());
    }


    selectedPart->getActor()->SetMapper(selectedPart->getMapper());
    selectedPart->getActor()->GetProperty()->SetColor(1., 0., 0.35);
    //renderer->AddActor(selectedPart->getActor());
    renderer->RemoveAllViewProps();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(selectedPart->getMapper());
    actor->GetProperty()->SetColor(1., 0., 0.35);
    renderer->AddActor(actor);


    //updateRender();


    // updateRender();
    renderer->GetRenderWindow()->Render();
}
*/
