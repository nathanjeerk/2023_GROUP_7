#include "changemodelcolor.h"
#include "ui_changemodelcolor.h"

changeModelColor::changeModelColor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changeModelColor)
{
    ui->setupUi(this);
}

changeModelColor::~changeModelColor()
{
    delete ui;
}
