/**
 * @file changemodelcolor.h
 * @brief This file contains the declaration of the changeModelColor class.
 */

#include "changemodelcolor.h"
#include "ui_changemodelcolor.h"

/**
 * @class changeModelColor
 * @brief The changeModelColor class inherits from QDialog and represents a dialog that allows the user to change the color of a model.
 * 
 * @param parent is a pointer to the widget that is logically the parent of this dialog. It is passed to the QDialog constructor.
 */
changeModelColor::changeModelColor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changeModelColor)
{
    ui->setupUi(this);
}

/**
 * @brief Destructor for the changeModelColor class. Deletes the UI.
 */
changeModelColor::~changeModelColor()
{
    delete ui;
}
