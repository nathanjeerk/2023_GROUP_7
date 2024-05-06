#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
/**
 * @namespace Ui
 * @brief The Ui namespace contains all the UI classes used in the application.
 */

/**
 * @class OptionDialog
 * @brief The OptionDialog class is forward declared here. It is defined in the Ui namespace.
 */
class OptionDialog;
}

/**
 * @class OptionDialog
 * @brief The OptionDialog class inherits from QDialog and represents a dialog that allows the user to set options.
 */
class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the OptionDialog class.
     * @param parent is a pointer to the widget that is logically the parent of this dialog. It is passed to the QDialog constructor.
     */
    explicit OptionDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the OptionDialog class.
     */
    ~OptionDialog();

    /**
     * @brief This function returns the menu data.
     * @return the DialogData structure.
     */
    struct DialogData getMenuData();

    /**
     * @brief This function sets the menu data.
     * @param data is the new menu data.
     */
    void setMenuData(DialogData data);

private:
    /**
     * @brief A pointer to the UI of the OptionDialog class.
     */
    Ui::OptionDialog *ui;
};

/**
 * @struct DialogData
 * @brief The DialogData structure represents the data of the dialog.
 */
struct DialogData {
    QString name;        /**< The name of the dialog. */
    bool isVisible;      /**< The visibility of the dialog. */
    unsigned int R;      /**< The red component of the color of the dialog. */
    unsigned int G;      /**< The green component of the color of the dialog. */
    unsigned int B;      /**< The blue component of the color of the dialog. */
};

#endif // OPTIONDIALOG_H
