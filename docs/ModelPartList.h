/** @file ModelPartList.h
  * @brief EEEE2076 - Software Engineering & VR Project
  * Template for model part list that will be used to create the treeview.
  * P Evans 2022
  */

#ifndef VIEWER_MODELPARTLIST_H
#define VIEWER_MODELPARTLIST_H

#include "ModelPart.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QList>

class ModelPart;

/**
 * @class ModelPartList
 * @brief The ModelPartList class represents a list of model parts that will be used to create the treeview.
 */
class ModelPartList : public QAbstractItemModel {
    Q_OBJECT        /**< A special Qt tag used to indicate that this is a special Qt class that might require preprocessing before compiling. */
public:
    /**
     * @brief Constructor for the ModelPartList class.
     * @param data is not used.
     * @param parent is used by the parent class constructor.
     */
    ModelPartList( const QString& data, QObject* parent = NULL );

    /**
     * @brief Destructor for the ModelPartList class.
     */
    ~ModelPartList();

    /**
     * @brief This function returns the number of columns in the tree view.
     * @param parent is not used.
     * @return the number of columns in the tree view.
     */
    int columnCount( const QModelIndex& parent ) const;

    /**
     * @brief This function returns the value of a particular row and columns.
     * @param index in a structure Qt uses to specify the row and column it wants data for.
     * @param role is how Qt specifies what it wants to do with the data.
     * @return a QVariant which is a generic variable used to represent any Qt class type.
     */
    QVariant data( const QModelIndex& index, int role ) const;

        /**
     * @brief This function returns a Qt item flags.
     * @param index in a structure Qt uses to specify the row and column it wants data for.
     * @return a Qt item flags.
     */
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    /**
     * @brief This function returns the header data.
     * @param section is the section of the header.
     * @param orientation is the orientation of the header.
     * @param role is the role of the header.
     * @return the header data.
     */
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    /**
     * @brief This function returns a valid QModelIndex for a location in the tree.
     * @param row is the row index of the item.
     * @param column is the column index of the item.
     * @param parent is the parent of the item.
     * @return the QModelIndex structure.
     */
    QModelIndex index( int row, int column, const QModelIndex& parent ) const;

    /**
     * @brief This function returns a QModelIndex for the parent of the item.
     * @param index of the item.
     * @return index of the parent item.
     */
    QModelIndex parent( const QModelIndex& index ) const;

    /**
     * @brief This function returns the number of rows (items) under an item in the tree.
     * @param parent is the parent of the item.
     * @return the number of child items.
     */
    int rowCount( const QModelIndex& parent ) const;

    /**
     * @brief This function returns a pointer to the root item of the tree.
     * @return the root item pointer.
     */
    ModelPart* getRootItem();

    /**
     * @brief This function appends a child to the parent item.
     * @param parent is the parent item.
     * @param data is the data of the child item.
     * @return the QModelIndex of the appended child.
     */
    QModelIndex appendChild( QModelIndex& parent, const QList<QVariant>& data );

private:
    ModelPart *rootItem;    /**< This is a pointer to the item at the base of the tree */
};
#endif
