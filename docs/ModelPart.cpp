/** @file ModelPart.cpp
  * @brief EEEE2076 - Software Engineering & VR Project
  * Template for model parts that will be added as treeview items
  * P Evans 2022
  */
#include "ModelPart.h"

/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>

/**
 * @brief Constructor for the ModelPart class.
 * @param data is a list of QVariant items that represent the data of the model part.
 * @param parent is a pointer to the parent ModelPart item.
 */
ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {
    /* You probably want to give the item a default colour */
}

/**
 * @brief Destructor for the ModelPart class.
 */
ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

/**
 * @brief This function appends a child item to this model part.
 * @param item is a pointer to the child ModelPart item.
 */
void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}

/**
 * @brief This function returns a pointer to the child item in the specified row.
 * @param row is the row index of the child item.
 * @return a pointer to the child ModelPart item.
 */
ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

/**
 * @brief This function returns the number of child items.
 * @return the number of child items.
 */
int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}

/**
 * @brief This function returns the number of columns (properties) that this item has.
 * @return the number of columns (properties).
 */
int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

/**
 * @brief This function returns the data associated with a column of this item.
 * @param column is the column index of the item data.
 * @return the data associated with the column.
 */
QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

/**
 * @brief This function sets the data associated with a column of this item.
 * @param column is the column index of the item data.
 * @param value is the new value of the item data.
 */
void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

/**
 * @brief This function returns a pointer to the parent item.
 * @return a pointer to the parent ModelPart item.
 */
ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

/**
 * @brief This function returns the row index of this item, relative to its parent.
 * @return the row index of this item.
 */
int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

/**
 * @brief This function sets the color of the model part.
 * @param R is the red component of the color.
 * @param G is the green component of the color.
 * @param B is the blue component of the color.
 */
void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    colour.Set(R, G, B);
    if (actor != nullptr) {
        actor->GetProperty()->SetColor(((double)R / 255.), ((double)G / 255.), ((double)B / 255.));
    }
}

/**
 * @brief This function returns the red component of the color of the model part.
 * @return the red component of the color.
 */
unsigned char ModelPart::getColourR() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetRed();   // needs updating
}

/**
 * @brief This function returns the green component of the color of the model part.
 * @return the green component of the color.
 */
unsigned char ModelPart::getColourG() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetGreen();   // needs updating
}

/**
 * @brief This function returns the blue component of the color of the model part.
 * @return the blue component of the color.
 */
unsigned char ModelPart::getColourB() {
   /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetBlue();   // needs updating
}

/**
 * @brief This function sets the visibility of the model part.
 * @param isVisible is a boolean value that represents the visibility of the model part.
 */
void ModelPart::setVisible(bool isVisible) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    //this->isVisible = isVisible;
    actor->SetVisibility(isVisible);
    /* As the name suggests ... */
}

/**
 * @brief This function returns the visibility of the model part.
 * @return a boolean value that represents the visibility of the model part.
 */
bool ModelPart::visible() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return isVisible;
}

/**
 * @brief This function loads an STL file.
 * @param fileName is the name of the STL file.
 */
void ModelPart::loadSTL( QString fileName ) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* 1. Use the vtkSTLReader class to load the STL file 
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();
    /* 2. Initialise the part's vtkMapper */
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());

    /* 3. Initialise the part's vtkActor and link to the mapper */
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
}

/**
 * @brief This function returns a smart pointer to the vtkActor to allow part to be rendered.
 * @return a smart pointer to the vtkActor.
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
    return actor;
}

/**
 * @brief This function returns a new actor for the model part.
 * @return a pointer to the new vtkActor.
 */
vtkActor* ModelPart::getNewActor() {
    auto vrMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    if (file == nullptr) {
        qDebug() << "File render is null, aborting";
        return nullptr;
    }

    vrMapper->SetInputConnection(file->GetOutputPort());
    vtkActor* newActor = vtkActor::New();
    newActor->SetMapper(vrMapper);
    newActor->SetProperty(actor->GetProperty());
    return newActor;
}
