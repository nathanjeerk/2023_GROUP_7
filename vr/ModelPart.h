/** @file ModelPart.h
  * @brief EEEE2076 - Software Engineering & VR Project
  * Template for model parts that will be added as treeview items
  * P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

/* VTK headers - will be needed when VTK used in next worksheet,
 * commented out for now
 *
 * Note that there are a few function definitions and variables
 * commented out below - this is because you haven't yet installed
 * the VTK library which is needed.
 */
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

/**
 * @class ModelPart
 * @brief The ModelPart class represents a model part that will be added as a treeview item.
 */
class ModelPart {
public:
    /**
     * @brief Constructor for the ModelPart class.
     * @param data is a list of QVariant items that represent the data of the model part.
     * @param parent is a pointer to the parent ModelPart item.
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /**
     * @brief Destructor for the ModelPart class.
     */
    ~ModelPart();

    /**
     * @brief This function appends a child item to this model part.
     * @param item is a pointer to the child ModelPart item.
     */
    void appendChild(ModelPart* item);

    /**
     * @brief This function returns a pointer to the child item in the specified row.
     * @param row is the row index of the child item.
     * @return a pointer to the child ModelPart item.
     */
    ModelPart* child(int row);

    /**
     * @brief This function returns the number of child items.
     * @return the number of child items.
     */
    int childCount() const;

    /**
     * @brief This function returns the number of columns (properties) that this item has.
     * @return the number of columns (properties).
     */
    int columnCount() const;

    /**
     * @brief This function returns the data associated with a column of this item.
     * @param column is the column index of the item data.
     * @return the data associated with the column.
     */
    QVariant data(int column) const;

    /**
     * @brief This function sets the data associated with a column of this item.
     * @param column is the column index of the item data.
     * @param value is the new value of the item data.
     */
    void set( int column, const QVariant& value );

    /**
     * @brief This function returns a pointer to the parent item.
     * @return a pointer to the parent ModelPart item.
     */
    ModelPart* parentItem();

    /**
     * @brief This function returns the row index of this item, relative to its parent.
     * @return the row index of this item.
     */
    int row() const;

    /**
     * @brief This function sets the color of the model part.
     * @param R is the red component of the color.
     * @param G is the green component of the color.
     * @param B is the blue component of the color.
     */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    /**
     * @brief This function returns the red component of the color of the model part.
     * @return the red component of the color.
     */
    unsigned char getColourR();

    /**
     * @brief This function returns the green component of the color of the model part.
     * @return the green component of the color.
     */
    unsigned char getColourG();

    /**
     * @brief This function returns the blue component of the color of the model part.
     * @return the blue component of the color.
     */
    unsigned char getColourB();

    /**
     * @brief This function sets the visibility of the model part.
     * @param isVisible is a boolean value that represents the visibility of the model part.
     */
    void setVisible(bool isVisible);

    /**
     * @brief This function returns the visibility of the model part.
     * @return a boolean value that represents the visibility of the model part.
     */
    bool visible();
	
    /**
     * @brief This function loads an STL file.
     * @param fileName is the name of the STL file.
     */
    void loadSTL(QString fileName);

    /**
     * @brief This function returns a smart pointer to the vtkActor to allow part to be rendered.
     * @return a smart pointer to the vtkActor.
     */
    vtkSmartPointer<vtkActor> getActor();

    /**
     * @brief This function returns a new actor for the model part.
     * @return a pointer to the new vtkActor.
     */
    vtkActor* getNewActor();

private:
    QList<ModelPart*>                           m_childItems;       /**< List (array) of child items */
    QList<QVariant>                             m_itemData;         /**< List (array of column data for item */
    ModelPart*                                  m_parentItem;       /**< Pointer to parent */
    vtkSmartPointer<vtkPolyDataMapper> m_mapper;
    vtkSmartPointer<vtkActor> m_actor;
    /* These are some typical properties that I think the part will need, you might
     * want to add you own.
     */
    bool                                        isVisible;          /**< True/false to indicate if should be visible in model rendering */
	
	/* These are vtk properties that will be used to load/render a model of this part,
	 * commented out for now but will be used later
	 */
	vtkSmartPointer<vtkSTLReader>               file;               /**< Datafile from which part loaded */
    vtkSmartPointer<vtkMapper>                  mapper;             /**< Mapper for rendering */
    vtkSmartPointer<vtkActor>                   actor;              /**< Actor for rendering */
    vtkColor3<unsigned char>                    colour;             /**< User defineable colour */
};  


#endif
