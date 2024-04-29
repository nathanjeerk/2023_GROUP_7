#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QMessageBox>
#include <QPushButton>

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    connect( ui->pushButton, &QPushButton::released, this, &OptionDialog::handleOk );
    connect( ui->pushButton_2, &QPushButton::released, this, &OptionDialog::handleCancel);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

QString OptionDialog::getText() const {
    return ui->Text->text();
}

void OptionDialog::handleOk() {
    this->accept();
}

void OptionDialog::handleCancel() {
    this->close();
}

int OptionDialog::red() {
    return ui->setRed->value();
}

int OptionDialog::green() {
    return ui->setGreen->value();
}

int OptionDialog::blue() {
    return ui->setBlue->value();
}

bool OptionDialog::isVisible() {
    return ui->isVisible->isChecked();
}

void OptionDialog::setRGB(const int &red, const int& green, const int& blue) {
    ui->setRed->setValue(red);
    ui->setGreen->setValue(green);
    ui->setBlue->setValue(blue);
}

void OptionDialog::setVisible(const QString& text) {
    if (text == "true") {
        ui->isVisible->setChecked(true);
    } else {
        ui->isVisible->setChecked(false);
    }
}

void OptionDialog::setName(const QString &text) {
    ui->Text->setText(text);
}
