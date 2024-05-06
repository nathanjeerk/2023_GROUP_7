#ifndef CHANGEMODELCOLOR_H
#define CHANGEMODELCOLOR_H

#include <QDialog>

/**
 * @file changemodelcolor.h
 * @brief This file contains the declarations of UI that shows up to change the color of model.
 */

namespace Ui {
/**
 * @namespace Ui
 * @brief The Ui namespace contains all the UI classes used in the application.
 */

/**
 * @class changeModelColor
 * @brief The changeModelColor class is forward declared here. It is defined in the Ui namespace.
 */
class changeModelColor;
}

/**
 * @class changeModelColor
 * @brief The changeModelColor class inherits from QDialog and represents a dialog that allows the user to change the color of a model.
 * 
 * @param parent is a pointer to the widget that is logically the parent of this dialog. It is passed to the QDialog constructor.
 */
class changeModelColor : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the changeModelColor class.
     * 
     * @param parent is a pointer to the widget that is logically the parent of this dialog. It is passed to the QDialog constructor.
     */
    explicit changeModelColor(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the changeModelColor class. Deletes the UI.
     */
    ~changeModelColor();

private:
    /**
     * @brief A pointer to the UI of the changeModelColor class.
     */
    Ui::changeModelColor *ui;
};

#endif // CHANGEMODELCOLOR_H
