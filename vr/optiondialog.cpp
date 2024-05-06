#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

struct DialogData OptionDialog::getMenuData() {
    struct DialogData MenuData;
    MenuData.R = ui->spinBox->value();
    MenuData.G = ui->spinBox_2->value();
    MenuData.B = ui->spinBox_3->value();

    MenuData.isVisible = ui->checkBox->isChecked();
    MenuData.name = ui->lineEdit->text();

    return MenuData;
};

void OptionDialog::setMenuData(DialogData data) {
    ui->spinBox->setValue(data.R);
    ui->spinBox_2->setValue(data.G);
    ui->spinBox_3->setValue(data.B);

    ui->lineEdit->setText(data.name);
    ui->checkBox->setChecked(data.isVisible);
}