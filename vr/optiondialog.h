#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    QString getText() const;
public slots:
    void handleOk();
    void handleCancel();
    /*set colour*/
    int red();
    int green();
    int blue();

    bool isVisible();

    void setName(const QString &text);
    void setVisible(const QString &text);
    void setRGB(const int& red, const int& green, const int& blue);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
