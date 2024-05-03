#ifndef CHANGEMODELCOLOR_H
#define CHANGEMODELCOLOR_H

#include <QDialog>

/** @file changemodelcolor.h
* This file contains the declarations of UI that shows up to change the color of model
*/

namespace Ui {
class changeModelColor;
}

class changeModelColor : public QDialog
{
    Q_OBJECT

public:
    explicit changeModelColor(QWidget *parent = nullptr);
    ~changeModelColor();

private:
    Ui::changeModelColor *ui;
};

#endif // CHANGEMODELCOLOR_H
